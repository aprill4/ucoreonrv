#include"pmm.h"
#include"mem.h"

void *ROUND_UP(void *p){
	uint32 r = (uint64)p % PAGESIZE;
	return (r == 0) ? p : p + PAGESIZE - r;
}

void page_init(struct Physical_Memory_Manager *pmm) {

	extern int8 kernel_end[];
	cprintf("\nkernel end: 0x%x\n", (uint64)kernel_end);
	
	uint32 total_pages = PHYSICAL_MEMORY_SIZE / PAGESIZE;
	cprintf("\nphysical memory size: 0x%x, page size: %d, total pages: %d\n", PHYSICAL_MEMORY_SIZE, PAGESIZE, total_pages);

	struct Page *base_page = (struct Page *)kernel_end;
	uint32 page_struct_size = sizeof(struct Page) * total_pages;
	void *page_struct_end = (void *)base_page + page_struct_size;
	cprintf("\npage base: 0x%x\n", (uint64)base_page);
	cprintf("\npage struct end: 0x%x\n", (uint64)page_struct_end);
	
	void *page_struct_align = ROUND_UP((void *)page_struct_end);
	cprintf("\npage struct align: 0x%x\n", (uint64)page_struct_align);

	uint32 reserved_pages = ((uint64)page_struct_align - PHYSICAL_MEMORY_BASE_PADDR) / PAGESIZE + KSTACKPAGE;
	cprintf("\nreserved pages * sizeof(Page) = %d * %d\n", reserved_pages, sizeof(struct Page));

	pmm -> num_free_pages = total_pages - reserved_pages;

	struct Page *page = base_page;

	for (int i = 0; i < reserved_pages; i++){
		page -> flags = PAGE_RESERVED;
		page++;
	}
	
	struct Page *free_page_base = page;
	cprintf("\nfree page base: 0x%x\n", (uint64)free_page_base);

	free_page_base -> property = total_pages - reserved_pages;
	free_page_base -> flags = PAGE_FREE;
	free_page_base -> page_link.prev = NULL;
	free_page_base -> page_link.next = NULL;

	pmm -> free_list = &(free_page_base -> page_link);

	page++;
	for (int i = reserved_pages + 1; i < total_pages; i++){
		page -> flags = PAGE_FREE;
		page -> ref = page -> property = 0;
		page -> page_link.prev = page -> page_link.next = NULL;
		page++;
	}
}

void insert_le(struct Physical_Memory_Manager *pmm, struct Page *p, uint32 n){
	struct Page *insert = p + n;
	insert -> property = p -> property - n;
	insert -> page_link.prev = p -> page_link.prev;
	insert -> page_link.next = p -> page_link.next;

	if (p -> page_link.prev == NULL){
		pmm -> free_list = &(insert -> page_link);
	}

	cprintf("best_block + %d = 0x%x\n", n, (uint64)insert);
	cprintf("insert -> property: %d\n", insert -> property);

	if (p -> page_link.prev != NULL){
		p -> page_link.prev -> next = &(insert -> page_link);
	}

	if (p -> page_link.next != NULL){
		p -> page_link.next -> prev = &(insert -> page_link);
	}
}

struct Page *alloc_pages(struct Physical_Memory_Manager *pmm, uint32 n){
	if (n == 0){
		cprintf("ni you bing!\n");
		return NULL;
	}

	if (pmm -> num_free_pages < n){
		cprintf("I have only %d pages, but you want %d pages.\n", pmm -> num_free_pages, n);
		return NULL;
	}
	
	struct Page *best_block = NULL;

	for (struct list_entry *free_block = pmm -> free_list; free_block; free_block = free_block -> next){
		struct Page *page = le2page(free_block);

		cprintf("\n0x%x(property: %d)\n", (uint64)page, page -> property);

		if (page -> property >= n && (best_block == NULL || page -> property < best_block -> property)){
			best_block = page;
		}
	}

	cprintf("\nbest_block: 0x%x(property: %d)\n", (uint64)best_block, best_block -> property);
	if (best_block -> property > n){
		insert_le(pmm, best_block, n);
	}
	else{

		if (best_block -> page_link.prev == NULL){
			pmm -> free_list = best_block -> page_link.next;
		}
		else{

			best_block -> page_link.prev -> next = best_block -> page_link.next;
		}

		if (best_block -> page_link.next != NULL){
			best_block -> page_link.next -> prev = best_block -> page_link.prev;
		}
	}

	best_block -> property = n;
	for (struct Page *page = best_block; page < best_block + n; page++){
		page -> ref += 1;
		page -> flags &= ~PAGE_FREE; 
	}

	pmm -> num_free_pages -= n;

	return best_block;
} 

void merge_free_blocks(struct Physical_Memory_Manager *pmm){
	Page *cur_block, *next_block;

	for (struct list_entry *free_block = pmm -> free_list; free_block;){
		cur_block = le2page(free_block);
		if (free_block -> next == NULL) break;
		next_block = le2page(free_block -> next);
		if (cur_block + cur_block -> property == next_block){
			cur_block -> property = cur_block -> property + next_block -> property;
			cur_block -> page_link.next = next_block -> page_link.next;
			if (next_block -> page_link.next != NULL){
				next_block -> page_link.next -> prev = &(cur_block -> page_link);
			}
		}
		else{
			free_block = free_block -> next;
		}
	}
}

void free_pages(struct Physical_Memory_Manager *pmm, struct Page *p){
	uint32 n = p -> property;
	if (n == 0){
		cprintf("no page\n");
	}
	
	for (struct Page *page = p; page < p + n; page++){
		page -> ref -= 1;
		page -> flags |= PAGE_FREE; 
	}

	struct Page *page = NULL;
	for (struct list_entry *free_block = pmm -> free_list; free_block; free_block = free_block -> next){
		page = le2page(free_block);
		if ((uint64)page > (uint64)p){
			break;		
		}
	}
	p -> page_link.next = &(page -> page_link);
	p -> page_link.prev = page -> page_link.prev;
	if (p -> page_link.prev == NULL){
		pmm -> free_list = &(p -> page_link);
	}
	if (page -> page_link.prev != NULL){
		page -> page_link.prev -> next = &(p -> page_link);
	}
	page -> page_link.prev = &(p -> page_link);
}

void check(struct Physical_Memory_Manager *pmm){
	cprintf("check alloc_pages\n");

	uint32 n = 3;
	struct Page *t1 = alloc_pages(pmm, n);
	struct Page *t2 = alloc_pages(pmm, n);
	struct Page *t3 = alloc_pages(pmm, n);

	cprintf("free list :\n");
	for (struct list_entry *free_block = pmm -> free_list; free_block; free_block = free_block -> next){
		struct Page *page = le2page(free_block);
		cprintf("0x%x(property: %d) -> ", (uint64)page2pa(page), page -> property);
	}
	
	cprintf("\n\ncheck free pages\n");
	free_pages(pmm, t3);

	cprintf("free list :\n");
	for (struct list_entry *free_block = pmm -> free_list; free_block; free_block = free_block -> next){
		struct Page *page = le2page(free_block);
		cprintf("0x%x(property: %d) -> ", (uint64)page2pa(page), page -> property);
	}

	cprintf("\n\nmerge free blocks\n");
	merge_free_blocks(pmm);

	cprintf("free list :\n");
	for (struct list_entry *free_block = pmm -> free_list; free_block; free_block = free_block -> next){
		struct Page *page = le2page(free_block);
		cprintf("0x%x(property: %d) -> ", (uint64)page2pa(page), page -> property);
	}

	cprintf("\n\nalloc pages again\n");
	alloc_pages(pmm, n);

	cprintf("free list :\n");
	for (struct list_entry *free_block = pmm -> free_list; free_block; free_block = free_block -> next){
		struct Page *page = le2page(free_block);
		cprintf("0x%x(property: %d) -> ", (uint64)page2pa(page), page -> property);
	}
	
	cprintf("\ncheck ends\n");
}

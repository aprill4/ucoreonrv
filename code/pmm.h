#ifndef PMM_H
#define PMM_H
#include"defs.h"
#include"stdio.h"
#include"mem.h"

struct list_entry{
	struct list_entry *prev, *next;
};

typedef struct list_entry list_entry;
#define PAGE_RESERVED 1
#define PAGE_FREE 2

#define le2page(list_entry_p) \
	(struct Page *)(((void *)list_entry_p) - 16)

#define PAGE_STRUCT_BASE 0x80204000 //TODO:assert
#define page2pa(page_p) \
	(void *)(((uint64)page_p - PAGE_STRUCT_BASE) / sizeof(Page) * PAGESIZE + PHYSICAL_MEMORY_BASE_PADDR)

typedef struct Page{
	uint32 ref;
	uint32 flags;
	uint32 property;
	struct list_entry page_link;
} Page;

typedef struct Physical_Memory_Manager{
	uint32 num_free_pages;
	struct list_entry *free_list;
} Physical_Memory_Manager;

void *ROUND_UP(void *p);

void page_init(struct Physical_Memory_Manager *pmm);
//Page* alloc_pages(uint32 n, PHYSICAL_MEMORY_Manager *pmm);
struct Page *alloc_pages(struct Physical_Memory_Manager *pmm, uint32 n);
void check(struct Physical_Memory_Manager *pmm);
void free_pages(struct Physical_Memory_Manager *pmm, struct Page *p);
#endif


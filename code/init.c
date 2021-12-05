#include"trap.h"
#include"riscv.h"
#include"stdio.h"
#include"clock.h"
//#include"print_dtb.h"
#include"mem.h"
#include"pmm.h"


void kern_init(){
	/*
	asm volatile(
		"mv a0, a1\n"
		"tail print_dtb\n"
	);
	*/

	const char *hello = "hello, myOS!\n";
	cprintf(hello);

	struct Physical_Memory_Manager pmm;
	page_init(&pmm);
	check(&pmm);

	idt_init(); // trap_entry

	intr_enable(); // enable interrupt

	clock_init(); // enable clock 


	while(1);
}

#include"trap.h"
#include"riscv.h"
#include"stdio.h"
#include"clock.h"

void kern_init(){
	const char *hello = "hello, myos!\n";
	cput(hello);
	
	idt_init();

	intr_enable();

	clock_init();


	while(1);
}

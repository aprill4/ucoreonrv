#include"clock.h"
#include"stdio.h"
#include"riscv.h"
#include"trap.h"

#define TICK_NUM 100

extern uint64 ticks;

void idt_init(){
	extern void trap_entry(void); 
	//sscratch用于区分中断前处于什么状态
	write_csr(CSR_SSCRATCH, 0);
	write_csr(CSR_STVEC, &trap_entry);
}

void intr_enable(){ set_csr(CSR_SSTATUS, SSTATUS_SIE);}
void intr_disable(){clear_csr(CSR_SSTATUS, SSTATUS_SIE);}


void print_ticks(){
//printf()
	cprintf("%d ticks\n", ticks);
}

void interrupt_handler(struct trapframe* tf){
	uint64 cause = (tf->cause << 1) >> 1;
	switch(cause){
		case IRQ_S_TIMER:{
			clock_set_next_event();
			ticks++;
			if (ticks == 300){
				asm volatile("mret");
			}
			else if (ticks % TICK_NUM == 0){ 
				print_ticks();
			}
		break;
	 }
	}
}

void exception_handler(struct trapframe* tf){
	uint64 cause = tf->cause;
	switch(cause){
		case 2:{
				cput("illegal inst!\n");
				tf->epc += 4;
			}
		break;
	}
}

void trap_dispatch(struct trapframe* tf){
	if (tf->cause < 0){
		interrupt_handler(tf);
	}
	else
		exception_handler(tf);
}
void trap(struct trapframe* tf){
	trap_dispatch(tf);
}

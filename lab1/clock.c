#include"clock.h"
#include"sbi.h"
#include"riscv.h"
#include"defs.h"
#include"stdio.h"

volatile uint64 ticks;

static uint64 get_time(){
	uint64 n;
	asm volatile("rdtime %0" : "=r"(n));
	return n;
}

static uint64 timebase = 100000;

void clock_init(){
	uint64 tmp = 1 << IRQ_S_TIMER;
	set_csr(CSR_SIE, tmp);
	clock_set_next_event();
	ticks = 0; 
	cput("++ setup timer interrupts\n");	
}

void clock_set_next_event(){
	sbi_set_timer(get_time() + timebase);
}

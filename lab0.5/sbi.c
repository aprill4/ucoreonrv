#include"defs.h"
#include"sbi.h"

uint64 SBI_CONSOLE_PUTCHAR = 1;
uint64 SBI_CONSOLE_GETCHAR = 2;

uint64 sbi_call(uint64 sbi_type, uint64 arg0, uint64 arg1, uint64 arg2){
	uint64 ret_val;
	__asm__ volatile(
		"mv x17, %[sbi_type]\n"
		"mv x10, %[arg0]\n"
		"mv x11, %[arg1]\n"
		"mv x12, %[arg2]\n"
		"ecall\n"
		"mv %[ret_val], x10"
		:[ret_val] "=r" (ret_val)
		:[sbi_type] "r" (sbi_type), [arg0] "r" (arg0), [arg1] "r" (arg1), [arg2] "r" (arg2)
		:"memory"
		);
	return ret_val;
}

void sbi_putchar(char c){
	sbi_call(SBI_CONSOLE_PUTCHAR, c, 0, 0);
}

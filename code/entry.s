#include"mem.h"

.section .text, "ax", %progbits
.global kern_entry

kern_entry:
	la sp, bootstacktop
	tail kern_init
	
.section .data 
	.align 12
	.global bootstack
bootstack: 
	.space 8192
	.global bootstacktop
bootstacktop:



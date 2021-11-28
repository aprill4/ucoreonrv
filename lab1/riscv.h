#ifndef RISCV_H
#define RISCV_H

#define set_csr(csr, reg)({\
	asm volatile("csrrs x0, %0, %1" :: "i"(csr), "r"(reg));\
	})

#define write_csr(csr, reg)({\
	asm volatile("csrw %0, %1" :: "i"(csr), "r"(reg));\
	})

#define clear_csr(csr, reg)({\
	asm volatile("csrrc x0, %0, %1" :: "i"(csr), "r"(reg));\
	})

#define SSTATUS_SIE 0x00000002
#define CSR_SIE 0x104
#define IRQ_S_TIMER 5
#define CSR_SSCRATCH 0x140
#define CSR_SSTATUS 0x100
#define CSR_STVEC 0x105
#define CSR_SEPC  0x141
#define CSR_SBADADDRESS 0x143
#define CSR_SCAUSE 0x142
#endif


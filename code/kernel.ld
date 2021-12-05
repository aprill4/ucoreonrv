OUTPUT_ARCH(riscv)
ENTRY(kern_entry)

BASE_ADDRESS = 0x80200000;

SECTIONS
{
	. = BASE_ADDRESS;
	.text : {
		*(.text)
	}

	PROVIDE(etext = .);

	.data : {
		*(.data)
	}
	
	PROVIDE(edata = .);

	.bss : {
		*(.bss)
		*(.bss.*)
		*(.sbss*)
	}
	
	. = ALIGN(0x1000);

	PROVIDE(kernel_end = .);
}

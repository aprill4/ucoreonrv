OUTPUT_ARCH(riscv)
ENTRY(kern_entry)

BASE_ADDRESS = 0x80200000;

SECTIONS
{
	. = BASE_ADDRESS;
	.text : {
		*(.text)
	}

	.data : {
		*(.data)
	}
	
	.sdata : {
		*(.sdata)
		*(.sdata.*)
	}
}

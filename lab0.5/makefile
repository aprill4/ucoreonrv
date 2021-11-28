GCCPREFIX := riscv64-unknown-elf-

OBJCOPY :=$(GCCPREFIX)objcopy 
CC := $(GCCPREFIX)gcc
LD := $(GCCPREFIX)ld
CFLAGS := -Wall -Werror -Wno-unused --std=gnu99 -mcmodel=medany -g -Os
LDFLAGS := -m elf64lriscv -nostdlib -T kernel.ld --gc-sections

obj = entry.o init.o sbi.o stdio.o

silly_little_ucore.img: init
	$(OBJCOPY) init --strip-all -O binary silly_little_ucore.img

init: $(obj) kernel.ld
	$(LD) $(LDFLAGS) $(obj) -o init 

sbi.o: defs.h sbi.h sbi.c
	$(CC) $(CFLAGS) -c sbi.c

stdio.o: sbi.h stdio.h stdio.c
	$(CC) $(CFLAGS) -c stdio.c

entry.o: mem.h entry.s
	$(CC) $(CFLAGS) -c entry.s

init.o: stdio.h defs.h init.c
	$(CC) $(CFLAGS) -c init.c

.PHONY: clean
clean:
	rm init $(obj) silly_little_ucore.img init

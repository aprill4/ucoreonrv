#ifndef MEM_H
#define MEM_H

#define PAGESIZE 4096
#define PAGESHIFT 12

#define PHYSICAL_MEMORY_BASE_PADDR 0x80000000
#define KERNEL_BASE_PADDR 0x80200000
#define PHYSICAL_MEMORY_END 0x88000000
#define PHYSICAL_MEMORY_SIZE 0x08000000

#define KSTACKPAGE 2
#define KSTACKSIZE (KSTACKPAGE * PAGESIZE)

#endif

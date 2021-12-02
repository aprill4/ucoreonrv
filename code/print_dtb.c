// $ gcc -o # @
#include<stdio.h>
#include"defs.h"
#include"print_dtb.h"

struct fdt_header{
	uint32 magic;
	uint32 totalsize;
	uint32 off_dt_struct;
	uint32 off_dt_strings;
	uint32 off_mem_rsvmap;
	uint32 version;
	uint32 last_comp_version;
	uint32 boot_cpuid_phys;
	uint32 size_dt_struct;
	uint32 size_dt_strings;
};

uint32 convert_be_to_le(uint32 t){
	return ((t >> 24) & 0xff) | ((t >> 8) & 0xff00) | ((t << 8) & 0xff0000) | ((t <<24) & 0xff000000);
}

void print_dtb(u32 *p) {
	struct fdt_header *p = (struct fdt_header *)p;
	uint32 struct_off = p -> off_dt_struct;
	uint32 strings_off = p -> off_dt_strings;
	uint32 struct_addr = (uin32 *)p + struct_off;
	uint32 strings_addr = (uint32 *)p + strings_off;

}
/*
int main(){
	uint32 x = 0x01020304;
	uint32 y = convert_be_to_le(x);
	printf("x: %x, y: %x\n", x, y);
	return 0;
}
*/

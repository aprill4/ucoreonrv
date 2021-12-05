// $ gcc -o # @
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

struct PROP{
	uint32 len;
	uint32 nameoff;
};

uint32 convert_be_to_le(uint32 t){
	return ((t >> 24) & 0xff) | ((t >> 8) & 0xff00) | ((t << 8) & 0xff0000) | ((t <<24) & 0xff000000);
}

enum Structure_Token{
	FDT_BEGIN_NODE = 0x01000000,
	FDT_END_NODE = 0x02000000,
	FDT_PROP = 0x03000000,
	FDT_NOP = 0x04000000,
	FDT_END = 0x09000000
};

bool is_valid_char(char c){
	return (c == ',') || (c == '.') || (c == '-') || (c == '_') || (c == '+') || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (a >= 'A' && a <= 'Z');
}

uint32* skip_padding_zeros(uint32* p){

	uint8** ptr = &p;
	
	if ((uint64)(*ptr) % 4 != 0){
		(*ptr)++;
	}
	return *ptr;
}

void print_dtb(uint32 *p) {
	struct fdt_header *p = (struct fdt_header *)p;
	uint32 struct_off = p -> off_dt_struct;
	uint32 strings_off = p -> off_dt_strings;
	uint32* token = (uint32 *)p + convert_be_to_le(struct_off);
	uint32* strings = (uint32 *)p + convert_be_to_le(strings_off);

	while (*token != FDT_END) {
		switch(*token){
			case FDT_BEGIN_NODE: {
				token++;
				u8* s = (u8 *)token;
				while (*s){
					printf("%c", *s);
					s++;
				}

				s++; // skip '\0'

				token = skip_padding_zeros((uint32 *)s);
				uint32 addr = *token;
				printf("0x%x\n", addr);
				token++;
			} break;
			case FDT_END_NODE: {
				token++;
		    } break;
			case FDT_PROP: {
				token++;
				struct PROP *prop = (struct PROP *)token;
				uint32 len = convert_be_to_le(prop -> len);
				uint32 nameoff = convert_be_to_le(prop -> nameoff);
				uint8 *name = (uint8 *)(strings + nameoff);
				token += 2;

				printf("%s\n", name);

				uint8* s = (uint8 *)token;
				bool is_str = 1;
				for (int i = 0; i < len; i++){
					if (!is_valid_char(s[i] && s[i])) {
						is_str = 0;
						break;
					}
				}

				if (is_str){
					printf("%s\n", s);
					s++;
					token = skip_padding_zeros((uint32 *)s);
				} else {
					uint32 num_bytes = len / sizeof(uint32);
					for (uint32 = 0; i < num_bytes; i++){
						printf("0x%x", convert_be_to_le(*(token++)));
					}
				}

				//
		    } break;
			case FDT_NOP: {
				token++;
		    } break;
			default:
				break;
		}
	}
	
}
/*
int main(){
	uint32 x = 0x01020304;
	uint32 y = convert_be_to_le(x);
	printf("x: %x, y: %x\n", x, y);
	return 0;
}
*/

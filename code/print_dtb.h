#ifndef PRINT_DTB_H
#define PRINT_DTB_H

#include"defs.h"

void print_dtb(uint32* p);
uint32 convert_be_to_le(uint32 t);
bool is_valid_char(char c);
uin32* skip_padding_zeros(uint32* p);


#endif

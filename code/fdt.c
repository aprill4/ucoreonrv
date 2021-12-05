#ifndef _FDT_H
#define _FDT_H

#include "print.h"
#include "type.h"

// @CLEANUP: burn all these to the ground

typedef struct {
    u32 magic;
    u32 totalsize;
    u32 off_dt_struct;
    u32 off_dt_strings;
    u32 off_mem_rsvmap;
    u32 version;
    u32 last_comp_version;
    u32 boot_cpuid_phys;
    u32 size_dt_strings;
    u32 size_dt_struct;
} FDT_Header;

struct fdt_reserve_entry {
    u64 address;
    u64 size;
};

typedef struct {
    u32 len;
    u32 nameoff;
} FDT_Property;

enum Structure_Token {
    FDT_BEGIN_NODE = 0x01000000,
    FDT_END_NODE   = 0x02000000,
    FDT_PROP       = 0x03000000,
    FDT_NOP        = 0x04000000,
    FDT_END        = 0x09000000
};

u32 load_be32(u32 *mem) {
    // 0x12345678 -> 0x78563412
    u32 t = *mem;
    u32 le = ((t & 0xff000000) >> 24) | ((t & 0x00ff0000) >> 8) |
             ((t & 0x0000ff00) << 8) | ((t & 0x000000ff) << 24);
    return le;
}

// Ref: https://devicetree-specification.readthedocs.io/en/stable/devicetree-basics.html#id4
static int is_node_character(i8 c) {
    return (is_digit(c) || is_alpha(c) || c == ',' || c == '.' || c == '_' ||
            c == '+' || c == '-');
}

void skip_padding_bytes(u8 **p) {
    while ((u64)(*p) % 4 != 0) {
        (*p)++;
    }
}

#define PRINT_N_TABS(N)                                                        \
    do {                                                                       \
        for (int i = 0; i < (N); i++)                                          \
            print("    ");                                                     \
    } while (0)

void print_devicetree(FDT_Header *fdt_header) {
    // @TODO: assert magic number
    // print("0x%x", fdt_header->magic);
    
    u32 *structure_block = ((void *)(fdt_header) + load_be32(&fdt_header->off_dt_struct));
    u8 *strings_block    = ((void *)(fdt_header) + load_be32(&fdt_header->off_dt_strings));
    u32 token;

    u32 indent_level = 0;

    // iterate over the device tree
    u32 *p = structure_block;
    while ((token = *p) != FDT_END) {
        switch (token) {
            case FDT_BEGIN_NODE: {
                PRINT_N_TABS(indent_level);
                indent_level++;

                p++;
                u8 *s = (u8 *)p;
                while (*s != 0) {
                    print_char(*s++);
                }
                s++; // skip string terminator null
                skip_padding_bytes(&s);

                print(" {\n");

                p = (u32 *)s;

            } break;

            case FDT_END_NODE: {
                indent_level--;
                PRINT_N_TABS(indent_level);
                print("};\n\n");
                p++;
            } break;

            case FDT_PROP: {
                p++;

                FDT_Property *property = (FDT_Property *)p;
                u32 string_offset = load_be32(&property->nameoff);
                u32 value_length  = load_be32(&property->len);
                p += 2;

                PRINT_N_TABS(indent_level);
                print("%s", strings_block + string_offset);

                if (value_length != 0) {
                    print(" = ");
                    u8 *s = (u8 *)p;

                    b32 is_byte_stream = 0;
                    // @NOTE: that value_length would take the null terminator into account
                    for (i32 i = 0; i < value_length - 1; i++) {
                        if (!is_print(s[i]) && s[i] != '\0') {
                            is_byte_stream = 1;
                            break;
                        }
                    }

                    if (is_byte_stream) {
                        i32 num_bytes = value_length / sizeof(u32);
                        print_char('<');
                        for (i32 i = 0; i < num_bytes; i++) {
                            print("0x%x", load_be32(p++));
                            if (i != num_bytes - 1) print_char(' ');
                        }
                        print_char('>');
                    } else {
                        print_char('"');
                        for (i32 i = 0; i < value_length - 1; i++) {
                            print_char(*s++);
                        }
                        print_char('"');

                        s++;
                        skip_padding_bytes(&s);

                        p = (u32 *)s;
                    }

                }

                print(";\n");
            } break;

            case FDT_NOP: {
                p++;
            } break;

            default: {
                /*
                u8 *s = (u8 *)p;
                print("skip %x, %c\n", *s, *s);
                s++;
                p = (u32 *)s;
                break;
                */
                print("error, %x\n", token);
                while(1) {}
            } break;
        }
    }
}

#endif

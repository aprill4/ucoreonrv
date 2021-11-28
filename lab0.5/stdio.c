#include"sbi.h"

void cputchar(char c){
	sbi_putchar(c);
}

int cput(const char *str){
	int len = 0;
	const char* c = str;
	while ( *(c) != '\0') {
		cputchar(*c);
		c++;
		len++;
	}
	return len;
}

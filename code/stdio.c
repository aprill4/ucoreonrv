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

void cprintf(const char *str, unsigned int x){
	const char *c = str;
	while (*c != '\0'){
		if (*c == '%' && *(c+1) == 'd'){
			char num[10];
			int i = 0;
			while (x != 0){
				int re = x % 10;
				x = x / 10;
				num[i] = re;
				i++;
			}
			for (int j = i-1; j >= 0; j--)
				cputchar('0' + num[j]);
			c += 2;
		}
		if (*c == '\0') return;
		cputchar(*c);
		c++;
	}
}

// $ gcc -o # @ 
#include"sbi.h"
//#include<stdio.h>
//#include<stdarg.h>

void cputchar(char c){
	sbi_putchar(c);
	//putchar(c);
}

void prints(const char *c){
	while (*c) {
		cputchar(*c);
		c++;
	}
}

void printd(unsigned int x){
	char num[16];
	int i = 0;
	if (x == 0) {
		cputchar('0');
		return;
	}
	while (x != 0){
		int re = x % 10;
		x = x / 10;
		num[i++] = re;
	}
	for (int j = i-1; j >= 0; j--)
		cputchar('0' + num[j]);
}

void printc(char c){
	cputchar(c);
}

void printx(unsigned int d){
	char x[24];
	int i = 0;
	if (d == 0) {
		cputchar('0');
		return;
	}
	while (d != 0){
		int re = d % 16;
		d = d / 16;
		x[i++] = re;
	}
	for (int j = i - 1; j >= 0; j--)
		switch(x[j]){
			case 15: cputchar('F'); break;
			case 14: cputchar('E'); break;
			case 13: cputchar('D'); break;
			case 12: cputchar('C'); break;
			case 11: cputchar('B'); break;
			case 10: cputchar('A'); break;
			default: cputchar('0' + x[j]); break;
		}
}

void cprintf(const char *fmt, ...){
	__builtin_va_list ap;
	__builtin_va_start(ap, fmt);

	char* s;
	unsigned int d;
	char c;

	while (*fmt){
		if (*fmt != '%'){
			cputchar(*fmt);
			fmt++;
			continue;
		}

		fmt++;

		if (!(*fmt)){
			break;
		}

		switch (*fmt){
			case 's':
				s = __builtin_va_arg(ap, char*);
				prints(s);
				break;
			case 'd':
				d = __builtin_va_arg(ap, unsigned int);
				printd(d);
				break;
			case 'c':
				c = (char)__builtin_va_arg(ap, int);
				printc(c);
				break;
			case 'x':
				d = __builtin_va_arg(ap, unsigned int);
				printx(d);
				break;
			default:
				break;
		}
		fmt++;
	}
	__builtin_va_end(ap);
}

/* test
int main(){
	char *s = "hello";
	char c = 'l';
	int d = 31;
	cprintf("s: %s, c: %c, d: %d, x: %x\n", s, c, d, d);
	return 0;
}
*/

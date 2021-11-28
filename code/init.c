#include"stdio.h"

void kern_init(){
	const char *hello = "hello, myos!";
	cput(hello);

	while(1);
}

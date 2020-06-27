#include "malloc.h"


int main(void)
{
	void *ptr1;

	ptr1 = malloc(100);
	ptr1 = malloc(100);
	ptr1 = malloc(100);
	ptr1 = malloc(100);
	ptr1 = malloc(100);
	ptr1 = malloc(100);
	ptr1 = malloc(100);
	ptr1 = malloc(100);
	ptr1 = malloc(100);
	ptr1 = malloc(100);
	ptr1 = malloc(100);
	ptr1 = malloc(100);
	ptr1 = malloc(100);
	ptr1 = malloc(100);
	ptr1 = malloc(100);
	ptr1 = malloc(100);
	ptr1 = malloc(100);
	show_alloc_mem_ex();
	ptr1 = malloc(1000);
	show_alloc_mem_ex();

	return (0);
}

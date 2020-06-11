#include "malloc.h"

void	header_zone(void *p, size_t i)
{
	if (i == 0)
		printf("%25s -- %p\n", "TINY", p);
	else if (i == 1)
		printf("%25s -- %p\n", "SMALL", p);
	else if (i == 2)
		printf("%25s -- %p\n", "LARGE", p);
}

void	print_chunk(t_block *curr)
{
	if (curr->size & 1)
	{
		printf(MGREEN);
		printf("%20p - %p  => ", (char *)curr + HSIZE, (char *)curr + (HSIZE + (curr->size & -2)));
		printf("%zu bytes allocated\n", curr->size & -0x4);
	}
	else
	{
		printf(MBLUE);
		printf("%20p - %p  => ", (char *)curr + HSIZE, (char *)curr + (HSIZE + (curr->size & -2)));
		printf("%zu bytes available\n", curr->size & -0x4);
	}
	printf(MWHITE);
}

void	show_alloc_mem_ex(void)
{
	t_block		*bl;
	size_t		i;

	i = 0;
	printf(MWHITE);
	while (i <= large)
	{
		bl = g_mem[i];
		while (bl)
		{
			if (bl->size & 2)
				header_zone(bl, i);
			print_chunk(bl);
			bl = bl->nx;
		}
		i++;
	}
}

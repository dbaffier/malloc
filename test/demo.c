#include "malloc.h"
# define N 42

void	new_line(int i)
{
	if (i != 0 && i % 64 == 0)
		printf("\n");
	if (!(i % 64))
	{
		printf(MWHITE);
		printf("%.8X ", i);
	}
}

void	print_memory(int i, t_block *memory, int header, int size)
{
	t_block *t;

	t = memory;
	while (++i < 4096)
	{
		new_line(i);
		if (!size)
		{
			t = t->nx;
			header = HSIZE;
			size = t->size & ~0x3;
		}
		if (header && header-- >= -17)
			printf(t->size & 1 ? MRED : MGREEN);
		else
		{
			printf(t->size & 1 ? MORANGE : MLG);
			size--;
		}
		printf("%.2X", ((unsigned char *)memory)[i]);
		if (i % 64 != 63)
			printf(" ");
	}
}

void	hexdumper(t_block *memory)
{
	printf("\e[1J");
	print_memory(-1, memory, HSIZE, memory->size & ~0x3);
	printf("\n");
	usleep(500000);
}

void	demo_loop(unsigned char *ptr[N], bool u[N], int i)
{
	unsigned int r;
	unsigned int j;

	while (++i < N)
		if (u[i] == false)
		{
			r = rand() % 100;
			ptr[i] = (unsigned char *)malloc(r);
			j = -1;
			while (++j < r)
				ptr[i][j] = 0x88;
			u[i] = true;
		}
	hexdumper(g_mem[0]);
	i = -1;
	while (++i < N)
		if (u[i] && !(rand() % 5))
		{
			free(ptr[i]);
			u[i] = false;
		}
	hexdumper(g_mem[0]);
}

int		main(void)
{
	unsigned char	*ptr[N];
	bool			u[N];
	int				i;

	srand(time(0));
	i = -1;
	while (++i < N)
	{
		u[i] = false;
		ptr[i] = NULL;
	}
	while (1)
		demo_loop(ptr, u, -1);
	return (0);
}

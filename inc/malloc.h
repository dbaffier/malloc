#ifndef MALLOC_H
#define MALLOC_H

#include "libft.h"
#include <stdio.h>
#include "ft_printf.h"
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
# include <stdbool.h>

# define MRED "\033[1;38;2;225;20;20m"
# define MORANGE "\033[1;38;2;255;120;10m"
# define MYELLO "\033[1;38;2;255;200;0m"
# define MGREEN "\033[1;38;2;0;255;101m"
# define MBLUE "\033[1;38;2;0;255;255m"
# define MLG "\033[1;38;2;167;244;66m"
# define MPURPLE "\033[1;38;2;255;75;255m"
# define MWHITE "\033[1;38;2;255;250;255m"
# define P(X, S) ft_putstr(X); ft_putnbr(S); ft_putchar('\n');

# define TINY 128
# define TINY_PAGE getpagesize() * 4
# define SMALL 1024
# define SMALL_PAGE getpagesize() * 32

# define MAPPING PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0

# define ALIGNMENT 16
# define ALIGN(size) (((size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1)))
# define ALIGN_PAGE(size) (((size + (getpagesize() - 1)) & ~(getpagesize() - 1)))

# define USE 1

# define HSIZE sizeof(t_block)
# define ADDR(x) (unsigned char *)(x) + HSIZE
# define PACK(size, alloc) ((size) | (alloc))
# define SIZEH(size) size + HSIZE

# define PAGESIZE getpagesize()

enum {
	tiny,
	small,
	large
};

typedef struct	s_block
{
	unsigned int	size;
	struct s_block	*nx;
}				t_block;


void	*malloc(size_t size);
void	free(void *ptr);
void	*realloc(void *ptr, size_t size);
void	show_alloc_mem_ex(void);

extern t_block *g_mem[3];

#endif

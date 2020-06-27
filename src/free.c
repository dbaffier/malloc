#include "malloc.h"

pthread_mutex_t g_mut;

static t_block	*ft_find(void *ptr, t_block **prev, size_t *grp)
{
	t_block		*save;

	*grp = tiny - 1;
	while (++(*grp) <= large) // loop over grp
	{
		save = g_mem[*grp];
		if (prev) // save prev
			*prev = save;
		while (save) // loop over grp list
		{
			if ((save->size & 1) && ADDR(save) == ptr)
				return (save);
			if (prev)											// previous ptr
				*prev = save;
			save = save->nx;
		}
	}
	return (NULL);
}

t_block *coalescence(t_block *curr, t_block *prev)
{
	size_t	f;

	f = prev && prev->size & 2 ? 1 : 0;
	if (prev && !(prev->size & 1) && !(curr->size & 2)) // if prev not malloc + curr not first MMAP
	{
		if (curr->nx && !(curr->nx->size & 3)) // if next + next is not malloc and not first MMAP
		{
			if (f)
				prev->size = PACK((prev->size & ~0x3) + curr->nx->size + HSIZE, 0x2);
			else
				prev->size = (prev->size & ~0x3) + curr->nx->size + HSIZE;
			prev->nx = curr->nx->nx;
		}
		else
			prev->nx = curr->nx;
		if (f)
			prev->size = PACK((prev->size & ~0x3) + (curr->size & ~0x3) + HSIZE, 0x2);
		else
			prev->size = (prev->size & ~0x3) + (curr->size & ~0x3) + HSIZE;
		return (prev);
	}
	else
	{
		if (curr->nx && !(curr->nx->size & 3)) // if next + next not malloc and not first
		{
			if (curr->size & 2)
				curr->size = PACK((curr->size & ~0x3) + curr->nx->size + HSIZE, 0x2);
			else
				curr->size = (curr->size & ~0x3) + curr->nx->size + HSIZE;
			curr->nx = curr->nx->nx;
		}
	}
	return (curr);
}

void	adapt(size_t grp, t_block *prev)
{
	t_block		*list;

	list = g_mem[grp];
	while (list) // loop over grp list
	{
		if (list->nx && list->nx == prev)
			list->nx = prev->nx;
		list = list->nx;
	}
}

void	free(void *ptr)
{
	size_t		grp;
	t_block		*find;
	t_block		*prev;

	if (ptr == NULL)
		return ;
	//show_alloc_mem_ex();
	//printf("GOING TO FREE : %p--", ptr);
	pthread_mutex_lock(&g_mut);
	if (!(find = ft_find((unsigned char *)ptr, &prev, &grp)))
		return ;
	find->size &= -0x2; // REMOVE MALLOCATED BYTE
	if (grp < large)
		find = coalescence(find, prev);
	if ((grp == tiny && find && (find->size & ~0x3) == (size_t)TINY_PAGE)
		|| (grp == small && find && (find->size & ~0x3) == (size_t )SMALL_PAGE)
		|| grp == large)
	{
		if (g_mem[grp] == find)
			g_mem[grp] = find->nx;
		else if (prev && prev->nx != find->nx)
			prev->nx = find->nx;
		else
			adapt(grp, prev);
		munmap((unsigned char *)find, find->size & ~0x3);
	}
	pthread_mutex_unlock(&g_mut);
}

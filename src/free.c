#include "malloc.h"

pthread_mutex_t g_mut;

static t_block	*ft_find(void *ptr, t_block **prev, size_t *grp, t_block **last_page)
{
	t_block		*save;

	*grp = tiny - 1;
	while (++(*grp) <= large)
	{
		save = g_mem[*grp];
		if (prev)
			*prev = save;
		while (save)
		{
			if (last_page && save->nx && (save->nx->size & 2))
				*last_page = save;
			if ((save->size & 1) && ADDR(save) == ptr)
				return (save);
			if (prev)
				*prev = save;
			save = save->nx;
		}
	}
	return (NULL);
}

t_block *coalescence(t_block *curr, t_block *prev)
{
	if (prev && !(prev->size & 1) && !(curr->size & 2))
	{
		if (curr->nx && !(curr->nx->size & 3))
		{
			prev->size += curr->nx->size + HSIZE;
			prev->nx = curr->nx->nx;
		}
		else
			prev->nx = curr->nx;
		prev->size += curr->size + HSIZE;
		return (prev);
	}
	else
	{
		curr->size &= -0x2;
		if (curr->nx && !(curr->nx->size & 3))
		{
			if (!prev)
				curr->size = PACK(curr->size + curr->nx->size + HSIZE, 0x2);
			else
				curr->size += curr->nx->size + HSIZE;
			curr->nx = curr->nx->nx;
		}
	}
	return (curr);
}

void	free(void *ptr)
{
	size_t		grp;
	t_block		*find;
	t_block		*prev;
	t_block		*last;

	if (ptr == NULL)
		return ;
	pthread_mutex_lock(&g_mut);
	find = ft_find(ptr, &prev, &grp, &last);
	find->size &= -0x2;
	if (grp < large)
		find = coalescence(find, prev);
	if ((grp == tiny && (find->size & ~2) == (size_t)ALIGN_PAGE(TINY))
		|| (grp == small && (find->size & ~2) == (size_t )ALIGN_PAGE(SMALL)))
	{
		if (g_mem[grp] == find)
			g_mem[grp] = find->nx;
		else if (prev)
			prev->nx = find->nx;
		if (last)
			last->nx = last->nx->nx;
		munmap((char *)find, find->size);
	}
	pthread_mutex_unlock(&g_mut);
}

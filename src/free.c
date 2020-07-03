/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/01 22:46:36 by dbaffier          #+#    #+#             */
/*   Updated: 2020/07/01 22:46:38 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static t_block	*ft_find(void *ptr, t_block **prev, size_t *grp)
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
			if ((save->size & 1) && (unsigned char *)(save)
			+ sizeof(t_block) == (unsigned char *)ptr)
				return (save);
			if (prev)
				*prev = save;
			save = save->nx;
		}
	}
	pthread_mutex_unlock(&g_mutex);
	return (NULL);
}

static t_block	*coalescence(t_block *curr, t_block *prev, size_t f)
{
	if (prev && !(prev->size & 1) && !(curr->size & 2))
	{
		if (curr->nx && !(curr->nx->size & 3))
		{
			prev->size = f ? ((prev->size & ~3) + curr->nx->size + sizeof(B))
			| 2 : ((prev->size & ~0x3) + curr->nx->size + sizeof(B)) | 2;
			prev->nx = curr->nx->nx;
		}
		else
			prev->nx = curr->nx;
		prev->size = f ? ((prev->size & ~3) + (curr->size & ~3) + sizeof(B)) | 2
		: (prev->size & ~0x3) + (curr->size & ~0x3) + sizeof(B);
		return (prev);
	}
	else
	{
		if (curr->nx && !(curr->nx->size & 3))
		{
			curr->size = curr->size & 2 ?
			((curr->size & ~0x3) + sizeof(t_block) + curr->nx->size) | 0x2
			: (curr->size & ~0x3) + curr->nx->size + sizeof(B);
			curr->nx = curr->nx->nx;
		}
	}
	return (curr);
}

static void		adapt(size_t grp, t_block *prev)
{
	t_block		*list;

	list = g_mem[grp];
	while (list)
	{
		if (list->nx && list->nx == prev)
			list->nx = prev->nx;
		list = list->nx;
	}
}

void			free(void *ptr)
{
	size_t		grp;
	t_block		*find;
	t_block		*prev;

	if (ptr == NULL)
		return ;
	pthread_mutex_lock(&g_mutex);
	if (!(find = ft_find(ptr, &prev, &grp)))
		return ;
	find->size &= ~1;
	if (grp < large)
		find = coalescence(find, prev, prev && prev->size & 2);
	if ((grp == tiny && find && (find->size & ~0x3) ==
	(size_t)getpagesize() * 4) || (grp == small && find &&
		(find->size & ~0x3) == (size_t)getpagesize() * 32) || grp == large)
	{
		if (g_mem[grp] == find)
			g_mem[grp] = find->nx;
		else if (prev && prev->nx != find->nx)
			prev->nx = find->nx;
		else
			adapt(grp, prev);
		munmap((unsigned char *)find, find->size & ~0x3);
	}
	pthread_mutex_unlock(&g_mutex);
}

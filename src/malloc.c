/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/01 22:46:43 by dbaffier          #+#    #+#             */
/*   Updated: 2020/07/01 22:49:31 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

pthread_mutex_t g_mut;

void	*find_fit(t_block **last, size_t sf, size_t size)
{
	t_block	*list;
	t_block	*new;

	list = g_mem[sf];
	while (list)
	{
		if (!(list->size & 0x1) && (list->size & ~0x3) >= size)
		{
			if ((list->size & ~0x3) - size > HSIZE)
			{
				new = (t_block *)(ADDR(list) + size);
				ft_bzero(new, sizeof(t_block *));
				new->size = (list->size & ~3) - size - HSIZE;
				new->nx = list->nx;
				list->size = (list->size & 2) ? PACK(size, 0x3)
				: PACK(size, 0x1);
				list->nx = new;
				return (ADDR(list));
			}
		}
		*last = list;
		list = list->nx;
	}
	return (NULL);
}

void	*block_alloc(t_block *last, size_t sf, size_t size)
{
	t_block		*new;
	t_block		*next;
	size_t		pagesize;

	pagesize = sf == 0 ? TINY_PAGE : SMALL_PAGE;
	if (sf == 2)
		pagesize = ALIGN(size + HSIZE);
	new = (t_block *)mmap(0, pagesize, MAPPING);
	ft_bzero((unsigned char *)new, pagesize);
	new->nx = NULL;
	new->size = PACK(size, 0x3);
	if (last)
		last->nx = new;
	else
		g_mem[sf] = new;
	if (sf < large)
	{
		next = (t_block *)(ADDR(new) + size);
		new->nx = next;
		next->size = pagesize - (size + HSIZE);
		next->nx = NULL;
	}
	return (ADDR(new));
}

void	*malloc(size_t size)
{
	size_t		sf;
	size_t		newsize;
	t_block		*b;
	t_block		*last;

	if (size == 0)
		return (NULL);
	pthread_mutex_lock(&g_mut);
	newsize = ALIGN(size);
	sf = large;
	if (size <= TINY)
		sf = tiny;
	else if (size <= SMALL)
		sf = small;
	last = g_mem[sf];
	b = find_fit(&last, sf, newsize);
	if (b == NULL)
		b = block_alloc(last, sf, newsize);
	pthread_mutex_unlock(&g_mut);
	return ((void *)b);
}

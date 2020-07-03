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

void	*find_fit(t_block **last, size_t sf, size_t size)
{
	t_block	*list;
	t_block	*new;

	list = g_mem[sf];
	while (list)
	{
		if (!(list->size & 0x1) && (list->size & ~0x3) >= size)
		{
			if ((list->size & ~0x3) - size > sizeof(t_block))
			{
				new = (t_block *)((unsigned char *)
				(list) + sizeof(t_block) + size);
				ft_bzero(new, sizeof(t_block *));
				new->size = (list->size & ~3) - size - sizeof(t_block);
				new->nx = list->nx;
				list->size = (list->size & 2) ? size | 0x3
				: size | 0x1;
				list->nx = new;
				return ((unsigned char *)(list) + sizeof(t_block));
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

	pagesize = sf == 0 ? getpagesize() * 4 : getpagesize() * 32;
	if (sf == 2)
		pagesize = (((size + sizeof(t_block)) +
		(ALIGNMENT - 1)) & ~(ALIGNMENT - 1));
	new = (t_block *)mmap(0, pagesize, PROT_READ | PROT_WRITE,
	MAP_ANON | MAP_PRIVATE, -1, 0);
	ft_bzero((unsigned char *)new, pagesize);
	new->nx = NULL;
	new->size = size | 0x3;
	if (last)
		last->nx = new;
	else
		g_mem[sf] = new;
	if (sf < large)
	{
		next = (t_block *)((unsigned char *)(new) + sizeof(t_block) + size);
		new->nx = next;
		next->size = pagesize - (size + sizeof(t_block));
		next->nx = NULL;
	}
	return ((unsigned char *)(new) + sizeof(t_block));
}

void	*malloc(size_t size)
{
	size_t		sf;
	size_t		newsize;
	t_block		*b;
	t_block		*last;

	if (size == 0)
		return (NULL);
	pthread_mutex_lock(&g_mutex);
	newsize = ((size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1));
	sf = large;
	if (size <= TINY)
		sf = tiny;
	else if (size <= SMALL)
		sf = small;
	last = g_mem[sf];
	b = find_fit(&last, sf, newsize);
	if (b == NULL)
		b = block_alloc(last, sf, newsize);
	pthread_mutex_unlock(&g_mutex);
	return ((void *)b);
}

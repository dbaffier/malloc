/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/01 22:46:26 by dbaffier          #+#    #+#             */
/*   Updated: 2020/07/01 22:53:07 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static t_block	*ft_find(void *ptr)
{
	t_block *save;
	size_t	grp;

	grp = tiny;
	while (grp <= large)
	{
		save = g_mem[grp];
		while (save)
		{
			if ((save->size & 1) && (unsigned char *)(save)
			+ sizeof(t_block) == (unsigned char *)ptr)
				return (save);
			save = save->nx;
		}
		grp++;
	}
	return (NULL);
}

static void		*expand(t_block *find, size_t size)
{
	t_block		*nx;
	size_t		old;
	size_t		f;

	f = find->size & 2;
	find->size &= ~3;
	if (size < find->size + find->nx->size)
	{
		old = find->nx->size & ~3;
		nx = (t_block *)((unsigned char *)(find) + sizeof(t_block) + size);
		ft_bzero(nx, sizeof(t_block *));
		nx->nx = find->nx->nx;
		find->nx = nx;
		nx->size = ((find->size + old) - size);
		find->size = f ? size | 0x3 : size | 0x1;
	}
	else
	{
		find->size = f ? (find->size + find->nx->size) | 0x3
			: (find->size + find->nx->size) | 0x1;
		find->nx = find->nx->nx;
	}
	return ((void *)(unsigned char *)(find) + sizeof(t_block));
}

void			*realloc(void *ptr, size_t size)
{
	t_block		*find;

	if (ptr == NULL)
		return (malloc(size));
	if (!size)
		free(ptr);
	if (!size || !(find = ft_find(ptr)))
		return (ptr);
	size = ((size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1));
	if (size <= (find->size & ~3))
		return (ptr);
	if (find->nx && !(find->nx->size & 3)
			&& size < (find->size & ~3) + (find->nx->size & ~3)
			+ sizeof(t_block))
		ptr = expand(find, size);
	else
	{
		ptr = malloc(size);
		ft_memcpy(ptr, (unsigned char *)(find) +
		sizeof(t_block), (find->size & ~3));
		free((unsigned char *)(find) + sizeof(t_block));
	}
	return (ptr);
}

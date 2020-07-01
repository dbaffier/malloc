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

pthread_mutex_t	g_mut;

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
			if ((save->size & 1) && ADDR(save) == (unsigned char *)ptr)
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
		nx = (t_block *)(ADDR(find) + size);
		ft_bzero(nx, sizeof(t_block));
		nx->nx = find->nx->nx;
		find->nx = nx;
		nx->size = ((find->size + old) - size);
		find->size = f ? PACK(size, 0x3) : PACK(size, 0x1);
	}
	else
	{
		find->nx = find->nx->nx;
		find->size = f ? PACK(find->size + find->nx->size, 0x3)
			: PACK(find->size + find->nx->size, 0x1);
	}
	return ((void *)ADDR(find));
}

static void		*unlock_ret(void *ptr)
{
	pthread_mutex_unlock(&g_mut);
	return (ptr);
}

void			*realloc(void *ptr, size_t size)
{
	t_block		*find;

	if (ptr == NULL)
		return (malloc(size));
	if (!size)
		free(ptr);
	pthread_mutex_lock(&g_mut);
	if (!size || !(find = ft_find(ptr)))
		return (unlock_ret(ptr));
	size = ALIGN(size);
	if (size <= (find->size & ~3))
		return (unlock_ret(ptr));
	if (find->nx && !(find->nx->size & 3)
			&& size < (find->size & ~3) + (find->nx->size & ~3) + HSIZE)
		ptr = expand(find, size);
	else
	{
		ptr = malloc(size);
		ft_memcpy(ptr, ADDR(find), (find->size & ~3));
		free(ADDR(find));
	}
	return (unlock_ret(ptr));
}

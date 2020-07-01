/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/01 22:46:52 by dbaffier          #+#    #+#             */
/*   Updated: 2020/07/01 22:46:54 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

pthread_mutex_t g_lock;

void	print_addr(void *ptr)
{
	uintptr_t	n;
	char		str[10];
	char		*hex;
	int			i;

	write(1, "0x", 2);
	hex = "0123456789abcdef";
	n = (uintptr_t)ptr;
	str[9] = '\0';
	i = 9;
	while (i-- >= 0)
	{
		str[i] = hex[n & 15];
		n >>= 4;
	}
	write(1, str, 9);
}

void	header_zone(void *p, size_t i)
{
	write(1, MWHITE, ft_strlen(MWHITE));
	if (i == 0)
		write(1, "\t\tTINY -- ", 10);
	else if (i == 1)
		write(1, "\t\tSMALL -- ", 11);
	else if (i == 2)
		write(1, "\t\tLARGE -- ", 11);
	print_addr(p);
	write(1, "\n", 1);
}

void	print_chunk(t_block *curr, int *total)
{
	if (curr->size & 1)
	{
		write(1, MGREEN, ft_strlen(MGREEN));
		write(1, "\t", 1);
		print_addr((unsigned char *)curr + HSIZE);
		write(1, " - ", 3);
		print_addr((unsigned char *)curr + (HSIZE + (curr->size & ~0x3)));
		write(1, " => ", 4);
		ft_putnbr(curr->size & ~0x3);
		*total += (curr->size & ~0x3);
		write(1, " bytes allocated", 16);
	}
	else
	{
		write(1, MBLUE, ft_strlen(MBLUE));
		write(1, "\t", 1);
		print_addr((unsigned char *)curr + HSIZE);
		write(1, " - ", 3);
		print_addr((unsigned char *)curr + (HSIZE + (curr->size & ~0x3)));
		write(1, " => ", 4);
		ft_putnbr(curr->size & ~0x3);
		write(1, " bytes available", 16);
	}
	write(1, MWHITE, ft_strlen(MWHITE));
	write(1, "\n", 1);
}

void	show_alloc_mem_ex(void)
{
	t_block		*bl;
	size_t		i;
	int			total;

	i = 0;
	pthread_mutex_lock(&g_lock);
	write(1, MWHITE, ft_strlen(MWHITE));
	while (i <= large)
	{
		bl = g_mem[i];
		while (bl)
		{
			if (bl->size & 2)
				header_zone(bl, i);
			print_chunk(bl, &total);
			if (bl == bl->nx)
				break ;
			bl = bl->nx;
		}
		i++;
	}
	write(1, "\t\tTotal : ", 10);
	ft_putnbr(total);
	write(1, "bytes\n", 6);
	pthread_mutex_unlock(&g_lock);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmonier <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 11:12:46 by mmonier           #+#    #+#             */
/*   Updated: 2018/11/23 12:07:24 by mmonier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *str)
{
	int		*addr;
	size_t	i;

	addr = (int *)str;
	i = 0;
	while (1)
	{
		if (!(*addr & 0xFF))
			return (i);
		if (!(*addr & 0xFF00))
			return (i + 1);
		if (!(*addr & 0xFF0000))
			return (i + 2);
		if (!(*addr & 0xFF000000))
			return (i + 3);
		addr++;
		i += 4;
	}
	return (i);
}

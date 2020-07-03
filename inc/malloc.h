/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 22:57:50 by dbaffier          #+#    #+#             */
/*   Updated: 2020/07/02 22:57:52 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>

# define MRED "\033[1;38;2;225;20;20m"
# define MORANGE "\033[1;38;2;255;120;10m"
# define MYELLO "\033[1;38;2;255;200;0m"
# define MGREEN "\033[1;38;2;0;255;101m"
# define MBLUE "\033[1;38;2;0;255;255m"
# define MLG "\033[1;38;2;167;244;66m"
# define MPURPLE "\033[1;38;2;255;75;255m"
# define MWHITE "\033[1;38;2;255;250;255m"

# define TINY 128
# define SMALL 1024

# define ALIGNMENT 16

# define USE 1
# define B t_block

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

void			*malloc(size_t size);
void			free(void *ptr);
void			*realloc(void *ptr, size_t size);
void			show_alloc_mem_ex(void);

extern t_block *g_mem[3];
extern pthread_mutex_t g_mutex;

#endif

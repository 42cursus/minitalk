/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 19:01:27 by abelov            #+#    #+#             */
/*   Updated: 2024/03/06 19:01:30 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# include <sysexits.h>
# include <signal.h>
# include <stddef.h>
# include <limits.h>
# include "libft.h"

typedef struct sigaction t_sigaction;

typedef struct s_global_lock
{
	volatile sig_atomic_t bit_received;
	volatile sig_atomic_t byte_received;
}				t_global_lock;

typedef struct context
{
	unsigned char 	c;
	int	i;
} t_context;

typedef struct sigaction t_sigaction;

#endif //MINITALK_H

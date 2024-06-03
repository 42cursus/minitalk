/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:49:03 by abelov            #+#    #+#             */
/*   Updated: 2024/02/22 18:49:05 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static volatile t_global_lock	g_lock = {0};

void	handle_signal(int signum, siginfo_t *info, void *ctx)
{
	((void)info, (void)ctx);
	if (signum == SIGUSR1)
		g_lock.bit_received = 1;
	if (signum == SIGUSR2)
		g_lock.byte_received = 1;
}

void	set_sig_handlers(void)
{
	t_sigaction	act;

	act.sa_flags = SA_SIGINFO | SA_RESTART;
	act.sa_sigaction = &handle_signal;
	sigemptyset(&act.sa_mask);
	if (sigaction(SIGUSR1, &act, NULL) != 0)
		exit(EXIT_FAILURE);
	if (sigaction(SIGUSR2, &act, NULL) != 0)
		exit(EXIT_FAILURE);
}

int	send_char(pid_t pid, char c)
{
	int				i;
	unsigned char	mask;
	int				sin_num;

	i = 0;
	mask = 0b10000000;
	g_lock.byte_received = 0;
	while (i++ < CHAR_BIT)
	{
		if (c & mask)
			sin_num = SIGUSR1;
		else
			sin_num = SIGUSR2;
		g_lock.bit_received = 0;
		if (kill(pid, sin_num) == -1)
			exit(EXIT_FAILURE);
		mask >>= 1;
		while (!g_lock.bit_received)
			;
	}
	while (!g_lock.byte_received)
		;
	return (0);
}

/**
 * If the SA_SIGINFO flag is set in the sa_flags field,
 * the sa_sigaction field specifies a signal-catching function.
 *
 * str = "a\t\tz привет мир 郑逸玲\n";
 */
int	main(int argc, char **argv)
{
	char	*str;
	pid_t	pid;

	(void)argc;
	set_sig_handlers();
	str = argv[2];
	pid = ft_atoi(argv[1]);
	while (*str)
		send_char(pid, *str++);
	send_char(pid, '\0');
	return (EX_OK);
}

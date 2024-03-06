/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:48:27 by abelov            #+#    #+#             */
/*   Updated: 2024/02/22 18:48:29 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	confirm_byte(t_context *cont, int sipid)
{
	char	c;

	c = (char)cont->c;
	if (c == '\0')
		c = '\n';
	write(STDOUT_FILENO, &c, 1);
	cont->c = 0;
	cont->i = 0;
	if (kill(sipid, SIGUSR2) == -1)
		exit(EXIT_FAILURE);
}

/**
 * For the purpose of name lookup, after the anonymous union definition,
 * the members of the anonymous union are considered to have been defined
 * in the scope in which the anonymous union is declared:
 *
 * info->_sifields._kill.si_pid => info->si_pid;
 */
void	sig_handler(int sig, siginfo_t *info, void *ctx)
{
	static t_context	cont = {0};
	int					sipid;

	sipid = info->si_pid;
	(void) ctx;
	if (CHAR_BIT > cont.i)
	{
		if (sig == SIGUSR1)
			cont.c = (cont.c << 1) | 0b00000001;
		else if (sig == SIGUSR2)
			cont.c <<= 1;
		cont.i++;
		if (kill(sipid, SIGUSR1) == -1)
			exit(EXIT_FAILURE);
		if (CHAR_BIT == cont.i)
			confirm_byte(&cont, sipid);
	}
	else
		confirm_byte(&cont, sipid);
}

int	main(void)
{
	t_sigaction	act;

	act.sa_flags = SA_SIGINFO | SA_RESTART;
	act.sa_sigaction = &sig_handler;
	sigemptyset(&act.sa_mask);
	if (sigaction(SIGUSR1, &act, NULL) != 0)
		exit(EXIT_FAILURE);
	if (sigaction(SIGUSR2, &act, NULL) != 0)
		exit(EXIT_FAILURE);
	ft_printf("server pid: %d\n", getpid());
	while (1)
		pause();
	return (EX_OK);
}

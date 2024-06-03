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

#include <minitalk.h>

void	confirm_byte(t_context *cont, int sipid)
{
	char	c;

	c = (char) cont->c;
	if (c == '\0')
		c = '\n';
	write(STDOUT_FILENO, &c, 1);
	cont->c = 0;
	cont->i = 0;
	if (kill(sipid, SIGUSR2) == -1)
		exit(EXIT_FAILURE);
}

/**
 * From man 2 sigaction:
 * When the SA_SIGINFO flag is specified in act.sa_flags, the signal
 * handler address is passed via the act.sa_sigaction field.  This
 * handler takes three arguments:
 *
 * 		sig    The number of the signal that caused invocation of the
 * 		       handler.
 *
 * 		info   A pointer to a siginfo_t, which is a structure containing
 * 		       further information about the signal, as described below.
 *
 * 		ucontext
 * 		       This is a pointer to a ucontext_t structure, cast to
 * 		       void *.  The structure pointed to by this field contains
 * 		       signal context information that was saved on the user-
 * 		       space stack by the kernel.
 *
 *  typedef struct {
 * 		int si_signo;     // Signal number
 * 		int si_errno;     // An errno value
 * 		int si_code;      // Signal code
 * 		int si_trapno;    // Trap number that caused
 * 		                                        hardware-generated signal
 * 		                                        (unused on most architectures)
 * 		pid_t si_pid;     // Sending process ID
 * 		uid_t si_uid;     // Real user ID of sending process
 * 		int si_status;    // Exit value or signal
 * 		clock_t si_utime; // User time consumed
 * 		clock_t si_stime; // System time consumed
 * 		union sigval si_value; // Signal value
 * 		int si_int;       // POSIX.1b signal
 * 		void *si_ptr;     // POSIX.1b signal
 * 		int si_overrun;   // Timer overrun count;
 * 		int si_timerid;   // Timer ID; POSIX.1b timers
 * 		void *si_addr;    // Memory location which caused fault
 * 		long si_band;     // Band event (was int in glibc 2.3.2 and earlier)
 * 		int si_fd;        // File descriptor
 * 		short si_addr_lsb; // Least significant bit of address
 * 		void *si_lower;   // Lower bound when address violation occurred
 * 		void *si_upper;   // Upper bound when address violation occurred
 * 		int si_pkey;      // Protection key on PTE that caused
 * 		void *si_call_addr; // Address of system call instruction
 * 		int si_syscall;   // Number of attempted system call
 * 		unsigned int si_arch;  // Architecture of attempted system call
 *  } siginfo_t;
 *
 * For the purpose of name lookup, after the anonymous union definition,
 * the members of the anonymous union are considered to have been defined
 * in the scope in which the anonymous union is declared:
 *
 * info->_sifields._kill.si_pid => info->si_pid;
 */
void	sig_handler(int sig, siginfo_t *info, void *ctx)
{
	int					sipid;
	static t_context	cont = {0};

	sipid = info->si_pid;
	(void)ctx;
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
	ft_printf("server pid is: %d\n", getpid());
	while (1)
		pause();
	return (EX_OK);
}

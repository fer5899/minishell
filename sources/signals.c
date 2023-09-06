/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgomez-d <fgomez-d@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:43:17 by alvgomez          #+#    #+#             */
/*   Updated: 2023/09/06 15:38:39 by fgomez-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int	g_prog_state;

/*	fix visual CTRL+ with termios.
 *	&= ~	: it's disable the attribute.
 *	ECHOCTL	: reference to CTRL char.
 *	TCSANOW	: change the attribute immediately.
 *	termios C_LFLAG	: refers to local modes.
 */

void	sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		if (g_prog_state == basic_prompt)
		{
			ft_printf("\n");
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
		}
		else if (g_prog_state == process)
		{
			ft_printf("\n");
			rl_replace_line("", 0);
			rl_on_new_line();
		}
		else if (g_prog_state == in_heredoc_process)
		{
			ft_printf("\n");
			rl_replace_line("", 0);
			exit(1);
		}
	}
}

void	sigquit_handler(int sig)
{
	if (sig == SIGQUIT)
	{
		if (g_prog_state == process)
		{
			ft_printf("^\\Quit: 3\n");
			rl_replace_line("", 0);
			rl_on_new_line();
			g_prog_state = quit_process;
		}
	}
}

void	handle_signals(void)
{
	struct termios	tty_data;

	tcgetattr(STDIN_FILENO, &tty_data);
	tty_data.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &tty_data);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
}

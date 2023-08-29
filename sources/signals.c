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

void	handle_signals(void)
{
	struct termios	tty_data;

	tcgetattr(STDIN_FILENO, &tty_data);
	tty_data.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &tty_data);
	signal(SIGINT, sigint_handler);
}


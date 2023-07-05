#include "../minishell.h"

void	fatal_error(void)
{
	ft_printf_fd("FATAL ERROR\n", 2);
	exit(1);
}

void	redirection_error(char *str)
{
	ft_printf_fd("minishell: ", 2);
	perror(str);
	exit(1);
}

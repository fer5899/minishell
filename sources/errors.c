#include "../minishell.h"

void	fatal_error(void)
{
	// free everything
	ft_printf_fd("FATAL ERROR\n", 2);
	exit(1);
}

void	file_error(char *str, int code)
{
	// free everything
	ft_printf_fd("minishell: ", 2);
	perror(str);
	exit(code);
}

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

void	export_unset_error(t_master *data, char *arg, int is_unset)
{
	char	*error;

	if (is_unset)
		error = "unset";
	else
		error = "export";
	ft_printf_fd("minishell: %s: `%s\': not a valid identifier",
		2, error, arg);
	data->exit_code = 1;
}

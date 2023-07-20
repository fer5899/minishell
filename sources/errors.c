#include "../minishell.h"

void	fatal_error(void)
{
	// free everything
	ft_printf_fd("FATAL ERROR\n", 2);
	exit(1);
}

void	file_error(char *path, char *err_str, int code)
{
	// free everything
	ft_printf_fd("minishell: %s: %s\n", 2, path, err_str);
	exit(code);
}

void	export_unset_error(t_master *data, char *arg, int is_unset)
{
	char	*error;

	if (is_unset)
		error = "unset";
	else
		error = "export";
	ft_printf_fd("minishell: %s: `%s\': not a valid identifier\n",
		2, error, arg);
	data->exit_code = 1;
}

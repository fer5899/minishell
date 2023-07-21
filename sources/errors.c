#include "../minishell.h"

void	fatal_error(t_master *d)
{
	ft_printf_fd("FATAL ERROR\n", 2);
	free_master_exit(d, 1);
}

void	file_error(t_master *d, char *path, char *err_str, int code)
{
	ft_printf_fd("minishell: %s: %s\n", 2, path, err_str);
	free_master_exit(d, code);
}

void	export_unset_error(t_master *d, char *arg, int is_unset)
{
	char	*error;

	if (is_unset)
		error = "unset";
	else
		error = "export";
	ft_printf_fd("minishell: %s: `%s\': not a valid identifier\n",
		2, error, arg);
	d->exit_code = 1;
}

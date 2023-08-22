#include "../minishell.h"

void	pwd(t_master *d)
{
	char	*pwd_str;

	pwd_str = get_pwd(d);
	ft_printf("%s\n", pwd_str);
	free(pwd_str);
	free_master_exit(d, 0);
}

void	echo(t_master *d, char **args)
{
	int	nl;

	if (d->nargs == 1)
	{
		ft_printf("\n");
		free_master_exit(d, 0);
	}
	nl = 1;
	args++;
	get_echo_arg(&args, &nl);
	while (*args != NULL)
	{
		ft_printf("%s", *args);
		if (*(args + 1) != NULL)
			ft_printf(" ");
		args++;
	}
	if (nl)
		ft_printf("\n");
	free_master_exit(d, 0);
}

void	cd(t_master *d)
{
	char	*old_pwd;

	if (d->nargs < 2)
	{
		d->exit_code = 0;
		return ;
	}
	old_pwd = get_pwd(d);
	if (chdir(d->args[1]) == -1)
	{
		ft_printf_fd("minishell: cd: %s: No such file or directory",
			2, d->args[1]);
		d->exit_code = 1;
		free(old_pwd);
	}
	else
	{
		d->exit_code = 0;
		update_env(d, ft_strdup("OLDPWD"), old_pwd, 0);
		update_env(d, ft_strdup("PWD"), get_pwd(d), 0);
	}
}

void	exit_builtin(t_master *d)
{
	char	*trimmed;

	if (d->nargs > 1)
	{
		trimmed = ft_strtrim(d->args[1], " \t\v");
		if (!is_long(trimmed))
		{
			ft_printf_fd("minishell: exit: %s: numeric argument required\n",
				2, d->args[1]);
			d->exit_code = 255;
			free_master_exit(d, d->exit_code);
		}
		else if (d->nargs > 2)
		{
			ft_printf_fd("minishell: exit: too many arguments\n", 2);
			d->exit_code = 1;
		}
	}
	else if (d->nargs == 1)
		free_master_exit(d, d->exit_code);
}



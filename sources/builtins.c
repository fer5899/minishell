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
	if (ft_strlen(*args) == 2 && !ft_strncmp(*args, "-n", 2))
	{
		nl = 0;
		args++;
	}
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
	int	prev_exit;

	prev_exit = d->exit_code;
	if (d->nargs > 2)
	{
		ft_printf_fd("minishell: exit: too many arguments", 2);
		free_master_exit(d, 1);
	}
	if (d->nargs == 2)
	{
		if (is_long(d->args[1]))
			exit(get_exit_status(d->args[1]));
		ft_printf_fd("minishell: exit: %s: numeric argument required",
			2, d->args[1]);
		free_master_exit(d, 255);
	}
	else if (d->nargs == 1)
		free_master_exit(d, prev_exit);
}



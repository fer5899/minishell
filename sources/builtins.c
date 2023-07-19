#include "../minishell.h"

void	pwd(void)
{
	char	*pwd_str;

	pwd_str = get_pwd();
	ft_printf("%s\n", pwd_str);
	free(pwd_str);
	exit(0);
}

void	echo(t_master *data, char **args)
{
	int	nl;

	if (data->nargs == 1)
	{
		ft_printf("\n");
		exit(0);
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
	exit(0);
}

void	cd(t_master *data, char **args)
{
	char	*old_pwd;

	if (data->nargs < 2)
	{
		data->exit_code = 0;
		return ;
	}
	old_pwd = get_pwd();
	if (chdir(args[1]) == -1)
	{
		ft_printf_fd("minishell: cd: %s: No such file or directory",
			2, args[1]);
		data->exit_code = 1;
	}
	else
	{
		data->exit_code = 0;
		update_env(data, ft_strdup("OLDPWD"), old_pwd, 0);
		update_env(data, ft_strdup("PWD"), get_pwd(), 0);
	}
}

void	exit_builtin(t_master *data, char **args)
{
	int	prev_exit;

	prev_exit = data->exit_code;
	if (data->nargs > 2)
	{
		ft_printf_fd("minishell: exit: too many arguments", 2);
		exit(1);
	}
	if (data->nargs == 2)
	{
		if (is_long(args[1]))
			exit(get_exit_status(args[1]));
		ft_printf_fd("minishell: exit: %s: numeric argument required",
			2, args[1]);
		exit(255);
	}
	else if (data->nargs == 1)
		exit(prev_exit);
}



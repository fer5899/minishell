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

void	env(t_master *data)
{
	ft_lstiter(data->env_lst, print_env);
	exit(0);
}

void	cd(t_master *data, char **args)
{
	if (data->nargs < 2)
	{
		data->exit_code = 0;
		return ;
	}
	if (chdir(args[1]) == -1)
	{
		ft_printf_fd("minishell: cd: %s: No such file or directory",
			2, args[1]);
		data->exit_code = 1;
	}
	else
		data->exit_code = 0;
	// actualizar PWD y OLD_PWD
}

void	exit_builtin(t_master *data, char **args)
{
	if (data->nargs > 2)
	{
		ft_printf_fd("minishell: exit: too many arguments", 2);
		exit(1);
	}
	if (data->nargs == 2)
	{
		// coger is_digit y is_int_size de philosophers y hacer atoi
	}

}

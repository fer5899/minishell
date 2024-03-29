/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgomez-d <fgomez-d@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:40:31 by alvgomez          #+#    #+#             */
/*   Updated: 2023/09/06 15:46:24 by fgomez-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pwd(void)
{
	char	*pwd_str;

	pwd_str = get_pwd();
	ft_printf("%s\n", pwd_str);
	free(pwd_str);
	exit(0);
}

void	echo(t_master *d, char **args)
{
	int	nl;

	if (d->nargs == 1)
	{
		ft_printf("\n");
		exit(0);
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
	free_pargs(d);
	exit(0);
}

void	cd(t_master *d)
{
	char	*old_pwd;
	char	*home;

	old_pwd = get_pwd();
	if (d->nargs < 2)
	{
		home = get_env_variable("HOME", d);
		update_env(d, ft_strdup("OLDPWD"), old_pwd, 0);
		update_env(d, ft_strdup("PWD"), ft_strdup(home), 0);
		chdir(home);
		d->exit_code = 0;
	}
	else if (chdir(d->args[1]) == -1 && !str_equal(d->args[1], ""))
	{
		check_wrong_chdir(d->args[1]);
		d->exit_code = 1;
		free(old_pwd);
	}
	else
	{
		d->exit_code = 0;
		update_env(d, ft_strdup("OLDPWD"), old_pwd, 0);
		update_env(d, ft_strdup("PWD"), get_pwd(), 0);
	}
	free_pargs(d);
}

void	exit_builtin(t_master *d)
{
	char	*trimmed;

	ft_printf_fd("exit\n", 2);
	if (d->nargs > 1)
	{
		trimmed = ft_strtrim(d->args[1], " \t\v");
		if (!is_long(trimmed))
			exit_non_numeric_argument(d, trimmed);
		else if (d->nargs > 2)
		{
			ft_printf_fd("minishell: exit: too many arguments\n", 2);
			free(trimmed);
			free_pargs(d);
			d->exit_code = 1;
		}
		else
		{
			d->exit_code = get_exit_status(trimmed);
			free(trimmed);
			free_master_and_exit(d, d->exit_code);
		}
	}
	else if (d->nargs == 1)
		free_master_and_exit(d, d->exit_code);
}

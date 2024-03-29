/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvgomez <alvgomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:40:22 by alvgomez          #+#    #+#             */
/*   Updated: 2023/09/05 11:40:24 by alvgomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_wrong_chdir(char *path)
{
	struct stat	path_info;

	if (ft_strlen(path) > 255)
	{
		ft_printf_fd("minishell: cd: %s: File name too long\n",
			2, path);
		return ;
	}
	if (access(path, F_OK) == -1)
		ft_printf_fd("minishell: cd: %s: No such file or directory\n",
			2, path);
	else if (stat(path, &path_info) == 0)
	{
		if (!S_ISDIR(path_info.st_mode))
			ft_printf_fd("minishell: cd: %s: Not a directory\n",
				2, path);
	}
	if (access(path, R_OK | X_OK) == -1)
		ft_printf_fd("minishell: cd: %s: Permission denied\n",
			2, path);
	else
		ft_printf_fd("minishell: cd: %s: No such file or directory\n",
			2, path);
}

int	is_export_append(char **key)
{
	int		key_len;
	char	*new_key;

	key_len = ft_strlen(*key);
	if ((*key)[key_len - 1] == '+')
	{
		new_key = ft_substr(*key, 0, key_len - 1);
		free(*key);
		*key = new_key;
		return (1);
	}
	return (0);
}

void	upd_or_app(t_list *lst, char *key, char *value, int is_append)
{
	char	*joined_value;

	free(key);
	if (!is_append)
	{
		free(((t_env *)(lst->content))->value);
		((t_env *)(lst->content))->value = value;
	}
	else
	{
		joined_value = ft_strjoin(((t_env *)(lst->content))->value, value);
		free(((t_env *)(lst->content))->value);
		free(value);
		((t_env *)(lst->content))->value = joined_value;
	}
}

void	exit_non_numeric_argument(t_master *d, char *trimmed)
{
	ft_printf_fd("minishell: exit: %s: numeric argument required\n",
		2, d->args[1]);
	d->exit_code = 255;
	free(trimmed);
	free_master_and_exit(d, d->exit_code);
}

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

void	update_or_append(t_list *lst, char *key, char *value, int is_append)
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

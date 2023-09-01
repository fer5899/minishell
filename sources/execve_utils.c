#include "../minishell.h"

char	*get_env_str(t_list *lst)
{
	t_env	*dict_entry;
	char	*key_and_equal;
	char	*fullstr;

	dict_entry = (t_env *) lst->content;
	key_and_equal = ft_strjoin(dict_entry->key, "=");
	fullstr = ft_strjoin(key_and_equal, dict_entry->value);
	free(key_and_equal);
	return (fullstr);
}

char	**get_env_arr(t_master *d)
{
	t_list	*env;
	char	**env_arr;
	int		env_len;
	int		i;

	env = d->env_lst;
	env_len = ft_lstsize(env);
	env_arr = (char **) ft_calloc(env_len + 1, sizeof(char *));
	i = -1;
	while (++i < env_len)
	{
		env_arr[i] = get_env_str(env);
		env = env->next;
	}
	env_arr[i] = NULL;
	return (env_arr);
}

int	is_filepath_valid(char *filepath)
{
	struct stat	sb;

	if (str_equal(filepath, ""))
		return (1);
	if (filepath == NULL)
		exit(0);
	if (access(filepath, F_OK | X_OK) == 0)
	{
		if (stat(filepath, &sb) == 0 && S_ISREG(sb.st_mode))
			return (1);
		file_error(filepath, "is a directory", 126);
		return (0);
	}
	else if (errno == 13)
	{
		file_error(filepath, "Permission denied", 126);
		return (0);
	}
	else
		return (0);
}

char	**get_path_arr(t_master *d)
{
	t_list *env;

	env = d->env_lst;
	if (env == NULL)
		return (NULL);
	while (env != NULL)
	{
		if (ft_strlen(((t_env *)(env->content))->key) == 4
			&& ft_strncmp(((t_env *)(env->content))->key, "PATH", 4) == 0)
			return (ft_split(((t_env *)(env->content))->value, ':'));
		env = env->next;
	}
	return (NULL);
}

char	*get_prog_path(t_list *lst, char **path_arr)
{
	char	*add_slash;
	char	*cmd;
	char	*fullpath;

	cmd = get_pname(lst);
	if (is_filepath_valid(cmd) && (cmd[0] == '/' || cmd[0] == '.'))
		return (cmd);
	if (path_arr != NULL && !str_equal(cmd, ""))
	{
		while (*path_arr != NULL)
		{
			add_slash = ft_strjoin(*path_arr, "/");
			fullpath = ft_strjoin(add_slash, cmd);
			free(add_slash);
			if (is_filepath_valid(fullpath))
				return (fullpath);
			path_arr++;
			free(fullpath);
		}
	}
	if (ft_strchr(cmd, '/'))
		ft_printf_fd("minishell: %s: No such file or directory\n", 2, cmd);
	else
		ft_printf_fd("minishell: %s: command not found\n", 2, cmd);
	return (exit(127), NULL);
}

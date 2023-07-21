#include "../minishell.h"

void	env(t_master *data)
{
	ft_lstiter(data->env_lst, print_env);
	exit(0);
}

void	export_unset(t_master *data, char **args, int is_unset)
{
	char	*key;
	char	*value;

	data->exit_code = 0;
	if (data->nargs == 1 && !is_unset)
		print_sorted_env(data->env_lst);
	if (data->nargs > 1)
	{
		args++;
		while (*args != NULL)
		{
			key = get_key(*args);
			value = get_value(*args);
			if (is_valid_env_key(key))
				update_env(data, key, value, is_unset);
			else
			{
				export_unset_error(data, *args, is_unset);
				// free(key);
				// free(value);
				data->exit_code = 1;
			}
			args++;
		}
	}
}

char	*get_pwd(void)
{
	char	*buf;
	int		buf_size;	

	buf_size = 100;
	buf = (char *) ft_calloc(buf_size, sizeof(char));
	while (!getcwd(buf, buf_size))
	{
		free(buf);
		buf_size += 100;
		if (buf_size > 2147483500)
			exit(1);
		buf = (char *) ft_calloc(buf_size, sizeof(char));
	}
	return (buf);
}

void	update_env(t_master *data, char *key, char *value, int is_unset)
{
	t_list	*lst;


	lst = data->env_lst;
	while (lst != NULL)
	{
		if (str_equal(((t_env *)(lst->content))->key, key))
		{
			if (is_unset)
				ft_lstpop(&(data->env_lst), lst); // replace when finished testing // ft_lstdelone(ft_lstpop(&(data->env_lst), lst), free_env);
			else
			{
				// free(key);
				// free(((t_env *)(lst->content))->value);
				((t_env *)(lst->content))->value = value;
				return ;
			}
		}
		lst = lst->next;
	}
	if (!is_unset)
		ft_lstadd_front(&(data->env_lst), ft_lstnew(init_env_data(key, value)));
}

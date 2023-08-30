#include "../minishell.h"

void	env_builtin(t_master *d)
{
	ft_lstiter(d->env_lst, print_env);
	free_master_exit(d, 0);
}

void	export_unset(t_master *d, char **args, int is_unset)
{
	char	*key;
	char	*value;

	d->exit_code = 0;
	if (d->nargs == 1 && !is_unset)
		print_sorted_env(d->env_lst);
	if (d->nargs > 1)
	{
		args++;
		while (*args != NULL)
		{
			key = get_key(*args);
			value = get_value(*args);
			if (is_valid_env_key(key))
				update_env(d, key, value, is_unset);
			else
			{
				export_unset_error(d, *args, is_unset);
				free(key);
				free(value);
				d->exit_code = 1;
			}
			args++;
		}
	}
}

char	*get_pwd(t_master *d)
{
	char	*buf;
	int		buf_size;	

	buf_size = 100;
	buf = (char *) ft_calloc(buf_size, sizeof(char));
	if (!buf)
		fatal_error(d);
	while (!getcwd(buf, buf_size))
	{
		free(buf);
		buf_size += 100;
		if (buf_size > 2147483500)
			fatal_error(d);
		buf = (char *) ft_calloc(buf_size, sizeof(char));
		if (!buf)
			fatal_error(d);
	}
	return (buf);
}

void	update_env(t_master *d, char *key, char *value, int is_unset)
{
	t_list	*lst;
	int		is_append;

	if (!is_unset)
		is_append = is_export_append(&key);
	lst = d->env_lst;
	while (lst != NULL)
	{
		if (str_equal(((t_env *)(lst->content))->key, key))
		{
			if (is_unset)
				ft_lstdelone(ft_lstpop(&(d->env_lst), lst), free_env);
			else
			{
				update_or_append(lst, key, value, is_append);
				return ;
			}
		}
		lst = lst->next;
	}
	if (!is_unset)
		ft_lstadd_front(&(d->env_lst), ft_lstnew(init_env_data(key, value)));
}

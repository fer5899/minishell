#include "../minishell.h"

int	str_equal(char *str1, char *str2)
{
	if (ft_strlen(str1) != ft_strlen(str2))
		return (0);
	if (ft_strncmp(str1, str2, ft_strlen(str1)) == 0)
		return (1);
	return (0);
}

t_env	*init_env_data(char *key, char *value)
{
	t_env	*d;

	d = (t_env *) ft_calloc(1, sizeof(t_env));
	d->key = key;
	d->value = value;
	return (d);
}

char	*get_pname(t_master *d, t_list *lst)
{
	char	*prog_name;
	t_data	*content;

	prog_name = NULL;
	while (lst != NULL)
	{
		content = ((t_data *)lst->content);
		if (content->type == pipe_)
			break ;
		if (content->type == prog_name_)
		{
			prog_name = content->str;
			break ;
		}
		lst = lst->next;
	}
	if (prog_name == NULL)
		free_master_exit(d, 0);
	return (prog_name);
}

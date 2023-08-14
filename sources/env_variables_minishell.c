
#include "../minishell.h"

extern char	**environ;

t_list	*inicialize_env(void)
{
	t_list	*list;
	t_list	*new;
	t_env	*env;
	char	**split;

	list = NULL;
	while (*environ != NULL)
	{
		env = ft_calloc(1, sizeof(t_env));
		split = ft_split_env(*environ, '=');
		env->key = split[0];
		env->value = split[1];
		free(split);
		new = ft_lstnew(env);
		ft_lstadd_back(&list, new);
		environ++ ;
	}
	return (list);
}

t_master	*inicialize_struct(void)
{
	t_master	*master;

	master = ft_calloc(1, sizeof(t_master));
	master->env_lst = inicialize_env();
	return (master);
}

char	*get_env_variable(char *key, t_master *master)
{
	t_list	*current;
	t_env	*env;

	current = master->env_lst;
	while (current)
	{
		env = (t_env *)current->content;
		if (ft_strncmp(env->key, key, ft_strlen(key)) == 0)
		{
			return (env->value);
		}
		current = current->next;
	}
	return (NULL);
}

void	print_env_list(t_master *master)
{
	t_list	*current;
	t_env	*env;

	current = master->env_lst;
	while (current)
	{
		env = (t_env *)current->content;
		printf("%s=%s\n", env->key, env->value);
		current = current->next;
	}
}
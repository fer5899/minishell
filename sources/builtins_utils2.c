#include "../minishell.h"

void	*dup_env_data(void *env_data)
{
	t_env	*new_env;

	new_env = (t_env *) ft_calloc(1, sizeof(t_env));
	if (new_env == NULL)
		return (NULL);
	new_env->key = ft_strdup(((t_env *)(env_data))->key);
	if (new_env->key == NULL)
		return (free(new_env), NULL);
	if (((t_env *)(env_data))->value == NULL)
		new_env->value = NULL;
	else
	{
		new_env->value = ft_strdup(((t_env *)(env_data))->value);
		if (new_env->value == NULL)
			return (free(new_env->key), free(new_env), NULL);
	}
	return (new_env);
}

void	swap_next_nodes(t_list **first, t_list *start)
{
	t_list	*second;
	t_list	*third;

	second = (*first)->next;
	third = (*first)->next->next;
	second->next = third->next;
	third->next = second;
	(*first)->next = third;
	*first = start;
}

void	swap_nodes(t_list **first, t_list **start)
{
	t_list	*second;

	second = (*first)->next;
	(*first)->next = second->next;
	second->next = *first;
	*first = second;
	*start = second;
}

void	print_sorted_env(t_list *env)
{
	t_list	*start;
	t_list	*lst;
	char	*key1;
	char	*key2;

	start = ft_lstmap(env, dup_env_data, free_env);
	lst = start;
	while (lst->next != NULL)
	{
		key1 = ((t_env *)(lst->content))->key;
		key2 = ((t_env *)(lst->next->content))->key;
		if (lst->next && ft_strncmp(key1, ((t_env *)(lst->next->content))->key,
			ft_strlen(key1)) > 0)
			swap_nodes(&lst, &start);
		else if (lst->next && lst->next->next && ft_strncmp(key2,
				((t_env *)(lst->next->next->content))->key,
			ft_strlen(key2)) > 0)
			swap_next_nodes(&lst, start);
		else
			lst = lst->next;
	}
	ft_lstiter(start, print_export);
	ft_lstclear(&start, free_env);
	exit(0);
}

void	get_echo_arg(char ***args, int *nl)
{
	char	*curr_letter;

	while (**args != NULL)
	{
		if (ft_strncmp(**args, "-n", 2) != 0)
			break ;
		curr_letter = **args + 2;
		while (*curr_letter != '\0')
		{
			if (*curr_letter != 'n')
				break ;
			curr_letter++;
		}
		if (*curr_letter != 'n' && *curr_letter != '\0')
			break ;
		*nl = 0;
		*args += 1;
	}
}

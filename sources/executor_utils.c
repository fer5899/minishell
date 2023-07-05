#include "../minishell.h"

void	find_next_cmd(t_list **lst)
{
	while (*lst != NULL)
	{
		*lst = (*lst)->next;
		if (!*lst)
			break ;
		if (((t_data *)(*lst)->content)->type == pipe_)
		{
			*lst = (*lst)->next;
			break ;
		}
	}
}

void	init_pipes(t_master *data)
{
	t_list	*lst;
	int		i;

	i = 0;
	data->n_pipes = 0;
	lst = data->parsed_lst;
	while (lst != NULL)
	{
		if (((t_data *) lst->content)->type == pipe_)
			data->n_pipes++;
		lst = lst->next;
	}
	if (data->n_pipes > MAX_PIPES)
		fatal_error();
	while (i < data->n_pipes)
	{
		if (pipe(data->fds[i]) == -1)
			fatal_error();
		i++;
	}
}

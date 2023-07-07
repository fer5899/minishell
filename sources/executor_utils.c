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

void	count_pipes(t_master *data)
{
	t_list	*lst;

	data->n_pipes = 0;
	lst = data->parsed_lst;
	while (lst != NULL)
	{
		if (((t_data *) lst->content)->type == pipe_)
			data->n_pipes++;
		lst = lst->next;
	}
}

void	init_pipes(t_master *data)
{
	t_list	*lst;
	int		i;

	i = 0;
	if (data->n_pipes > MAX_PIPES)
		fatal_error();
	while (i < data->n_pipes)
	{
		if (pipe(data->fds[i]) == -1)
			fatal_error();
		i++;
	}
}

int	count_args(t_list *lst)
{
	int		nargs;

	nargs = 1;
	while (lst != NULL)
	{
		if (((t_data *) lst->content)->type == pipe_)
			break ;
		if (((t_data *) lst->content)->type == prog_arg_)
			nargs++;
		lst = lst->next;
	}
}

char	**get_prog_args(t_list *lst)
{
	char	**args;
	int		nargs;
	int		i;

	nargs = count_args(lst);
	args = (char **) ft_calloc(nargs + 1, sizeof(char *));
	i = 0;
	while (i < nargs && lst != NULL)
	{
		if (((t_data *) lst->content)->type == prog_arg_
			|| ((t_data *) lst->content)->type == prog_name_)
		{
			args[i] = ((t_data *) lst->content)->str;
			i++;
		}
		lst = lst->next;
	}
	args[i] = NULL;
	return (args);
}

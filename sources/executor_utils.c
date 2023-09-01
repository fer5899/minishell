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

void	count_pipes(t_master *d)
{
	t_list	*lst;

	d->n_pipes = 0;
	lst = d->parsed_lst;
	while (lst != NULL)
	{
		if (((t_data *) lst->content)->type == pipe_)
			d->n_pipes++;
		lst = lst->next;
	}
}

void	init_pipes(t_master *d)
{
	int		i;

	i = 0;
	if (d->n_pipes > MAX_PIPES)
		fatal_error(d);
	while (i < d->n_pipes)
	{
		if (pipe(d->fds[i]) == -1)
			fatal_error(d);
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
	return (nargs);
}

void	get_pargs(t_master *d, t_list *lst)
{
	char	**args;
	int		i;

	d->nargs = count_args(lst);
	args = (char **) ft_calloc(d->nargs + 1, sizeof(char *));
	if (!args)
		fatal_error(d);
	i = 0;
	while (i < d->nargs && lst != NULL)
	{
		if (((t_data *) lst->content)->type == prog_arg_
			|| ((t_data *) lst->content)->type == prog_name_)
		{
			args[i] = ft_strdup(((t_data *) lst->content)->str);
			i++;
		}
		lst = lst->next;
	}
	args[i] = NULL;
	d->args = args;
}

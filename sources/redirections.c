#include "../minishell.h"

void	set_redirection(int type, char *str, t_master *d)
{
	int	fd;

	if (type == in_redir_)
		in_redirection(str);
	else if (type == heredoc_ || type == heredoc_q_)
		heredoc(d);
	else if (type == out_red_ || type == out_red_app_)
	{
		if (!out_redirection_check(str))
			exit(1);
		if (type == out_red_)
			fd = open(str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = open(str, O_WRONLY | O_CREAT | O_APPEND, 0644);
		dup2(fd, 1);
		close(fd);
	}
}

void	set_pipe_redirection(t_master *d)
{
	if (d->n_pipes <= 0)
		return ;
	if (d->cmd_idx >= 0 && d->cmd_idx < d->n_pipes)
		dup2(d->fds[d->cmd_idx][wr], 1);
	if (d->cmd_idx > 0 && d->cmd_idx <= d->n_pipes)
		dup2(d->fds[d->cmd_idx - 1][rd], 0);
	close_fds(d);
}

void	set_all_redirections(t_master *d, t_list *lst)
{
	t_data	*content;

	set_pipe_redirection(d);
	while (lst != NULL)
	{
		content = ((t_data *)lst->content);
		if (content->type == pipe_)
			break ;
		else if (content->type > prog_arg_)
			set_redirection(content->type, content->str, d);
		lst = lst->next;
	}
}

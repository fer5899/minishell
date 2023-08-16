#include "../minishell.h"

void	set_redirection(int type, char *str, t_master *d)
{
	int	fd;

	if (type == in_redir_)
		in_redirection(str, d);
	else if (type == heredoc_ || type == heredoc_q_)
		heredoc(d);
	else if (type == out_red_ || type == out_red_app_)
	{
		if (access(str, F_OK) == 0)
			if (access(str, W_OK) == -1)
				file_error(d, str, "Permission denied", 1);
		if (type == out_red_)
			fd = open(str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = open(str, O_WRONLY | O_CREAT | O_APPEND, 0644);
		dup2(fd, 1);
		close(fd);
	}
}

void	get_all_input_heredoc(t_master *d)
{
	int		type;
	t_list	*lst;

	lst = d->parsed_lst;
	d->cmd_idx = 0;
	d->heredoc_idx = 0;
	while (lst != NULL)
	{
		type = ((t_data *) lst->content)->type;
		if (type == pipe_)
		{
			d->cmd_idx++;
			d->heredoc_idx = 0;
		}
		if (type == heredoc_ || type == heredoc_q_)
		{
			input_heredoc(((t_data *) lst->content)->str, type, d);
			d->heredoc_idx++;
		}
		lst = lst->next;
	}
	d->heredoc_idx = 0;
}

void	heredoc(t_master *d)
{
	int		tmp_fd;

	tmp_fd = open(get_tmp_path(d), O_RDONLY);
	unlink(get_tmp_path(d));
	dup2(tmp_fd, 0);
	close(tmp_fd);
	d->heredoc_idx++;
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

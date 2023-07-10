#include "../minishell.h"

void	set_redirection(int type, char *str, t_master *data)
{
	int	fd;

	if (type == in_redir_)
	{
		if (access(str, F_OK | R_OK) == -1)
			file_error(str, 1);
		fd = open(str, O_RDONLY);
		dup2(fd, 0);
		close(fd);
	}
	else if (type == heredoc_ || type == heredoc_q_)
		heredoc(data);
	else if (type == out_red_ || type == out_red_app_)
	{
		if (access(str, F_OK) == 0)
			if (access(str, W_OK) == -1)
				file_error(str, 1);
		if (type == out_red_)
			fd = open(str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = open(str, O_WRONLY | O_CREAT | O_APPEND, 0644);
		dup2(fd, 1);
		close(fd);
	}
}

void	get_all_input_heredoc(t_master *data)
{
	int		type;
	t_list	*lst;

	lst = data->parsed_lst;
	data->cmd_idx = 0;
	data->heredoc_idx = 0;
	while (lst != NULL)
	{
		type = ((t_data *) lst->content)->type;
		if (type == pipe_)
		{
			data->cmd_idx++;
			data->heredoc_idx = 0;
		}
		if (type == heredoc_ || type == heredoc_q_)
		{
			input_heredoc(((t_data *) lst->content)->str, type, data);
			data->heredoc_idx++;
		}
		lst = lst->next;
	}
	data->heredoc_idx = 0;
}

void	heredoc(t_master *data)
{
	int		tmp_fd;

	tmp_fd = open(get_tmp_path(data), O_RDONLY);
	unlink(get_tmp_path(data));
	dup2(tmp_fd, 0);
	close(tmp_fd);
	data->heredoc_idx++;
}

void	set_pipe_redirection(t_master *data)
{
	if (data->n_pipes <= 0)
		return ;
	if (data->cmd_idx >= 0 && data->cmd_idx < data->n_pipes)
		dup2(data->fds[data->cmd_idx][wr], 1);
	if (data->cmd_idx > 0 && data->cmd_idx <= data->n_pipes)
		dup2(data->fds[data->cmd_idx - 1][rd], 0);
	close_fds(data);
}

void	set_all_redirections(t_master *data, t_list *lst)
{
	t_data	*content;

	set_pipe_redirection(data);
	while (lst != NULL)
	{
		content = ((t_data *)lst->content);
		if (content->type == pipe_)
			break ;
		else if (content->type > prog_arg_)
			set_redirection(content->type, content->str, data);
		lst = lst->next;
	}
}

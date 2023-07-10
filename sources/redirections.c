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
		heredoc(str, type, data);
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

void	heredoc(char *delim, int type, t_master *data)
{
	int		tmp_fd;
	char	*input;

	tmp_fd = open(TEMP_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0600); // estudiar si es necesario un tmp file por cada heredoc
	write(1, "> ", 2);
	input = get_next_line(0);
	if (type == heredoc_)
		data = NULL; // borrar cuando exista expand_env_vars
	// 	input = expand_env_vars(input, data);
	while (ft_strlen(input) - 1 != ft_strlen(delim)
		|| ft_strncmp(input, delim, ft_strlen(input) - 1) != 0)
	{
		write(tmp_fd, input, ft_strlen(input));
		free(input);
		write(1, "> ", 2);
		input = get_next_line(0);
		// if (type == heredoc_)
		// 	input = expand_env_vars(input, data);
	}
	free(input);
	close(tmp_fd);
	tmp_fd = open(TEMP_PATH, O_RDONLY);
	unlink(TEMP_PATH);
	dup2(tmp_fd, 0);
	close(tmp_fd);
}

void	set_pipe_redirection(t_master *data, int cmd_idx)
{
	if (data->n_pipes <= 0)
		return ;
	if (cmd_idx >= 0 && cmd_idx < data->n_pipes)
		dup2(data->fds[cmd_idx][wr], 1);
	if (cmd_idx > 0 && cmd_idx <= data->n_pipes)
		dup2(data->fds[cmd_idx - 1][rd], 0);
	close_fds(data);
}

void	set_all_redirections(t_master *data, t_list *lst, int cmd_idx)
{
	t_data	*content;

	set_pipe_redirection(data, cmd_idx);
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

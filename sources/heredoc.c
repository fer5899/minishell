#include "../minishell.h"

extern int	g_prog_state;

void	get_all_input_heredoc(t_master *d)
{
	int		type;
	t_list	*lst;

	lst = d->parsed_lst;
	d->cmd_idx = 0;
	d->heredoc_idx = 0;
	while (lst != NULL && g_prog_state != exit_heredoc)
	{
		type = ((t_data *) lst->content)->type;
		if (type == pipe_)
		{
			d->cmd_idx++;
			d->heredoc_idx = 0;
		}
		if (type == heredoc_ || type == heredoc_q_)
		{
			if (!fork_heredoc(((t_data *) lst->content)->str, type, d))
				g_prog_state = exit_heredoc;
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

void	input_heredoc(char *delim, int type, t_master *d)
{
	int		tmp_fd;
	char	*input;

	tmp_fd = open_tmp_file(d);
	input = readline("> ");
	if (!input)
		return (close(tmp_fd), free(input));
	if (type == heredoc_)
		input = expand_env_variables_second_pass(input, d);
	input = add_nl(input);
	while (ft_strlen(input) - 1 != ft_strlen(delim)
		|| ft_strncmp(input, delim, ft_strlen(input) - 1) != 0)
	{
		write(tmp_fd, input, ft_strlen(input));
		free(input);
		input = readline("> ");
		if (!input)
			break ;
		if (type == heredoc_)
			input = expand_env_variables_second_pass(input, d);
		input = add_nl(input);
	}
	free(input);
	close(tmp_fd);
	exit(0);
}

int	fork_heredoc(char *delim, int type, t_master *d)
{
	pid_t	pid;
	int		status;

	status = 0;
	g_prog_state = in_heredoc;
	pid = fork();
	if (pid < 0)
		return (0);
	if (pid == 0)
	{
		g_prog_state = in_heredoc_process;
		input_heredoc(delim, type, d);
	}
	else
		waitpid(pid, &status, 0);
	if (!status)
		return (1);
	if (status)
	{
		exit_heredoc_after_signal(d);
		return (0);
	}
	return (0);
}

void	exit_heredoc_after_signal(t_master *d)
{
	int		tmp_fd;
	char	*tmp_path;

	tmp_path = get_tmp_path(d);
	tmp_fd = open(tmp_path, O_RDONLY);
	unlink(tmp_path);
	close(tmp_fd);
	free(tmp_path);
}

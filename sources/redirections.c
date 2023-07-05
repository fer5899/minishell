#include "../minishell.h"

void	redirection_error(char *str)
{
	ft_printf_fd("minishell: ", 2);
	perror(str);
	exit(1);
}

void	redirection(int type, char *str, t_master *data)
{
	int	fd;

	if (type == in_redir)
	{
		if (access(str, F_OK | R_OK) == -1)
			redirection_error(str);
		fd = open(str, O_RDONLY);
		dup2(fd, 0);
		close(fd);
	}
	else if (type == heredoc_ || type == heredoc_q)
		heredoc(str, type, data);
	else if (type == out_red || type == out_red_app)
	{
		if (access(str, F_OK) == 0)
			if (access(str, W_OK) == -1)
				redirection_error(str);
		if (type == out_red)
			fd = open(str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = open(str, O_WRONLY | O_CREAT, 0644);
		dup2(fd, 1);
		close(fd);
	}
}

void	heredoc(char *delim, int type, t_master *data)
{
	int		tmp_fd;
	char	*input;

	tmp_fd = open(TEMP_PATH, O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, 0600); // estudiar si es necesario un tmp file por cada fork
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

#include "../minishell.h"

void	close_fds(t_master *data)
{
	int	idx;

	idx = 0;
	while (idx < data->n_pipes)
	{
		if (data->fds[idx][rd] > 0)
			close(data->fds[idx][rd]);
		if (data->fds[idx][wr] > 0)
			close(data->fds[idx][wr]);
		idx++;
	}
}

char	*get_tmp_path(t_master *data)
{
	char	*and_cmd;
	char	*itoa;
	char	*and_underscore;
	char	*and_heredoc;
	char	*and_extension;

	itoa = ft_itoa(data->cmd_idx);
	and_cmd = ft_strjoin("tmp/", itoa);
	free(itoa);
	and_underscore = ft_strjoin(and_cmd, "_");
	free(and_cmd);
	itoa = ft_itoa(data->heredoc_idx);
	and_heredoc = ft_strjoin(and_underscore, itoa);
	free(itoa);
	free(and_underscore);
	and_extension = ft_strjoin(and_heredoc, ".tmp");
	free(and_heredoc);
	return (and_extension);
}

char	*add_nl(char *str)
{
	char	*str_nl;

	str_nl = ft_strjoin(str, "\n");
	free(str);
	return (str_nl);
}

void	input_heredoc(char *delim, int type, t_master *data)
{
	int		tmp_fd;
	char	*input;

	tmp_fd = open(get_tmp_path(data), O_WRONLY | O_CREAT | O_TRUNC, 0600); // estudiar si es necesario un tmp file por cada heredoc
	input = add_nl(readline("> "));
	type = 0; // BORRAR
	// if (type == heredoc_)
	// 	input = expand_env_vars(input, data);
	while (ft_strlen(input) - 1 != ft_strlen(delim)
		|| ft_strncmp(input, delim, ft_strlen(input) - 1) != 0)
	{
		write(tmp_fd, input, ft_strlen(input));
		free(input);
		input = add_nl(readline("> "));
		// if (type == heredoc_)
		// 	input = expand_env_vars(input, data);
	}
	free(input);
	close(tmp_fd);
}


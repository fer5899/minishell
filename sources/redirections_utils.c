#include "../minishell.h"

void	close_fds(t_master *d)
{
	int	idx;

	idx = 0;
	while (idx < d->n_pipes)
	{
		if (d->fds[idx][rd] > 0)
			close(d->fds[idx][rd]);
		if (d->fds[idx][wr] > 0)
			close(d->fds[idx][wr]);
		idx++;
	}
}

char	*get_tmp_path(t_master *d)
{
	char	*itoa;
	char	*and_underscore;
	char	*and_heredoc;
	char	*and_extension;

	itoa = ft_itoa(d->cmd_idx);
	and_underscore = ft_strjoin(itoa, "_");
	free(itoa);
	itoa = ft_itoa(d->heredoc_idx);
	and_heredoc = ft_strjoin(and_underscore, itoa);
	free(itoa);
	free(and_underscore);
	and_extension = ft_strjoin(and_heredoc, ".tmp");
	free(and_heredoc);
	and_heredoc = ft_strjoin("/tmp/", and_extension);
	free(and_extension);
	return (and_heredoc);
}

char	*add_nl(char *str)
{
	char	*str_nl;

	str_nl = ft_strjoin(str, "\n");
	free(str);
	return (str_nl);
}

void	in_redirection(char *str, t_master *d)
{
	int	fd;

	if (access(str, F_OK) == -1)
		file_error(d, str, "No such file or directory", 1);
	else if (access(str, R_OK) == -1)
		file_error(d, str, "Permission denied", 1);
	fd = open(str, O_RDONLY);
	dup2(fd, 0);
	close(fd);
}


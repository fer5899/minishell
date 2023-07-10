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

char	*gen_tmp_path(int cmd_idx, int heredoc_idx)
{
	char	*itoa;
	char	*and_underscore;
	char	*and_heredoc;
	char	*and_extension;

	itoa = ft_itoa(cmd_idx);
	and_underscore = ft_strjoin(itoa, "_");
	free(itoa);
	itoa = ft_itoa(heredoc_idx);
	and_heredoc = ft_strjoin(and_underscore, itoa);
	free(itoa);
	free(and_underscore);
	and_extension = ft_strjoin(and_heredoc, ".tmp");
	free(and_heredoc);
	return (and_extension);
}
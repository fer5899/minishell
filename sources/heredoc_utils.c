#include "../minishell.h"

int	open_tmp_file(t_master *d)
{
	char	*tmp_path;
	int		tmp_fd;

	tmp_path = get_tmp_path(d);
	tmp_fd = open(tmp_path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	free(tmp_path);
	return (tmp_fd);
}

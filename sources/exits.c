#include "../minishell.h"

void	catch_exit_code(t_master *data)
{
	int	status;

	status = 0;
	data->cmd_idx = -1;
	while (++data->cmd_idx < data->n_pipes + 1)
	{
		waitpid(data->pids[data->cmd_idx], &status, WUNTRACED | WCONTINUED);
		if (WIFEXITED(status))
			data->exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			data->exit_code = WTERMSIG(status);
		else if (WIFSTOPPED(status))
			data->exit_code = WSTOPSIG(status);
	}
}

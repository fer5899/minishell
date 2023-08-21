#include "../minishell.h"

int	catch_exit_code(t_master *d)
{
	int	status;

	status = 0;
	d->cmd_idx = -1;
	while (++d->cmd_idx < d->n_pipes + 1)
	{
		waitpid(d->pids[d->cmd_idx], &status, WUNTRACED | WCONTINUED);
		if (WIFEXITED(status))
			d->exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			d->exit_code = WTERMSIG(status);
		else if (WIFSTOPPED(status))
			d->exit_code = WSTOPSIG(status);
	}
    return (d->exit_code);
}

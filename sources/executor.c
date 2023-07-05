#include "../minishell.h"

void	executor(t_master *data)
{
	t_list	*lst;
	int		cmd_idx;

	lst = data->parsed_lst;
	cmd_idx = -1;
	init_pipes(data);
	while (lst != NULL)
	{
		cmd_idx++;
		data->pids[cmd_idx] = fork();
		if (data->pids[cmd_idx] == -1)
			fatal_error();
		if (data->pids[cmd_idx] == 0)
		{
			set_pipe_redirection(data, cmd_idx);
			set_all_redirections(data, lst);
			// check_builtin(lst, data);
			// execve(get_prog_path(lst), get_prog_args(lst), get_env(data));
			exit(0);
		}
		find_next_cmd(&lst);
	}
}


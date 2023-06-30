#include "../minishell.h"

void	fatal_error(void);

void	init_pipes(t_master *data)
{
	t_list	*lst;
	int		i;
	int		n_pipes;

	i = 0;
	n_pipes = 0;
	lst = data->parsed_lst;
	while (lst != NULL)
	{
		if (((t_data *) lst)->type == pipe_)
			n_pipes++;
		lst = lst->next;
	}
	if (n_pipes > MAX_PIPES)
		fatal_error();
	while (i < n_pipes)
	{
		if (pipe(data->fds[i]) == -1)
			fatal_error();
		i++;
	}
}

void	executor(t_master *data)
{
	t_list	*lst;
	int		cmd_idx;

	lst = data->parsed_lst;
	cmd_idx = -1;
	init_pipes(data);
	// Iterate the parsed list
	while (lst != NULL)
	{
		cmd_idx++;
		// Launch the process with list at the beginning of the command to execute
		data->pids[cmd_idx] = fork();
		if (data->pids[cmd_idx] == -1)
			fatal_error(); // if a fork fails stop minishell HOW?
		if (data->pids[cmd_idx] == 0)
		{
			// Apply redirections
			// Execute command

			// if (error->infile == 0 || cmd_idx - 1 != 0)
			// 	dup2(data->fds[cmd_idx - 1][rd], 0);
			// dup2(data->fds[cmd_idx][wr], 1);
			// close_fds(pipes, margs);
			// cmd_arr = strlst_to_strarr(\
			// 			cmd_parser(margs->argv[cmd_idx + 1], NULL, 0));
			// execve(get_cmd_path(cmd_arr[0], margs->env), cmd_arr, margs->env);
			// perror("pipex");
			// exit(1);
		}
		while (lst != NULL)
		{
			lst = lst->next;
			if (((t_data *) lst)->type == pipe)
			{
				lst = lst->next;
				break ;
			}
		}
	}
}


#include "../minishell.h"

void	find_next_cmd(t_list **lst)
{
	while (*lst != NULL)
	{
		*lst = (*lst)->next;
		if (!*lst)
			break ;
		if (((t_data *)(*lst)->content)->type == pipe_)
		{
			*lst = (*lst)->next;
			break ;
		}
	}
}

void	init_pipes(t_master *data)
{
	t_list	*lst;
	int		i;

	i = 0;
	data->n_pipes = 0;
	lst = data->parsed_lst;
	while (lst != NULL)
	{
		if (((t_data *) lst->content)->type == pipe_)
			data->n_pipes++;
		lst = lst->next;
	}
	if (data->n_pipes > MAX_PIPES)
		fatal_error();
	while (i < data->n_pipes)
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
			// cmd_arr = strlst_to_strarr(\
			// 			cmd_parser(margs->argv[cmd_idx + 1], NULL, 0));
			// execve(get_cmd_path(cmd_arr[0], margs->env), cmd_arr, margs->env);
			// perror("pipex");
			exit(0);
		}
		find_next_cmd(&lst);
	}
}


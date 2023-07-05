#include "../minishell.h"

char	*get_prog_name(t_list *lst)
{
	char	*prog_name;
	t_data	*content;

	prog_name = NULL;
	while (lst != NULL)
	{
		content = ((t_data *)lst->content);
		if (content->type == pipe_)
			break ;
		if (content->type == prog_name_)
		{
			prog_name = content->str;
			break ;
		}
		lst = lst->next;
	}
	if (prog_name_ == NULL)
		exit(0); // free stuff as well
	return (prog_name);
}

void	exec_builtin(char *name, char **args, t_master *data)
{
	name = NULL;
	args = NULL;
	data = NULL;
	exit(0);
}

void	check_builtin(char *prog_name, char **args, t_master *data)
{
	int	pn_len;

	pn_len = ft_strlen(prog_name);
	if ((pn_len == 4 && ft_strncmp(prog_name, "echo", 4))
		|| (pn_len == 3 && ft_strncmp(prog_name, "env", 3))
		|| (pn_len == 6 && ft_strncmp(prog_name, "export", 6))
		|| (pn_len == 3 && ft_strncmp(prog_name, "pwd", 3))
		|| (pn_len == 5 && ft_strncmp(prog_name, "unset", 5))
		|| (pn_len == 2 && ft_strncmp(prog_name, "cd", 2))
		|| (pn_len == 4 && ft_strncmp(prog_name, "exit", 4)))
		exec_builtin(prog_name, args, data);
		
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
			check_builtin(get_prog_name(lst), get_prog_args(lst), data);
			// execve(get_prog_path(lst), get_prog_args(lst), get_env(data));
			exit(0);
		}
		find_next_cmd(&lst);
	}
}


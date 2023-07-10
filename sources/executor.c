#include "../minishell.h"

char	*get_pname(t_list *lst)
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
	if (prog_name == NULL)
		exit(0); // free stuff as well
	return (prog_name);
}

void	exec_builtin(char *prog_name, char **args, t_master *data)
{
	int	pn_len;

	args = NULL; // delete when used
	data = NULL; // delete when used
	pn_len = ft_strlen(prog_name);
	if (pn_len == 6 && ft_strncmp(prog_name, "export", 6))
		return ; // exec export must NOT finish with exit()
	else if (pn_len == 5 && ft_strncmp(prog_name, "unset", 5))
		return ; // exec unset must NOT finish with exit()
	else if (pn_len == 2 && ft_strncmp(prog_name, "cd", 2))
		return ; // exec cd must NOT finish with exit()
	else if (pn_len == 4 && ft_strncmp(prog_name, "exit", 4))
		return ; // exec exit must NOT finish with exit()
	else if (pn_len == 4 && ft_strncmp(prog_name, "echo", 4))
		exit(0); // exec echo must finish with exit()
	else if (pn_len == 3 && ft_strncmp(prog_name, "env", 3))
		exit(0); // exec env must finish with exit()
	else if (pn_len == 3 && ft_strncmp(prog_name, "pwd", 3))
		exit(0); // exec pwd must finish with exit()
}

int	check_builtin(char *prog_name, char **args, t_master *data)
{
	int	pn_len;

	pn_len = ft_strlen(prog_name);
	if (data->n_pipes == 0 && data->cmd_idx < 0 && prog_name != NULL)
	{
		if ((pn_len == 6 && !ft_strncmp(prog_name, "export", 6))
			|| (pn_len == 5 && !ft_strncmp(prog_name, "unset", 5))
			|| (pn_len == 2 && !ft_strncmp(prog_name, "cd", 2))
			|| (pn_len == 4 && !ft_strncmp(prog_name, "exit", 4)))
			return (exec_builtin(prog_name, args, data), 1);
	}
	else if (prog_name != NULL)
	{
		if ((pn_len == 6 && !ft_strncmp(prog_name, "export", 6))
			|| (pn_len == 5 && !ft_strncmp(prog_name, "unset", 5))
			|| (pn_len == 2 && !ft_strncmp(prog_name, "cd", 2))
			|| (pn_len == 4 && !ft_strncmp(prog_name, "exit", 4)))
			exit(0);
		if ((pn_len == 4 && !ft_strncmp(prog_name, "echo", 4))
			|| (pn_len == 3 && !ft_strncmp(prog_name, "env", 3))
			|| (pn_len == 3 && !ft_strncmp(prog_name, "pwd", 3)))
			return (exec_builtin(prog_name, args, data), 1);
	}
	return (0);
}

void	executor(t_master *data)
{
	t_list	*lst;

	lst = data->parsed_lst;
	data->cmd_idx = -1;
	count_pipes(data);
	if (check_builtin(get_pname(lst), get_pargs(lst), data))
		return ;
	init_pipes(data);
	while (lst != NULL)
	{
		data->cmd_idx++;
		data->pids[data->cmd_idx] = fork();
		if (data->pids[data->cmd_idx] == -1)
			fatal_error();
		if (data->pids[data->cmd_idx] == 0)
		{
			set_all_redirections(data, lst);
			check_builtin(get_pname(lst), get_pargs(lst), data);
			execve(get_prog_path(lst, get_path_arr(data)),
				get_pargs(lst), get_env_arr(data));
			exit(1);
		}
		find_next_cmd(&lst);
	}
}


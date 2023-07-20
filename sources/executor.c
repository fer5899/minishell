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
	if (str_equal(prog_name, "export"))
		export_unset(data, args, 0);
	else if (str_equal(prog_name, "unset"))
		export_unset(data, args, 1);
	else if (str_equal(prog_name, "cd"))
		cd(data, args);
	else if (str_equal(prog_name, "exit"))
		exit_builtin(data, args);
	else if (str_equal(prog_name, "echo"))
		echo(data, args);
	else if (str_equal(prog_name, "env"))
		env(data);
	else if (str_equal(prog_name, "pwd"))
		pwd();
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

void	run_process(t_master *data, t_list *lst)
{
	set_all_redirections(data, lst);
	check_builtin(get_pname(lst), get_pargs(data, lst), data);
	execve(get_prog_path(lst, get_path_arr(data)),
		get_pargs(data, lst), get_env_arr(data));
	exit(12);
}

void	executor(t_master *data)
{
	t_list	*lst;

	lst = data->parsed_lst;
	get_all_input_heredoc(data);
	data->cmd_idx = -1;
	count_pipes(data);
	if (check_builtin(get_pname(lst), get_pargs(data, lst), data))
		return ;
	init_pipes(data);
	while (lst != NULL)
	{
		data->cmd_idx++;
		data->pids[data->cmd_idx] = fork();
		if (data->pids[data->cmd_idx] == -1)
			fatal_error();
		if (data->pids[data->cmd_idx] == 0)
			run_process(data, lst);
		find_next_cmd(&lst);
	}
	close_fds(data);
	catch_exit_code(data);
}


#include "../minishell.h"

char	*get_pname(t_master *d, t_list *lst)
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
		free_master_exit(d, 0);
	return (prog_name);
}

void	exec_builtin(char *prog_name, t_master *d)
{
	if (str_equal(prog_name, "export"))
		export_unset(d, d->args, 0);
	else if (str_equal(prog_name, "unset"))
		export_unset(d, d->args, 0);
	else if (str_equal(prog_name, "cd"))
		cd(d);
	else if (str_equal(prog_name, "exit"))
		exit_builtin(d);
	else if (str_equal(prog_name, "echo"))
		echo(d, d->args);
	else if (str_equal(prog_name, "env"))
		env_builtin(d);
	else if (str_equal(prog_name, "pwd"))
		pwd(d);
}

int	check_builtin(char *prog_name, t_master *d)
{
	if (d->n_pipes == 0 && d->cmd_idx < 0 && prog_name != NULL)
	{
		if (str_equal(prog_name, "export") || str_equal(prog_name, "unset")
			|| str_equal(prog_name, "cd") || str_equal(prog_name, "exit"))
			return (exec_builtin(prog_name, d), 1);
	}
	else if (prog_name != NULL && d->cmd_idx >= 0)
	{
		if (str_equal(prog_name, "export") || str_equal(prog_name, "unset")
			|| str_equal(prog_name, "cd") || str_equal(prog_name, "exit"))
			free_master_exit(d, 0);
		if (str_equal(prog_name, "echo") || str_equal(prog_name, "env")
			|| str_equal(prog_name, "pwd"))
			return (exec_builtin(prog_name, d), 1);
	}
	return (0);
}

void	run_process(t_master *d, t_list *lst)
{
	set_all_redirections(d, lst);
	get_pargs(d, lst);
	check_builtin(get_pname(d, lst), d);
	execve(get_prog_path(d, lst, get_path_arr(d)),
		d->args, get_env_arr(d));
	free_master_exit(d, 1);
}

void	executor(t_master *d)
{
	t_list	*lst;

	lst = d->parsed_lst;
	get_all_input_heredoc(d);
	d->cmd_idx = -1;
	count_pipes(d);
	get_pargs(d, lst);
	if (check_builtin(get_pname(d, lst), d))
		return ;
	init_pipes(d);
	while (lst != NULL)
	{
		d->cmd_idx++;
		d->pids[d->cmd_idx] = fork();
		if (d->pids[d->cmd_idx] == -1)
			fatal_error(d);
		if (d->pids[d->cmd_idx] == 0)
			run_process(d, lst);
		find_next_cmd(&lst);
	}
	close_fds(d);
	catch_exit_code(d);
}


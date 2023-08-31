#include "../minishell.h"

extern int	g_prog_state;

void	launch_fork(t_master *d, t_list *lst)
{
	d->cmd_idx++;
	g_prog_state = process;
	d->pids[d->cmd_idx] = fork();
	if (d->pids[d->cmd_idx] == -1)
		fatal_error(d);
	if (d->pids[d->cmd_idx] == 0)
		run_process(d, lst);
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
		if ((str_equal(prog_name, "export") && d->nargs != 1)
			|| str_equal(prog_name, "unset") || str_equal(prog_name, "cd")
			|| str_equal(prog_name, "exit"))
			return (exec_builtin(prog_name, d), 1);
	}
	else if (prog_name != NULL && d->cmd_idx >= 0)
	{
		if ((str_equal(prog_name, "export") && d->nargs != 1)
			|| str_equal(prog_name, "unset") || str_equal(prog_name, "cd")
			|| str_equal(prog_name, "exit"))
			free_master_exit(d, 0);
		if (str_equal(prog_name, "echo")
			|| (str_equal(prog_name, "export") && d->nargs == 1)
			|| str_equal(prog_name, "env") || str_equal(prog_name, "pwd"))
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

int	executor(t_master *d)
{
	t_list	*lst;

	lst = d->parsed_lst;
	if (lst == NULL)
		return (0);
	get_all_input_heredoc(d);
	if (g_prog_state == exit_heredoc)
		return (1);
	d->cmd_idx = -1;
	count_pipes(d);
	get_pargs(d, lst);
	if (check_builtin(get_pname(d, lst), d))
		return (d->exit_code);
	init_pipes(d);
	while (lst != NULL)
	{
		launch_fork(d, lst);
		find_next_cmd(&lst);
	}
	close_fds(d);
	return (catch_exit_code(d));
}


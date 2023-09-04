
#include "../minishell.h"

int	g_prog_state;

void	ft_parse_input(char *command, t_master *master)
{
	t_split	*split;
	char	*str;

	str = expand_env_variables_first_pass(command, master);
	split = ft_split_parser(str, master);
	if (split->error == 1)
	{
		write(2, "syntax error near unexpected token \n", 37);
		master->exit_code = 2;
		free(str);
		ft_free_split_parser(split);
		master->parsed_lst = NULL;
		return ;
	}
	master->parsed_lst = initialize_parsed_lst(split, master);
	free(str);
	free(split);
}

void	increment_shlvl(t_master *d)
{
	char	*shlvl_str;
	int		prev_shlvl;

	shlvl_str = get_env_variable("SHLVL", d);
	if (is_long(shlvl_str))
	{
		prev_shlvl = ft_atoi(shlvl_str);
		if (prev_shlvl >= 1000)
		{
			ft_printf_fd("%s (%d) too high, resetting to 1\n",
				2, "minishell: warning: shell level", prev_shlvl + 1);
			update_env(d, ft_strdup("SHLVL"), ft_strdup("1"), 0);
		}
		else if (prev_shlvl < -1)
			update_env(d, ft_strdup("SHLVL"), ft_strdup("0"), 0);
		else
			update_env(d, ft_strdup("SHLVL"), ft_itoa(prev_shlvl + 1), 0);
	}
	else
		update_env(d, ft_strdup("SHLVL"), ft_strdup("1"), 0);
}

void	ft_command(char *command, t_master *master, int exit_code)
{
	if (!command)
	{
		if (isatty(STDIN_FILENO))
			write(2, "exit\n", 5);
		free(command);
		exit_code = ft_free_env_list(master);
		exit(exit_code);
	}
	else
	{
		if (str_equal(command, ""))
			return ;
		ft_parse_input(command, master);
		add_history(command);
		if (master->exit_code == 2 && !master->parsed_lst)
			return ;
		master->exit_code = executor(master);
		if (master->parsed_lst)
			ft_free_data_list(master->parsed_lst);
		free(command);
	}
}

int	main(void)
{
	t_master	*master;
	char		*command;
	int			exit_code;

	master = inicialize_struct();
	increment_shlvl(master);
	handle_signals();
	exit_code = 0;
	while (1)
	{
		g_prog_state = basic_prompt;
		command = readline("minishell$ ");
		ft_command(command, master, exit_code);
	}
	exit_code = ft_free_env_list(master);
	return (exit_code);
}

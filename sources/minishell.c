
#include "../minishell.h"

char	*command;

void	print_parsed_list(t_list *parsed_lst)
{
	t_list	*current;
	t_data	*data;

	current = parsed_lst;
	while (current)
	{
		data = (t_data *)current->content;
		printf("%s=%d - %c\n", data->str, data->type, data->char_type);
		current = current->next;
	}
	printf("\n");
}

int	ft_type_of_data(char *str, int arg_flag)
{
	if (*str == '<')
	{
		str++ ;
		if (*str == '<')
			return (3);
		else
			return (2);
	}
	else if (*str == '>')
	{
		str++ ;
		if (*str == '>')
			return (6);
		else
			return (5);
	}
	else if (*str == '|')
		return (7);
	else if (arg_flag == 0)
		return (0);
	else
		return (1);
}

void	*ft_syntax_error(t_init_list *l, t_split *split)
{
	printf("%s\n", "Syntax error PAPA");
	ft_free_data_list(l->list);
	while (split->str)
	{
		split++ ;
		free(split->str);
	}
	free(l->data);
	free(l);
	return (NULL);
}

void	*ft_initialize_parsed_lst_data_1(t_init_list *l, t_split *split)
{
	l->data = ft_calloc(1, sizeof(t_data));
	l->data->type = ft_type_of_data(split->str, l->arg_flag);
	l->data->char_type = split->char_type;
	if (l->data->type == 0)
		l->arg_flag = 1;
	else if (l->data->type == 7)
	{
		if ((split + 1)->str == NULL)
		{
			free(split->str);
			return (ft_syntax_error(l, split));
		}
		else
			l->arg_flag = 0;
	}
	return ((void *)1);
}

void	*ft_initialize_parsed_lst_data_2(t_init_list *l, t_split *split)
{
	if (l->data->type == 2 || l->data->type == 3 || l->data->type == 5 
		|| l->data->type == 6)
	{
		free((split + l->i)->str);
		if (l->data->type == 3
			&& ((split + l->i + 1)->char_type == '\'' 
				|| (split + l->i + 1)->char_type == '"'))
			l->data->type = 4;
		if (((split + l->i) + 1)->str == NULL 
			|| ft_type_of_data((split + l->i + 1)->str, l->arg_flag) == 2 
			|| ft_type_of_data((split + l->i + 1)->str, l->arg_flag) == 3
			|| ft_type_of_data((split + l->i + 1)->str, l->arg_flag) == 5
			|| ft_type_of_data((split + l->i + 1)->str, l->arg_flag) == 6
			|| ft_type_of_data((split + l->i + 1)->str, l->arg_flag) == 7)
			return (ft_syntax_error(l, (split + l->i)));
		else
		{
			l->i++ ;
			l->data->str = (split + l->i)->str;
			l->data->char_type = (split + l->i)->char_type;
		}
	}
	else
		l->data->str = (split + l->i)->str;
	return ((void *)1);
}

t_list	*initialize_parsed_lst(t_split *split, t_master *master)
{
	t_init_list	*l;
	t_list		*list;

	l = (t_init_list *)malloc(sizeof(t_init_list));
	l->list = NULL;
	l->arg_flag = 0;
	l->i = 0;
	while ((split + l->i)->str)
	{
		if (ft_initialize_parsed_lst_data_1(l, (split + l->i)) == NULL)
			return (NULL);
		if (ft_initialize_parsed_lst_data_2(l, split) == NULL)
			return (NULL);
		if (l->data->char_type != '\'')
			l->data->str = expand_env_variables(l->data->str, master);
		l->new = ft_lstnew(l->data);
		ft_lstadd_back(&l->list, l->new);
		l->i++ ;
	}
	list = l->list;
	free(l);
	return (list);
}

void	ft_free_split_parser(t_split *split)
{
	t_split	*split_2;

	split_2 = split;
	while (split->str)
	{
		free(split->str);
		split++ ;
	}
	free(split_2);
}

void	ft_parse_input(char *command, t_master *master)
{
	t_split	*split;

	split = ft_split_parser(command);
	if (split->error == 1)
	{
		printf("%s\n", "Syntax error");
		ft_free_split_parser(split);
		master->parsed_lst = NULL;
		return ;
	}
	master->parsed_lst = initialize_parsed_lst(split, master);
	free(split);
}

void	handle_ctrl_c(int signum)
{
	(void)signum;
	rl_replace_line("", 0);
	ft_printf("\n");
	rl_on_new_line();
	//ft_printf("\n%s\n", command);
	if (!command || ft_strncmp(command, "cat", 3) != 0) // Mejor soucion por ahora
		rl_redisplay();
}

void	handle_signals(void)
{
	struct sigaction	sa_1;
	struct sigaction	sa_2;
	struct sigaction	sa_3;

	sa_1.sa_handler = handle_ctrl_c;
	sigaction(SIGINT, &sa_1, NULL);
	sa_2.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa_2, NULL);
	sa_3.sa_handler = SIG_IGN;
	sigaction(SIGTSTP, &sa_3, NULL);
}

int	main(void)
{
	// char		*user;
	t_master	*master;

	master = inicialize_struct();
	//print_env_list(master);
	handle_signals();
	while (1)
	{
		//user = get_env_variable("USER", master);
		//user = ft_strjoin(user, "$ ");
		//command = readline(user);
		command = readline("minishell$ ");
		//if (command == NULL)
		//{
		//	printf("\nexit\n");
		//	free (command);
		//	break ;
		//}
		if (!command) // Added for the MPANIC tester
		{
			if (isatty(STDIN_FILENO))
				write(2, "exit\n", 6);
			free (command);
			exit(master->exit_code);
		}
		else
		{
			ft_parse_input(command, master);
			// print_parsed_list(master->parsed_lst);
			add_history(command);
			master->exit_code = executor(master);
			if (master->parsed_lst)
				ft_free_data_list(master->parsed_lst);
			free(command);
		}
	}
	ft_free_env_list(master);
	return (master->exit_code);
}


#include "../minishell.h"

int	g_prog_state;

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

int	ft_type_of_data(char *str, int arg_flag, char char_type)
{
	if (*str == '<' && char_type != '\'' && char_type != '"')
	{
		str++ ;
		if (*str == '<')
			return (3);
		else
			return (2);
	}
	else if (*str == '>' && char_type != '\'' && char_type != '"')
	{
		str++ ;
		if (*str == '>')
			return (6);
		else
			return (5);
	}
	else if (*str == '|' && char_type != '\'' && char_type != '"')
		return (7);
	else if (arg_flag == 0)
		return (0);
	else
		return (1);
}

void	*ft_syntax_error(t_init_list *l, t_split *split, t_master *master)
{
    write(2, "syntax error near unexpected token \n", 37);
    master->exit_code = 2;
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

void	*ft_initialize_parsed_lst_data_1(t_init_list *l, t_split *split, t_master *master)
{
	l->data = ft_calloc(1, sizeof(t_data));
	l->data->type = ft_type_of_data(split->str, l->arg_flag, split->char_type);
	l->data->char_type = split->char_type;
	if (l->data->type == 0)
		l->arg_flag = 1;
	else if (l->data->type == 7)
	{
		if ((split + 1)->str == NULL || !(split - 1)->str || *(split + 1)->str == '|')
		{
			free(split->str);
			return (ft_syntax_error(l, split, master));
		}
		else
			l->arg_flag = 0;
	}
	return ((void *)1);
}

void	*ft_initialize_parsed_lst_data_2(t_init_list *l, t_split *split, t_master *master)
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
			|| ft_type_of_data((split + l->i + 1)->str, l->arg_flag , (split + l->i + 1)->char_type) == 2 
			|| ft_type_of_data((split + l->i + 1)->str, l->arg_flag , (split + l->i + 1)->char_type) == 3
			|| ft_type_of_data((split + l->i + 1)->str, l->arg_flag , (split + l->i + 1)->char_type) == 5
			|| ft_type_of_data((split + l->i + 1)->str, l->arg_flag , (split + l->i + 1)->char_type) == 6
			|| ft_type_of_data((split + l->i + 1)->str, l->arg_flag , (split + l->i + 1)->char_type) == 7)
			return (ft_syntax_error(l, (split + l->i), master));
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

t_list	*initialize_parsed_lst(t_split *split)
{
	t_init_list	*l;
	t_list		*list;

	l = (t_init_list *)malloc(sizeof(t_init_list));
	l->list = NULL;
	l->arg_flag = 0;
	l->i = 0;
	while ((split + l->i)->str)
	{
		if (ft_initialize_parsed_lst_data_1(l, (split + l->i), master) == NULL)
			return (NULL);
		if (ft_initialize_parsed_lst_data_2(l, split, master) == NULL)
			return (NULL);
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

void	print_split(t_split *split)
{
	while (split->str)
	{
		if (split->error)
			printf("error: %d\n", split->error);
		ft_printf("%s -- %c\n", split->str, split->char_type);
		split++;
	}
}

void	ft_parse_input(char *command, t_master *master)
{
	t_split	*split;
    char    *str;

	str = expand_env_variables_first_pass(command, master);
    //ft_printf("str1: %s\n", str);
    split = ft_split_parser(str, master);
	//print_split(split);
	if (split->error == 1)
	{
		write(2, "syntax error near unexpected token \n", 37);
        master->exit_code = 2;
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


int	main(void)
{
	t_master	*master;
	char		*command;
	int			exit_code;

	master = inicialize_struct();
	increment_shlvl(master);
	handle_signals();
	while (1)
	{
		g_prog_state = basic_prompt;
		command = readline("minishell$ ");
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
				continue ;
			ft_parse_input(command, master);
			// print_parsed_list(master->parsed_lst);
			add_history(command);
            // if (master->exit_code != 2)
			    master->exit_code = executor(master);
			if (master->parsed_lst)
				ft_free_data_list(master->parsed_lst);
			free(command);
		}
	}
	exit_code = ft_free_env_list(master);
	return (exit_code);
}

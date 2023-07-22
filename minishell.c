
#include "minishell.h"

void	ft_free_split_parser(t_split *split);

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

void	*ft_syntax_error(t_list *list, t_data *data, t_split *split)
{
	printf("%s\n", "Syntax error PAPA");
	//printf("%s\n", split->str);
	//print_parsed_list(list);
	ft_free_data_list(list);
	while (split->str)
	{
		split++ ;
		free(split->str);
	}
	free(data);
	return (NULL);
}

t_list	*inicialize_data(t_split *split, t_master *master)
{
	int		arg_flag;
	t_list	*list;
	t_list	*new;
	t_data	*data;

	list = NULL;
	arg_flag = 0;
	while (split->str)
	{
		data = ft_calloc(1, sizeof(t_data));
		data->type = ft_type_of_data(split->str, arg_flag);
		data->char_type = split->char_type;
		if (data->type == 0)
			arg_flag = 1;
		else if (data->type == 7)
		{
			if ((split + 1)->str == NULL)
				return (ft_syntax_error(list, data, split));
			else
				arg_flag = 0;
		}
		if (data->type == 2 || data->type == 3 || data->type == 5 
			|| data->type == 6)
		{
			free(split->str);
			if (data->type == 3 && (split->char_type == '\'' || split->char_type == '"'))
				data->type = 4;
			if ((split + 1)->str == NULL || ft_type_of_data((split + 1)->str, arg_flag) == 2 || ft_type_of_data((split + 1)->str, arg_flag) == 3  || ft_type_of_data((split + 1)->str, arg_flag) == 5
				 || ft_type_of_data((split + 1)->str, arg_flag) == 6  || ft_type_of_data((split + 1)->str, arg_flag) == 7)
				return (ft_syntax_error(list, data, split));
			else
			{
				split++ ;
				data->str = split->str;
				data->char_type = split->char_type;
			}
		}
		else
			data->str = split->str;
		if (data->char_type != '\'')
			data->str = expand_env_variables(data->str, master);
		new = ft_lstnew(data);
		ft_lstadd_back(&list, new);
		split++ ;
	}
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
	t_split	*split_free;

	split = ft_split_parser(command);
	if (split->error == 1)
	{
		printf("%s\n", "Syntax error");
		ft_free_split_parser(split);
		return ;
	}
	master->parsed_lst = inicialize_data(split, master);
	free(split);
}

int	main(void)
{
	char		*command;
	t_master	*master;

	master = inicialize_struct();
	//print_env_list(master);
	while (1)
	{
		printf("%s", get_env_variable("USER", master));
		command = readline("$ ");
		if (ft_strlen(command) == 4 && ft_strncmp(command, "exit", 4) == 0)
		{
			free(command);
			break ;
		}
		else
		{
			ft_parse_input(command, master);
			print_parsed_list(master->parsed_lst);
			ft_free_data_list(master->parsed_lst);
			free(command);
		}
	}
	ft_free_env_list(master);
	return (0);
}


#include "minishell.h"

void	leaks(void)
{
	system("leaks -q minishell");
}

void	print_parsed_list(t_master *master)
{
	t_list	*current;
	t_data	*data;

	current = master->parsed_lst;
	while (current)
	{
		data = (t_data *)current->content;
		printf("%s=%d\n", data->str, data->type);
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
			return (5);
		else
			return (4);
	}
	else if (*str == '|')
		return (6);
	else if (arg_flag == 0)
		return (0);
	else
		return (1);
}

t_list	*inicialize_data(char **split)
{
	int		arg_flag;
	t_list	*list;
	t_list	*new;
	t_data	*data;

	list = NULL;
	arg_flag = 0;
	while (*split)
	{
		data = ft_calloc(1, sizeof(t_data));
		data->type = ft_type_of_data(*split, arg_flag);
		if (data->type == 0)
			arg_flag = 1;
		else if (data->type == 6)
			arg_flag = 0;
		if (data->type == 2 || data->type == 4 || data->type == 5)
		{
			free(*split);
			split++ ;
			data->str = *split;
		}
		else
			data->str = *split;
		new = ft_lstnew(data);
		ft_lstadd_back(&list, new);
		split++ ;
	}
	return (list);
}

void	ft_parse_input(char *command, t_master *master)
{
	char	**split;

	split = ft_split(command, ' ');
	master->parsed_lst = inicialize_data(split);
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
		if (command == NULL)
		{
			printf("\n");
			break ;
		}
		if (ft_strlen(command) == 4 && ft_strncmp(command, "exit", 4) == 0)
		{
			free(command);
			break ;
		}
		else
		{
			ft_parse_input(command, master);
			print_parsed_list(master);
			ft_free_data_list(master);
			free(command);
		}
	}
	ft_free_env_list(master);
	//atexit(leaks);
	return (0);
}

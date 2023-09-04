#include "../minishell.h"

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

void	*ft_initialize_parsed_lst_data_1(t_init_list *l,
			t_split *split, t_master *master)
{
	l->data = ft_calloc(1, sizeof(t_data));
	l->data->type = ft_type_of_data(split->str, l->arg_flag, split->char_type);
	l->data->char_type = split->char_type;
	if (l->data->type == 0)
		l->arg_flag = 1;
	else if (l->data->type == 7)
	{
		if ((split + 1)->str == NULL || !(split - 1)->str
			|| *(split + 1)->str == '|')
		{
			free(split->str);
			return (ft_syntax_error(l, split, master));
		}
		else
			l->arg_flag = 0;
	}
	return ((void *)1);
}

void	*ft_initialize_parsed_lst_data_2_aux(t_init_list *l,
			t_split *split, t_master *master)
{
	if (((split + l->i) + 1)->str == NULL
		|| ft_type_of_data((split + l->i + 1)->str,
			l->arg_flag, (split + l->i + 1)->char_type) == 2
		|| ft_type_of_data((split + l->i + 1)->str,
			l->arg_flag, (split + l->i + 1)->char_type) == 3
		|| ft_type_of_data((split + l->i + 1)->str,
			l->arg_flag, (split + l->i + 1)->char_type) == 5
		|| ft_type_of_data((split + l->i + 1)->str,
			l->arg_flag, (split + l->i + 1)->char_type) == 6
		|| ft_type_of_data((split + l->i + 1)->str,
			l->arg_flag, (split + l->i + 1)->char_type) == 7)
		return (ft_syntax_error(l, (split + l->i), master));
	else
	{
		l->i++ ;
		l->data->str = (split + l->i)->str;
		l->data->char_type = (split + l->i)->char_type;
	}
	return ((void *)1);
}

void	*ft_initialize_parsed_lst_data_2(t_init_list *l,
			t_split *split, t_master *master)
{
	if (l->data->type == 2 || l->data->type == 3 || l->data->type == 5
		|| l->data->type == 6)
	{
		free((split + l->i)->str);
		if (l->data->type == 3
			&& ((split + l->i + 1)->char_type == '\''
				|| (split + l->i + 1)->char_type == '"'))
			l->data->type = 4;
		return (ft_initialize_parsed_lst_data_2_aux(l, split, master));
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

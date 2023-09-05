/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvgomez <alvgomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:42:54 by alvgomez          #+#    #+#             */
/*   Updated: 2023/09/05 11:42:55 by alvgomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

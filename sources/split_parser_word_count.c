/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_parser_word_count.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvgomez <alvgomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:44:05 by alvgomez          #+#    #+#             */
/*   Updated: 2023/09/05 11:44:07 by alvgomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_check_for_pipes(t_split_param *sp)
{
	if (*(sp->s) == '|' && !sp->inside_quotes)
	{
		if (sp->pipe == 0)
		{
			sp->count_2++;
			sp->is_word = 1;
		}
		sp->pipe++;
		if (sp->pipe > 1)
			sp->error = 1;
		sp->after_sep = 1;
	}
	else
		sp->pipe = 0;
}

void	ft_check_for_redirections_left(t_split_param *sp)
{
	if (*(sp->s) == '<' && !sp->inside_quotes)
	{
		if (sp->red_l == 0)
		{
			sp->count_2++;
			sp->is_word = 1;
		}
		sp->red_l++;
		if (sp->red_l > 2)
			sp->error = 1;
		sp->after_sep = 1;
	}
	else
		sp->red_l = 0;
}

void	ft_check_for_redirections_right(t_split_param *sp)
{
	if (*(sp->s) == '>' && !sp->inside_quotes)
	{
		if (sp->red_r == 0)
		{
			sp->count_2++;
			sp->is_word = 1;
		}
		sp->red_r++;
		if (sp->red_r > 2)
			sp->error = 1;
		sp->after_sep = 1;
	}
	else
		sp->red_r = 0;
}

void	normal_character_check(t_split_param *sp)
{
	if (sp->after_sep == 1 && *(sp->s) != '>'
		&& *(sp->s) != '<' && *(sp->s) != '|')
	{
		sp->is_word = 0;
		sp->after_sep = 0;
	}
	if ((sp->is_word == 0 && !sp->inside_quotes))
	{
			sp->count_2++;
			sp->is_word = 1;
	}
}

void	word_count_options(t_split_param *sp)
{
	ft_check_for_redirections_left(sp);
	ft_check_for_redirections_right(sp);
	ft_check_for_pipes(sp);
	if (((*(sp->s) == '"' && (!*((sp->s) - 1) || *((sp->s) - 1) != '\\'))
			|| *(sp->s) == '\'') && !sp->inside_quotes)
	{
		sp->inside_quotes = 1;
		sp->quote_type = *(sp->s);
		if (sp->after_sep == 1)
			sp->after_sep = 0;
		sp->count_2++;
		sp->is_word = 1;
	}
	else if (*(sp->s) == sp->quote_type && sp->inside_quotes)
	{
		sp->inside_quotes = 0 ;
		sp->quote_type = '\0';
		sp->is_word = 0;
	}
	else if ((*(sp->s) == ' ' && !sp->inside_quotes))
		sp->is_word = 0;
	else
		normal_character_check(sp);
}

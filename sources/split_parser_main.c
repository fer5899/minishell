/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_parser_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvgomez <alvgomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 12:31:08 by alvgomez          #+#    #+#             */
/*   Updated: 2023/09/05 12:31:10 by alvgomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	word_count(t_split_param *sp)
{
	sp->count_2 = 0;
	sp->inside_quotes = 0;
	sp->is_word = 0;
	sp->quote_type = '\0';
	sp->red_l = 0;
	sp->red_r = 0;
	sp->pipe = 0;
	sp->after_sep = 0;
	while (*(sp->s))
	{
		word_count_options(sp);
		sp->s++;
	}
	if (sp->inside_quotes == 1)
		sp->error = 1;
	return (sp->count_2);
}

void	ft_check_for_separators(t_split_param *sp, char *s)
{
	if (*s == '\'' || (*s == '"' && (!*(s - 1) || *(s - 1) != '\\')))
	{
		sp->char_type = *s;
		s++;
	}
	else
		sp->char_type = ' ';
	if (*s == '<')
		sp->sep_type = '<';
	else if (*s == '>')
		sp->sep_type = '>';
	else if (*s == '|')
		sp->sep_type = '|';
	else
		sp->sep_type = '\0';
}

void	ft_modify_count_and_sp(t_split_total *st)
{
	st->split->str = *(st->sp->str_1);
	st->split->error = 0;
	st->split->char_type = st->sp->char_type;
	st->split->join_arg = st->sp->join_arg;
	st->sp->str_1++;
	st->split++;
	st->sp->count_1--;
}

void	ft_split_all_words(char *s, t_split_total *st)
{
	while (*s)
	{
		if (*s == ' ' && *s)
		{
			st->sp->join_arg = 0;
			while (*s == ' ' && *s)
				s++;
		}
		else
			st->sp->join_arg = 1;
		if (*s)
		{
			ft_check_for_separators(st->sp, s);
			*(st->sp->str_1) = fill_array_split(s, st->sp);
			ft_modify_count_and_sp(st);
			s = ft_skip_filled_word(st, s);
			st->sp->join_arg = 1;
		}
	}
	*(st->sp->str_1) = 0;
}

t_split	*ft_split_parser(char *s, t_master *master)
{
	t_split_total	*st;
	t_split			*result;
	int				count;

	st = ft_inizialize_split_total(s);
	result = st->split;
	count = st->sp->count_1;
	ft_split_all_words(s, st);
	st->split->str = *(st->sp->str_1);
	if (st->sp->error == 0)
		result = ft_expand_and_join_args(result, count, master);
	result->error = st->sp->error;
	free(st->sp);
	free(st->str_1);
	free(st);
	return (result);
}

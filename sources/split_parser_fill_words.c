/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_parser_fill_words.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvgomez <alvgomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:43:31 by alvgomez          #+#    #+#             */
/*   Updated: 2023/09/05 11:43:32 by alvgomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_fill_array_lenght(char *s, t_split_param *sp)
{
	int		length;

	length = 0;
	if (sp->char_type == ' ')
	{
		if (*s == '>' || *s == '<' || *s == '|')
		{
			while (s[length] == sp->sep_type && s[length])
				length++;
		}
		else
		{
			while ((s[length] != ' ' && s[length] != '>' && s[length] != '<'
					&& s[length] != '|' && (s[length] != '"'
						|| s[length - 1] == '\\') && s[length] != '\'')
				&& s[length])
				length++;
		}
	}
	else if (sp->char_type == '\'' || sp->char_type == '"')
	{
		while (s[length + 1] != sp->char_type && s[length])
			length++;
	}
	return (length);
}

int	ft_fill_array_extra(char *s, t_split_param *sp, char *str)
{
	int	i;

	i = 0;
	if (s[i] == '>' || s[i] == '<' || s[i] == '|')
	{
		sp->join_arg = 0;
		while ((s[i] == sp->sep_type) && s[i])
		{
			str[i] = s[i];
			i++;
		}
	}
	else
	{
		while ((s[i] != sp->char_type && s[i] != ' ' && s[i] != '>'
				&& s[i] != '<' && s[i] != '|' && (s[i] != '"'
					|| s[i - 1] == '\\') && s[i] != '\'') && s[i])
		{
			str[i] = s[i];
			i++;
		}
	}
	return (i);
}

char	*fill_array_split(char *s, t_split_param *sp)
{
	int		i;
	int		length;
	char	*str;

	i = 0;
	length = ft_fill_array_lenght(s, sp);
	str = (char *)ft_calloc((length + 1), sizeof(char));
	if (!str)
		return (0);
	if (sp->char_type == ' ')
		i = ft_fill_array_extra(s, sp, str);
	else if (sp->char_type == '\'' || sp->char_type == '"')
	{
		while (s[i + 1] != sp->char_type && s[i])
		{
			str[i] = s[i + 1];
			i++;
		}
	}
	str[i] = '\0';
	return (str);
}

char	*ft_skip_filled_word(t_split_total *st, char *s)
{
	if ((st->sp->char_type == '\'' || st->sp->char_type == '"') && *s)
	{
		s++;
		while (*s != st->sp->char_type && *s)
			s++;
		if (*s == '\'' || *s == '"')
			s++;
	}
	else if (st->sp->char_type == ' ' && *s)
	{
		if ((*s == '<' || *s == '>' || *s == '|') && *s)
		{
			while ((*s == st->sp->sep_type) && *s)
				s++;
		}
		else
		{
			while ((*s != ' ' && *s != '<' && *s != '>'
					&& *s != '|' && (*s != '"' || *(s - 1) == '\\')
					&& *s != '\'' ) && *s)
				s++;
		}
	}
	return (s);
}

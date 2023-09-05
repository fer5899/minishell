/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_parser_exp_and_join.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvgomez <alvgomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:43:24 by alvgomez          #+#    #+#             */
/*   Updated: 2023/09/05 11:43:26 by alvgomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_count_args(t_split *split, int count)
{
	while (split->str)
	{
		if (*split->str != '>' && *split->str != '<' && *split->str != '|'
			&& (split + 1)->join_arg && (split + 1)->join_arg == 1)
			count--;
		split++;
	}
	return (count);
}

void	aux_join_args(t_split *split, t_split *result, t_master *master)
{
	char	*str_free;

	str_free = result->str;
	if ((split + 1)->char_type == '"')
		(split + 1)->str = expand_env_variables_second_pass((split + 1)->str,
				master);
	result->str = ft_strjoin(result->str, (split + 1)->str);
	free(str_free);
	free((split + 1)->str);
	if (split->char_type == '\'' || (split + 1)->char_type == '\''
		|| split->char_type == '"' || (split + 1)->char_type == '"')
		result->char_type = '\'';
	else
		result->char_type = split->char_type;
}

void	ft_go_through_args(t_split *split, t_split *result,
			int count, t_master *master)
{
	while (count >= 0 && split->str)
	{
		*result = *split;
		if (split->char_type == '"')
			result->str = expand_env_variables_second_pass(split->str, master);
		if (*split->str != '>' && *split->str != '<' && *split->str != '|'
			&& (split + 1) && (split + 1)->join_arg == 1)
		{
			while ((split + 1) && (split + 1)->join_arg == 1)
			{
				aux_join_args(split, result, master);
				split++;
			}
			count--;
			result++;
		}
		else
		{
			count--;
			result++;
		}
		split++;
	}
}

void	ft_delete_slashes(t_split *result, int i, int j)
{
	char	*str;
	char	*str_free;

	while (result->str)
	{
		if (result->char_type == ' ')
		{
			str = result->str;
			i = 0;
			j = 0;
			while (str[i])
			{
				if (str[i] == '\\' && str[i + 1] == '"')
					i++;
				str[j] = str[i];
				i++;
				j++;
			}
			str[j] = '\0';
			str_free = result->str;
			result->str = ft_strdup(str);
			free(str_free);
		}
		result++;
	}
}

t_split	*ft_expand_and_join_args(t_split *split, int count, t_master *master)
{
	t_split	*result;
	t_split	*result_last;
	t_split	*split_free;

	split_free = split;
	count = ft_count_args(split, count);
	result = (t_split *)ft_calloc((count + 1), sizeof(t_split));
	if (!result)
		exit (1);
	result_last = result;
	ft_go_through_args(split, result, count, master);
	ft_delete_slashes(result, 0, 0);
	free(split_free);
	return (result_last);
}

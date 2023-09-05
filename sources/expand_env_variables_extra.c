/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_variables_extra.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvgomez <alvgomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:42:04 by alvgomez          #+#    #+#             */
/*   Updated: 2023/09/05 11:46:37 by alvgomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_exp_env	*inicialize_env_struct(void)
{
	t_exp_env	*ex;

	ex = (t_exp_env *)ft_calloc(1, sizeof(t_exp_env));
	ex->i = 0;
	ex->new = ft_strdup("");
	ex->env = NULL;
	ex->inside_single_quotes = 0;
	ex->inside_double_quotes = 0;
	return (ex);
}

char	*fix_double_quotes(char *str)
{
	int		i;
	int		num;
	char	*new;

	if (str == NULL)
		return (NULL);
	num = ft_count_char(str, '"');
	new = (char *)ft_calloc((ft_strlen(str) + num + 1), sizeof(char));
	i = 0;
	while (*str)
	{
		if (*str == '"')
		{
			new[i] = '\\';
			i++;
		}
		new[i] = *str;
		i++;
		str++;
	}
	return (new);
}

void	ft_quote_case(t_exp_env *ex, t_master *master)
{
	ex->env = substitude_env_variable_1("HOME", master);
	ex->new_free = ex->new;
	ex->new = ft_strjoin(ex->new, ex->env);
	free(ex->new_free);
	free(ex->env);
}

void	ft_other_case(t_exp_env *ex, char *str)
{
	ex->tmp = ft_substr(str, ex->i, 1);
	ex->new_free = ex->new;
	ex->new = ft_strjoin(ex->new, ex->tmp);
	free(ex->new_free);
	free(ex->tmp);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvgomez <alvgomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:39:54 by alvgomez          #+#    #+#             */
/*   Updated: 2023/09/05 11:39:58 by alvgomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_env(void *nd)
{
	if (((t_env *) nd)->value != NULL)
		ft_printf("%s=%s\n", ((t_env *) nd)->key, ((t_env *) nd)->value);
}

void	print_export(void *nd)
{
	if (((t_env *) nd)->value != NULL)
		ft_printf("declare -x %s=\"%s\"\n",
			((t_env *) nd)->key, ((t_env *) nd)->value);
	else
		ft_printf("declare -x %s\n", ((t_env *) nd)->key);
}

int	is_valid_env_key(char *key)
{
	int	key_len;
	int	i;

	key_len = ft_strlen(key);
	i = -1;
	if (key == NULL || key_len < 1)
		return (0);
	if (ft_isdigit(key[0]))
		return (0);
	while (++i < key_len)
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			if (!(i == key_len - 1 && key[i] == '+'))
				return (0);
	}
	return (1);
}

char	*get_key(char *arg)
{
	char	*equal;
	int		diff;

	equal = ft_strchr(arg, '=');
	if (equal == NULL)
		return (ft_strdup(arg));
	diff = equal - arg;
	if (diff == 0)
		return (NULL);
	return (ft_substr(arg, 0, (size_t) diff));
}

char	*get_value(char *arg)
{
	char	*equal;
	int		diff;

	equal = ft_strchr(arg, '=');
	if (equal == NULL)
		return (NULL);
	diff = equal - arg;
	return (ft_substr(arg, diff + 1, ft_strlen(arg)));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   long_long_parse.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvgomez <alvgomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:42:42 by alvgomez          #+#    #+#             */
/*   Updated: 2023/09/05 11:42:44 by alvgomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_long_digits(char *str)
{
	int	dig;

	if (str == NULL)
		return (-1);
	dig = 0;
	if (*str == '+' || *str == '-')
	{
		if (ft_strlen(str) == 1)
			return (-1);
		else
			str++;
	}
	while (*str == '0' && *str != '\0')
		str++;
	while (str[dig] != '\0')
		dig++;
	if (dig == 0)
		return (1);
	return (dig);
}

int	check_limit(char *str)
{
	char	*limit;
	int		i;

	i = 0;
	limit = "9223372036854775807";
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			limit = "9223372036854775808";
		str++;
	}
	while (*str == '0' && *str != '\0')
		str++;
	while (str[i] != '\0')
	{
		if (str[i] > limit[i])
			return (0);
		if (str[i] < limit[i])
			break ;
		i++;
	}
	return (1);
}

int	is_long_size(char *str)
{
	int		dig;

	dig = get_long_digits(str);
	if (dig > 19)
		return (0);
	else if (dig < 19)
		return (1);
	if (check_limit(str))
		return (1);
	return (0);
}

int	is_long(char *str)
{
	if (str == NULL || *str == '\0')
		return (0);
	if (!is_long_size(str))
		return (0);
	if (*str == '+' || *str == '-')
	{
		if (ft_strlen(str) == 1)
			return (0);
		else
			str++;
	}
	while (*str != '\0')
	{
		if (!('0' <= *str && *str <= '9'))
			return (0);
		str++;
	}
	return (1);
}

int	get_exit_status(char *str)
{
	long long	num;
	int			sign;
	char		*orig;

	if (str == NULL)
		return (0);
	orig = str;
	num = 0;
	sign = 1;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str != '\0')
	{
		num = num * 10 + (*str - 48);
		str++;
	}
	if (sign == 1)
		return (num % 256);
	else
		return ((9223372036854775807 - num + 1) % 256);
}

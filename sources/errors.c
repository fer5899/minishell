/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvgomez <alvgomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:40:50 by alvgomez          #+#    #+#             */
/*   Updated: 2023/09/05 11:40:52 by alvgomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	fatal_error(void)
{
	ft_printf_fd("FATAL ERROR\n", 2);
	exit(1);
}

void	file_error(char *path, char *err_str, int code)
{
	ft_printf_fd("minishell: %s: %s\n", 2, path, err_str);
	exit(code);
}

void	export_unset_error(t_master *d, char *arg, int is_unset)
{
	char	*error;

	if (is_unset)
		error = "unset";
	else
		error = "export";
	ft_printf_fd("minishell: %s: `%s\': not a valid identifier\n",
		2, error, arg);
	d->exit_code = 1;
}

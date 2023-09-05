/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvgomez <alvgomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:42:16 by alvgomez          #+#    #+#             */
/*   Updated: 2023/09/05 11:42:18 by alvgomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_tmp_file(t_master *d)
{
	char	*tmp_path;
	int		tmp_fd;

	tmp_path = get_tmp_path(d);
	tmp_fd = open(tmp_path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	free(tmp_path);
	return (tmp_fd);
}

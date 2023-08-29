#include "../minishell.h"

void	check_wrong_chdir(char *path)
{
	struct stat	path_info;

	if (ft_strlen(path) > 255)
	{
		ft_printf_fd("minishell: cd: %s: File name too long\n",
			2, path);
		return ;
	}
	if (access(path, F_OK) == -1)
		ft_printf_fd("minishell: cd: %s: No such file or directory\n",
			2, path);
    else if (stat(path, &path_info) == 0)
	{
        if (!S_ISDIR(path_info.st_mode))
			ft_printf_fd("minishell: cd: %s: Not a directory\n",
				2, path);
	}
	if (access(path, R_OK | X_OK) == -1)
		ft_printf_fd("minishell: cd: %s: Permission denied\n",
			2, path);
	else
		ft_printf_fd("minishell: cd: %s: No such file or directory\n",
			2, path);
}

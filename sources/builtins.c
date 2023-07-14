#include "../minishell.h"

void	pwd(void)
{
	char	*buf;
	int		buf_size;	

	buf_size = 100;
	buf = (char *) ft_calloc(buf_size, sizeof(char));
	while (!getcwd(buf, buf_size))
	{
		free(buf);
		buf_size += 100;
		if (buf_size > 2147483500)
			exit(1);
		buf = (char *) ft_calloc(buf_size, sizeof(char));
	}
	ft_printf("%s\n", buf);
	exit(0);
}


void	echo(t_master *data, char **args)
{
	int	nl;

	if (data->nargs == 1)
	{
		ft_printf("\n");
		exit(0);
	}
	nl = 1;
	args++;
	if (ft_strlen(*args) == 2 && !ft_strncmp(*args, "-n", 2))
	{
		nl = 0;
		args++;
	}
	while (*args != NULL)
	{
		ft_printf("%s", *args);
		if (*(args + 1) != NULL)
			ft_printf(" ");
		args++;
	}
	if (nl)
		ft_printf("\n");
	exit(0);
}

void	env(t_master *data)
{
	ft_lstiter(data->env_lst, print_env);
	exit(0);
}


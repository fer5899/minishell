#include "../minishell.h"

void	print_env(void *nd)
{
	ft_printf("%s=%s\n", ((t_env *) nd)->key, ((t_env *) nd)->value);
}

char	*get_pwd(void)
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
	return (buf);
}


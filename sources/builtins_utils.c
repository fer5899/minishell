#include "../minishell.h"

void	print_env(void *nd)
{
	ft_printf("%s=%s\n", ((t_env *) nd)->key, ((t_env *) nd)->value);
}

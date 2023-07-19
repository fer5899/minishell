#include "../minishell.h"

void	free_env(void *env_data)
{
	t_env	*env;

	env = (t_env *) env_data;
	free(env->key);
	free(env->value);
	free(env);
}

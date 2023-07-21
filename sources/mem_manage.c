#include "../minishell.h"

void	free_env(void *env_data)
{
	t_env	*env;

	env = (t_env *) env_data;
	if (env->key)
		free(env->key);
	if (env->value)
		free(env->value);
	free(env);
}

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

void	free_parsing(void *parsing_data)
{
	t_data	*prs;

	prs = (t_data *) parsing_data;
	if (prs->str)
		free(prs->str);
	free(prs);
}

void	free_master_exit(t_master *d, int exit_code) // modify with final master struct
{
	(void) d;
	// ft_lstclear(&(d->env_lst), free_env);
	// ft_lstclear(&(d->parsed_lst), free_parsing);
	// if (d->args)
	// 	free(d->args);
	// free(d->fds);
	// free(d->pids);
	exit(exit_code);
}

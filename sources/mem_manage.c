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

void	free_pargs(t_master *d)
{
	char	**args;

	args = d->args;
	if (args == NULL)
		return ;
	while (*args != NULL)
	{
		free(*args);
		args++;
	}
	free(d->args);
}

void	free_master_and_exit(t_master *d, int exit_code)
{
	ft_lstclear(&(d->env_lst), free_env);
	ft_lstclear(&(d->parsed_lst), free_parsing);
	free_pargs(d);
	free(d);
	exit(exit_code);
}

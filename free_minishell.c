
#include "minishell.h"

void	ft_free_env(void *env)
{
	t_env	*tmp;

	tmp = (t_env *)env;
	free(tmp->key);
	free(tmp->value);
	free(tmp);
}

void	ft_free_env_list(t_master *master)
{
	ft_lstclear(&(master->env_lst), ft_free_env);
	free(master);
}

void	ft_free_data(void *data)
{
	t_data	*tmp;

	tmp = (t_data *)data;
	free(tmp->str);
	free(tmp);
}

void	ft_free_data_list(t_master *master)
{
	ft_lstclear(&(master->parsed_lst), ft_free_data);
}
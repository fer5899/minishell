#include "../minishell.h"

void	ft_free_env(void *env)
{
	t_env	*tmp;

	tmp = (t_env *)env;
	free(tmp->key);
	free(tmp->value);
	free(tmp);
}

int	ft_free_env_list(t_master *master)
{
	int	exit_code;

	ft_lstclear(&(master->env_lst), free_env);
	exit_code = master->exit_code;
	free(master);
	return (exit_code);
}

void	ft_free_data(void *data)
{
	t_data	*tmp;

	tmp = (t_data *)data;
	free(tmp->str);
	free(tmp);
}

void	ft_free_data_list(t_list *parsed_lst)
{
	ft_lstclear(&(parsed_lst), ft_free_data);
}

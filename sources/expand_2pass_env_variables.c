#include "../minishell.h"

char	*substitude_env_variable_2(char *key, t_master *master)
{
	t_list	*list;
	t_env	*env;

	list = master->env_lst;
	if (str_equal(key, "?"))
		return (ft_itoa(master->exit_code));
	else if (str_equal(key, "$"))
		return (ft_strdup("$$"));
	else if (key == NULL)
		return (ft_strdup("$"));
	while (list)
	{
		env = (t_env *)list->content;
		if (str_equal(key, env->key))
			return (ft_strdup(env->value));
		list = list->next;
	}
	return (NULL);
}

void	ft_get_key_2(t_exp_env *ex, char *str)
{
	while (str[ex->i + ex->j + 1] && str[ex->i + ex->j + 1] != ' '
		&& str[ex->i + ex->j + 1] != '\'' && str[ex->i + ex->j + 1] != '"'
		&& str[ex->i + ex->j + 1] != '$' && str[ex->i + ex->j + 1] != '?'
		&& str[ex->i + ex->j + 1] != '~' && str[ex->i + ex->j + 1] != '/'
		&& str[ex->i + ex->j + 1] != '|' && str[ex->i + ex->j + 1] != '>'
		&& str[ex->i + ex->j + 1] != '<')
		ex->j++;
	if (str[ex->i + 1] && str[ex->i + 1] == '?')
	{
		ex->key = ft_substr(str, ex->i + 1, 1);
		ex->j++;
	}
	else if (str[ex->i + 1] && str[ex->i + 1] == '$')
	{
		ex->key = ft_substr(str, ex->i + 1, 1);
		ex->j++;
	}
	else if ((str[ex->i + 1] && (str[ex->i + 1] == ' ' || str[ex->i + 1] == '/'
				|| str[ex->i + 1] == '\'' || str[ex->i + 1] == '|'
				|| str[ex->i + 1] == '>' || str[ex->i + 1] == '<'))
		|| !str[ex->i + 1])
		ex->key = NULL;
	else
		ex->key = ft_substr(str, ex->i + 1, ex->j);
}

void	ft_dollar_case_2(t_exp_env *ex, char *str, t_master *master)
{
	ex->j = 0;
	ft_get_key_2(ex, str);
	ex->env = substitude_env_variable_2(ex->key, master);
	if (ex->env)
	{
		ex->new_free = ex->new;
		ex->new = ft_strjoin(ex->new, ex->env);
		free(ex->new_free);
		free(ex->env);
	}
	free(ex->key);
	ex->i += ex->j;
}

char	*expand_env_variables_second_pass(char *str, t_master *master)
{
	t_exp_env	*ex;
	char		*result;

	ex = inicialize_env_struct();
	while (str[ex->i])
	{
		if (str[ex->i] == '$')
			ft_dollar_case_2(ex, str, master);
		else
		{
			ex->tmp = ft_substr(str, ex->i, 1);
			ex->new_free = ex->new;
			ex->new = ft_strjoin(ex->new, ex->tmp);
			free(ex->new_free);
			free(ex->tmp);
		}
		ex->i++;
	}
	result = ex->new;
	free(ex);
	free(str);
	return (result);
}

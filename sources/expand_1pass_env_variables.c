#include "../minishell.h"

char	*substitude_env_variable_1(char *key, t_master *master)
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
			return (fix_double_quotes(env->value));
		list = list->next;
	}
	return (NULL);
}

void	ft_get_key_1(t_exp_env *ex, char *str)
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
	else if ((str[ex->i + 1] && (str[ex->i + 1] == ' '
				|| str[ex->i + 1] == '/')) || !str[ex->i + 1])
		ex->key = NULL;
	else
		ex->key = ft_substr(str, ex->i + 1, ex->j);
}

void	ft_dollar_case_1(t_exp_env *ex, char *str, t_master *master)
{
	ex->j = 0;
	ft_get_key_1(ex, str);
	ex->env = substitude_env_variable_1(ex->key, master);
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

char	*expand_env_variables_first_pass(char *str, t_master *master)
{
	t_exp_env	*ex;
	char		*result;

	ex = inicialize_env_struct();
	while (str[ex->i])
	{
		if (str[ex->i] == '\'' && ex->inside_double_quotes == 0)
			ex->inside_single_quotes = !ex->inside_single_quotes;
		else if (str[ex->i] == '"' && ex->inside_single_quotes == 0)
			ex->inside_double_quotes = !ex->inside_double_quotes;
		if (str[ex->i] == '$' && ex->inside_single_quotes == 0
			&& ex->inside_double_quotes == 0)
			ft_dollar_case_1(ex, str, master);
		else if (str[ex->i] == '~' && (str[ex->i + 1] == ' '
				|| str[ex->i + 1] == '/' || !str[ex->i + 1])
			&& (!str[ex->i - 1] || str[ex->i - 1] == ' ')
			&& ex->inside_single_quotes == 0 && ex->inside_double_quotes == 0)
			ft_quote_case(ex, master);
		else
			ft_other_case(ex, str);
		ex->i++;
	}
	result = ex->new;
	free(ex);
	return (result);
}

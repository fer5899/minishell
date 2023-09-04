#include "../minishell.h"

// char    *trim_env_variable(char *str, char *esp)
// {
//     char    **tmp;
//     char    *new;
//     char    *new_free;
//     char    **tmp_free;

//     tmp = ft_split(str, ' ');
//     tmp_free = tmp;
//     new = ft_strdup("'");
//     while (*tmp)
//     {
//         new_free = new;
//         new = ft_strjoin(new, *tmp);
//         free(new_free);
//         new_free = new;
//         new = ft_strjoin(new, esp);
//         free(new_free);
//         free(*tmp);
//         tmp++;
//     }
//     new_free = new;
//     new = ft_strtrim(new, esp);
//     free(new_free);
//     new_free = new;
//     *tmp = ft_strdup("'");
//     new = ft_strjoin(new, *tmp);
//     free(new_free);
//     free(*tmp);
//     free(tmp_free);
//     free(esp);
//     return (new);
// }

char	*fix_double_quotes(char *str)
{
	int		i;
	int		num;
	char	*new;

	if (str == NULL)
		return (NULL);
	num = ft_count_char(str, '"');
	new = (char *)ft_calloc((ft_strlen(str) + num + 1), sizeof(char));
	i = 0;
	while (*str)
	{
		if (*str == '"')
		{
			new[i] = '\\';
			i++;
		}
		new[i] = *str;
		i++;
		str++;
	}
	return (new);

}

char	*substitude_env_variable_1(char *key, t_master *master)
{
	t_list	*list;
	t_env	*env;

	list = master->env_lst;
	if (str_equal(key, "?"))
		return (ft_itoa(master->exit_code));
	else if (str_equal(key, "$"))
		return (ft_strdup("$$"));
	//else if (str_equal(key, "~"))
	//	return (ft_strdup("$~"));
	else if (key == NULL)
		return (ft_strdup("$"));
	while (list)
	{
		env = (t_env *)list->content;
		if (str_equal(key, env->key))
		{
			// ft_printf("env->value: %s\n", env->value);
			return(fix_double_quotes(env->value));
			//return (ft_strdup(env->value));
		}
			//return (trim_env_variable(env->value, ft_strdup(" ")));
		list = list->next;
	}
	return (NULL);
}

t_exp_env	*inicialize_env_struct(void)
{
	t_exp_env	*ex;

	ex = (t_exp_env *)ft_calloc(1, sizeof(t_exp_env));
	ex->i = 0;
	ex->new = ft_strdup("");
	ex->env = NULL;
	ex->inside_single_quotes = 0;
	ex->inside_double_quotes = 0;
	return (ex);
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
	//else if (str[ex->i + 1] && str[ex->i + 1] == '~') //mirar si es necesario
	//{
	//	ex->key = ft_substr(str, ex->i + 1, 1);
	//	ex->j++;
	//}
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

void	ft_quote_case(t_exp_env *ex, t_master *master)
{
	ex->env = substitude_env_variable_1("HOME", master);
	ex->new_free = ex->new;
	ex->new = ft_strjoin(ex->new, ex->env);
	free(ex->new_free);
	free(ex->env);
}

void	ft_other_case(t_exp_env *ex, char *str)
{
	ex->tmp = ft_substr(str, ex->i, 1);
	ex->new_free = ex->new;
	ex->new = ft_strjoin(ex->new, ex->tmp);
	free(ex->new_free);
	free(ex->tmp);
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


// int main(void)
// {
// 	char	    *str;
// 	t_master    *master;

// 	master = inicialize_struct();
// 	str = expand_env_variables_first_pass("a$USER cat>ho''la<< Ñ¡ $~ $hi~ a$USER pe| o'no que mal' \"$USER\"pepe '$USER' $USER$USERa pepe$USERa$USER$USERa$USER $?hola hola$?hola pepe$USERa? pepe$USER? caca$?$USER caca$?a >> y lo |'pe'te", master);
// 	ft_printf("str: %s\n", str);
// 	free(str);
// 	ft_free_env_list(master);
// }

// int main(void)
// {
//     char	*str;

//     str = trim_env_variable("    hola    \"hola\"    hola      ", ft_strdup(" "));
//     ft_printf("str:%s\n", str);
//     free(str);
//     atexit(leaks);
// }

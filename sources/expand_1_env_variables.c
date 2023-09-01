#include "../minishell.h"

void leaks(void)
{
	system("leaks -q a.out");
}

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

char    *fix_double_quotes(char *str)
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
		return(ft_itoa(master->exit_code));
	else if (str_equal(key, "$"))
		return (ft_strdup("$$"));
	else if (str_equal(key, "~"))
		return (ft_strdup("$~"));
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

char	*expand_env_variables_first_pass(char *str, t_master *master)
{
	int		i;
	int		j;
	char	*key;
	int     inside_single_quotes;
	int     inside_double_quotes;
	char    *tmp;
	char    *new;
	char    *new_free;
	char    *env;

	i = 0;
	//ft_printf("str: %s\n", str);
	new = ft_strdup("");
	env = NULL;
	inside_single_quotes = 0;
	inside_double_quotes = 0;
	while (str[i])
	{
		if (str[i] == '\'' && inside_double_quotes == 0)
			inside_single_quotes = !inside_single_quotes;
		else if (str[i] == '"' && inside_single_quotes == 0)
			inside_double_quotes = !inside_double_quotes;
		if (str[i] == '$' && inside_single_quotes == 0 && inside_double_quotes == 0)
		{
			j = 0;
			while (str[i + j + 1] && str[i + j + 1] != ' '
				&& str[i + j + 1] != '\'' && str[i + j + 1] != '"'
				&& str[i + j + 1] != '$' && str[i + j + 1] != '?'
				&& str[i + j + 1] != '~' && str[i + j + 1] != '/'
				&& str[i + j + 1] != '|' && str[i + j + 1] != '>'
				&& str[i + j + 1] != '<')
				j++;
			if (str[i + 1] && str[i + 1] == '?')
			{
				key = ft_substr(str, i + 1, 1);
				j++;
			}
			else if (str[i + 1] && str[i + 1] == '$')
			{
				key = ft_substr(str, i + 1, 1);
				j++;
			}
			if (str[i + 1] && str[i + 1] == '~') //mirar si es necesario
			{
				key = ft_substr(str, i + 1, 1);
				j++;
			}
			else if ((str[i + 1] && (str[i + 1] == ' ' || str[i + 1] == '/')) || !str[i + 1])
				key = NULL;
			else
				key = ft_substr(str, i + 1, j);
			env = substitude_env_variable_1(key, master);
			if (env)
			{
				new_free = new;
				new = ft_strjoin(new, env);
				free(new_free);
				free(env);
				
			}
			free(key);
			i += j;
		}
		else if (str[i] == '~' && (str[i + 1] == ' ' 
				|| str[i + 1] == '/' || !str[i + 1]) 
				&& (!str[i - 1] || str[i - 1] == ' ') && inside_single_quotes == 0 && inside_double_quotes == 0)
		{
			env = substitude_env_variable_1("HOME", master); 
			new_free = new;
			new = ft_strjoin(new, env);
			free(new_free);
			free(env);
		}
		else
		{
			tmp = ft_substr(str, i, 1);
			new_free = new;
			new = ft_strjoin(new, tmp);
			free(new_free);
			free(tmp);
		}
		i++;
	}
	return (new);
}


// int main(void)
// {
// 	char	*str;
// 	t_master	*master;

// 	master = inicialize_struct();
// 	str = expand_env_variables_first_pass("$HOLA", master);
// 	ft_printf("str: %s\n", str);
// 	free(str);
// 	atexit(leaks);
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

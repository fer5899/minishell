#include "../minishell.h"

char	*substitude_env_variable_2(char *key, t_master *master)
{
	t_list	*list;
	t_env	*env;

	list = master->env_lst;
    if (str_equal(key, "?"))
        return(ft_itoa(master->exit_code));
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

char	*expand_env_variables_second_pass(char *str, t_master *master)
{
	int		i;
	int		j;
	char	*key;
    char    *tmp;
    char    *new;
    char    *new_free;
    char    *env;

    i = 0;
    //ft_printf("str: %s\n", str);
    new = ft_strdup("");
    env = NULL;
	while (str[i])
	{
        if (str[i] == '$')
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
            if (str[i + 1] && str[i + 1] == '~')
            {
				key = ft_substr(str, i + 1, 1);
                j++;
            }
            else if ((str[i + 1] && (str[i + 1] == ' ' || str[i + 1] == '/' || str[i + 1] == '\'' || str[i + 1] == '|' || str[i + 1] == '>' || str[i + 1] == '<')) || !str[i + 1])
                key = NULL;
			else
				key = ft_substr(str, i + 1, j);
			env = substitude_env_variable_2(key, master);
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
// 
// 	char	*str;
// 	t_master	*master;

// 	master = inicialize_struct();
// 	str = expand_env_variables_second_pass("$'\n'", master);
//     ft_printf("str: %s\n", str);
//     free(str);
//     atexit(leaks);
//     ft_free_env_list(master);
// }

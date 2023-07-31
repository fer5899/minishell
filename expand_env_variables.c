#include "minishell.h"

char    *substitude_value_for_key(char *str, char *value)
{
    char    *new_str;
    char    *tmp;
    int     i;

    i = 0;
    while(str[i] && str[i] != '$')
        i++;
    tmp = ft_substr(str, 0, i);
    new_str = ft_strjoin(tmp, value);
    //printf("str: %s\n", str);
    return (new_str);
}


char    *substitude_env_variable(char *key, char *str, t_master *master)
{
	t_list	*list;
	t_env	*env;
    int     found;

    found = 0;
	list = master->env_lst;
	while (list)
	{
		env = (t_env *)list->content;
		if (ft_strncmp(key, env->key, ft_strlen(env->key)) == 0)
		{
			str = substitude_value_for_key(str, env->value);
            found = 1;
			break ;
		}
		list = list->next;
	}
    if (found == 0)
        str = substitude_value_for_key(str, '\0');
    return (str);
}


char    *expand_env_variables(char *str, t_master *master)
{
    int		i;
    int		j;
    char	*key;

    i = 0;
    while (str[i])
    {
        if (str[i] == '$')
        {
            j = 0;
            while (str[i + j + 1] && str[i + j + 1] != ' ' && str[i + j + 1] != '\'' && str[i + j + 1] != '"' && str[i + j + 1] != '$')
                j++;
            key = ft_substr(str, i + 1, j);
            //printf("key: %s - str: %s\n", key, str);
            str = substitude_env_variable(key, str, master);
            i += j;
        }
        i++;
    }
    return (str);
}

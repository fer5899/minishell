#include "../minishell.h"

char	*substitude_value_for_key(char *str, char *value, int key_len)
{
	char	*new_str;
	char	*tmp;
	char	*tmp2;
	int		i;
	int		j;

	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	tmp = ft_substr(str, 0, i);
	j = i + key_len + 1;
	while (str[j])
		j++;
	tmp2 = ft_substr(str, i + key_len + 1, j);
	new_str = ft_strjoin(tmp, value);
	new_str = ft_strjoin(new_str, tmp2);
	return (new_str);
}


char	*substitude_env_variable(char *key, char *str, t_master *master)
{
	t_list	*list;
	t_env	*env;
	int		found;

	found = 0;
	list = master->env_lst;
	while (list)
	{
		env = (t_env *)list->content;
		if (str_equal(key, env->key) || str_equal(key, "?"))
		{
			if (str_equal(key, "?"))
				str = ft_itoa(master->exit_code);
			else
				str = substitude_value_for_key(str, env->value, ft_strlen(key));
			found = 1;
			break ;
		}
		list = list->next;
	}
	if (found == 0)
		str = substitude_value_for_key(str, "\0", 0);
	return (str);
}


char	*expand_env_variables(char *str, t_master *master)
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
			while (str[i + j + 1] && str[i + j + 1] != ' '
				&& str[i + j + 1] != '\'' && str[i + j + 1] != '"'
				&& str[i + j + 1] != '$')
				j++;
			key = ft_substr(str, i + 1, j);
			str = substitude_env_variable(key, str, master);
			i += j;
		}
		i++;
	}
	return (str);
}

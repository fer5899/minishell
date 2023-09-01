#include "../minishell.h"

int	is_there_a_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*substitude_value_for_key(char *str, char *value, int key_len)
{
	char	*tmp;
	char	*tmp2;
	int		i;
	int		j;

	i = 0;
	//ft_printf("str: %s ,value: %s, key_len: %d, value_len: %d\n", str, value, key_len, value_len);
	//ft_printf("str: %s\n", str);
	while (str[i] && str[i] != '$')
		i++;
	tmp = ft_substr(str, 0, i);
	//ft_printf("tmp: %s\n", tmp);
	j = i + key_len + 1;
	//ft_printf("j: %d\n", j);
	//ft_printf("value: %s\n", value);
	//ft_printf("tmp2 = tmp: %s , value: %s\n", tmp, value);
	if (value == NULL)
		tmp2 = ft_strdup(tmp);
	else
		tmp2 = ft_strjoin(tmp, value);
	free(tmp);
	//ft_printf("suma: %s\n", tmp2);
	//ft_printf("tmp = tmp2: %s , str + j: %s, str: %s, j: %d\n", tmp2, str + j, str, j);
	tmp = ft_strjoin(tmp2, (str + j));
	//ft_printf("final: %s\n", (str + j));
	//ft_printf("->new: %s\n------\n", tmp);
	free(tmp2);
	//ft_printf("str: %s\n", str);
	free(str); //------> aqui esta el problema de valgrind
	return (tmp);
}

//int	where_is_char(char *str, char c)
//{
//	int	i;
//
//	i = 0;
//	while (str[i])
//	{
//		if (str[i] == c)
//			return (i);
//		i++;
//	}
//	return (-1);
//}


char	*substitude_env_variable(char *key, char *str, t_master *master)
{
	t_list	*list;
	t_env	*env;
	int		found;
	char	*tmp;

	found = 0;
	list = master->env_lst;
	//ft_printf("key: %s\n", key);
	while (list)
	{
		env = (t_env *)list->content;
		if (str_equal(key, env->key) || str_equal(key, "?"))
		{
			if (key[0] == '?')
			{
				tmp = ft_itoa(master->exit_code);
				//tmp = ft_itoa(127);
				//ft_printf("itoa: %s\n", tmp);
				str = substitude_value_for_key(str, tmp, ft_strlen("?"));
				free(tmp);
			}
			//else if (is_there_a_char(key, '?') == 1)
			//{
			//	ft_printf(("HHOOLLAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n"));
			//	i = where_is_char(key, '?');
			//	tmp = ft_substr(key, 0, i);
			//	ft_printf("tmp?: %s\n", tmp);
			//	ft_printf("env?: %s\n", env->key);
			//	if (str_equal(tmp, env->key))
			//	{
			//		str = substitude_value_for_key(str, tmp, ft_strlen(tmp), ft_strlen(tmp));
			//	}
			//	free(tmp);
			//}
			else
				str = substitude_value_for_key(str, env->value, ft_strlen(key));
			found = 1;
			break ;
		}
		list = list->next;
	}
	if (found == 0)
		str = substitude_value_for_key(str, NULL, ft_strlen(key));
	free(key);
	return (str);
}


char	*expand_env_variables(char *str, t_master *master)
{
	int		i;
	int		j;
	char	*key;

	//ft_printf("STR: %s\n", str);
	while (is_there_a_char(str, '$') == 1)
	{
		i = 0;
		while (str[i])
		{
			if (str[i] == '$')
			{
				j = 0;
				while (str[i + j + 1] && str[i + j + 1] != ' '
					&& str[i + j + 1] != '\'' && str[i + j + 1] != '"'
					&& str[i + j + 1] != '$' && str[i + j + 1] != '?')
					j++;
				if (str[i + 1] && str[i + 1] == '?')
					key = ft_substr(str, i + 1, 1);
				else
					key = ft_substr(str, i + 1, j);
				//ft_printf("KEYYYY: %s\n", key);
				str = substitude_env_variable(key, str, master);
				break ;
				i += j;
			}
			i++;
		}
	}
	return (str);
}

#include "../minishell.h"

int	str_equal(char *str1, char *str2)
{
	if (ft_strlen(str1) != ft_strlen(str2))
		return (0);
	if (ft_strncmp(str1, str2, ft_strlen(str1)) == 0)
		return (1);
	return (0);
}

t_env	*init_env_data(char *key, char *value)
{
	t_env	*data;

	data = (t_env *) ft_calloc(1, sizeof(t_env));
	data->key = key;
	data->value = value;
	return (data);
}

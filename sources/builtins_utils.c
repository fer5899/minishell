#include "../minishell.h"

void	print_env(void *nd)
{
	if (((t_env *) nd)->value != NULL)
		ft_printf("%s=%s\n", ((t_env *) nd)->key, ((t_env *) nd)->value);
}

void	print_export(void *nd)
{
	if (((t_env *) nd)->value != NULL)
		ft_printf("declare -x %s=\"%s\"\n",
			((t_env *) nd)->key, ((t_env *) nd)->value);
	else
		ft_printf("declare -x %s\n", ((t_env *) nd)->key);
}

int	is_valid_env_key(char *key)
{
	if (key == NULL)
		return (0);
	if (ft_isdigit(*key))
		return (0);
	while (*key != '\0')
	{
		if (!(ft_isalnum(*key) || *key == '_'))
			return (0);
		key++;
	}
	return (1);
}

char	*get_key(char *arg)
{
	char	*equal;
	int		diff;

	equal = ft_strchr(arg, '=');
	if (equal == NULL)
		return (ft_strdup(arg));
	diff = equal - arg;
	if (diff == 0)
		return (NULL);
	return (ft_substr(arg, 0, (size_t) diff));
}

char	*get_value(char *arg)
{
	char	*equal;
	int		diff;

	equal = ft_strchr(arg, '=');
	if (equal == NULL)
		return (NULL);
	diff = equal - arg;
	return (ft_substr(arg, diff + 1, ft_strlen(arg)));
}

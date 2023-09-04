
#include "../minishell.h"

static char	*fill_array(char *s, char c)
{
	int		i;
	int		length;
	char	*str;

	i = 0;
	length = 0;
	while (s[length] != c && s[length])
		length++;
	str = (char *)malloc((length + 1) * sizeof(char));
	if (!str)
		return (0);
	while (s[i] != c && s[i])
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

static char	*fill_array_two(char *s)
{
	int		i;
	int		length;
	char	*str;

	i = 0;
	length = 0;
	while (s[length])
		length++;
	str = (char *)malloc((length + 1) * sizeof(char));
	if (!str)
		return (0);
	while (s[i])
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	**ft_free_split(char **str, int i)
{
	while (i >= 0)
	{
		free(str[i]);
		str[i] = 0;
		i--;
	}
	free(str);
	str = 0;
	return (0);
}

char	**ft_split_env(char const *s, char c)
{
	char	**str;
	int		i;

	i = 0;
	str = (char **)malloc(3 * sizeof(char *));
	if (!str)
		return (0);
	str[i] = fill_array((char *)s, c);
	if (!str[i])
		return (ft_free_split(str, i));
	i++;
	while (*s != c && *s)
		s++;
	s++;
	str[i] = fill_array_two((char *)s);
	if (!str[i])
		return (ft_free_split(str, i));
	i++;
	str[i] = 0;
	return (str);
}

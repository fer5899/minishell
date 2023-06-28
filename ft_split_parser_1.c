
#include <stdio.h>
#include <stdlib.h>

static int	word_count(char *str)
{
	int		count;
	int		inside_quotes;
	int		is_word;
	char	quote_type;
	char	prev_char;

	count = 0;
	inside_quotes = 0;
	is_word = 0;
	quote_type = '\0';
	prev_char = '\0';
	while (*str)
	{
		if ((*str == '"' || *str == '\'') && !inside_quotes)
		{
			inside_quotes = 1;
			quote_type = *str;
			count++;
		}
		else if (*str == quote_type && inside_quotes)
		{
			inside_quotes = 0;
			quote_type = '\0';
		}
		else if (*str == ' ' && !inside_quotes)
			is_word = 0;
		else
		{
			if (is_word == 0 && !inside_quotes)
			{
				count++;
				is_word = 1;
			}
		}
		prev_char = *str;
		str++;
	}
	//printf("c:%d - iq:%d - iw:%d - qt:%c \n", count, inside_quotes, is_word, quote_type);
	if (inside_quotes == 1 && prev_char != quote_type)
		count++;
	return (count);
}

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

char	**ft_split_parser_1(char *s)
{
	char	**str;
    //int     count;
    //char	char_type;

	printf("%d\n", word_count(s));
    //count = word_count(s);
    //char_type = '\0';
	//str = (char **)malloc((count + 1) * sizeof(char *));
	//if (!str)
	//	return (0);
	//while (count > 0)
	//{
	//	if (*s == ' ' && *s)
	//		s++;
	//	else if (*s != ' ' && *s)
	//	{
	//		if (*s != '\'' || *s != '"')
    //            char_type = *s;
    //        else
    //            char_type = ' ';
    //        *str = fill_array(s, char_type);
	//		str++;
    //        count--;
    //        s++;
    //        while (*s != char_type && *s)
	//		    s++;
	//	}
	//}
    //str++;
	//str = 0;
	return (str);
}

int main(void)
{
	char **str;
	int i;
	
	i = 0;
	str = ft_split_parser_1("hola que tal");
	//while (str[i])
	//{
	//	printf("%s\n", str[i]);
	//	i++;
	//}

	//"hola que '' tal \"'estas. las''\" asdj 'cac c'    " - 7
	//"hola que '' tal \"'estas. las''\" asdj 'cac\"\" c'    'hol\"' pepe  '\"' " - 10
	//"ho 'la que '''\"\"'" - 5
	//"ho 'la que '''\"''''''\"''" - 5

    //hola que '' tal \"'estas. las''\" asdj 'cacc'   'hol\"' pepe
}

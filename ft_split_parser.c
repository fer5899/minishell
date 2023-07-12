
#include "minishell.h"

void	check_for_pipes(t_split_param *sp)
{
	if (*(sp->s) == '|')
	{
		if (sp->pipe == 0)
			sp->is_word = 0;
			//sp->count_2++;
		sp->pipe++;
		if (sp->pipe > 1)
		{
			//printf("%s\n", "Pipe error");
			sp->error = 1;
		}
		sp->after_sep = 1;
	}
	else
		sp->pipe = 0;
}

void	check_for_redirections(t_split_param *sp)
{
	if (*(sp->s) == '<')
	{
		if (sp->red_l == 0)
			sp->is_word = 0;
			//sp->count_2++;
		sp->red_l++;
		if (sp->red_l > 2)
		{
			//printf("%s\n", "Redirection error");
			sp->error = 1;
		}
		sp->after_sep = 1;
	}
	else
		sp->red_l = 0;
	if (*(sp->s) == '>')
	{
		if (sp->red_r == 0)
			sp->is_word = 0;
			//sp->count_2++;
		sp->red_r++;
		if (sp->red_r > 2)
		{
			//printf("%s\n", "Redirection error");
			sp->error = 1;
		}
		sp->after_sep = 1;
	}
	else
		sp->red_r = 0;
}

static void	word_count_options(t_split_param *sp)
{
	if ((*(sp->s) == '"' || *(sp->s) == '\'') && !sp->inside_quotes)
	{
		sp->inside_quotes = 1;
		sp->quote_type = *(sp->s);
		sp->count_2++;
	}
	else if (*(sp->s) == sp->quote_type && sp->inside_quotes)
	{
		sp->inside_quotes = 0;
		sp->quote_type = '\0';
	}
	else if ((*(sp->s) == ' ' && !sp->inside_quotes))
		sp->is_word = 0;
	else
	{
		check_for_redirections(sp);
		check_for_pipes(sp);
		if (sp->after_sep == 1 && *(sp->s) != '>' && *(sp->s) != '<' && *(sp->s) != '|')
		{
			sp->is_word = 0;
			sp->after_sep = 0;
		}
		if ((sp->is_word == 0 && !sp->inside_quotes))
		{
			sp->count_2++;
			sp->is_word = 1;
		}
	}
}

static int	word_count(t_split_param *sp)
{
	sp->count_2 = 0;
	sp->inside_quotes = 0;
	sp->is_word = 0;
	sp->quote_type = '\0';
	sp->red_l = 0;
	sp->red_r = 0;
	sp->pipe = 0;
	sp->after_sep = 0;
	while (*(sp->s))
	{
		word_count_options(sp);
		//printf("%c --> c:%d - iq:%d - iw:%d - qt:%c - rl:%d - rf:%d - p:%d\n",*(sp->s), sp->count_2, sp->inside_quotes, sp->is_word, sp->quote_type, sp->red_l, sp->red_r, sp->pipe);
		sp->s++;
	}
	if (sp->inside_quotes == 1)
	{
		//printf("%s\n", "Quote error");
		sp->error = 1;
	}
	return (sp->count_2);
}

static char	*fill_array(char *s, t_split_param *sp)
{
	int		i;
	int		length;
	char	*str;

	i = 0;
	length = 0;
	if (sp->char_type == ' ')
	{
		if (*s == '>' || *s == '<' || *s == '|')
		{
			while (s[length] == sp->sep_type && s[length])
				length++;
		}
		else
		{
			while ((s[length] != ' ' && s[length] != '>' && s[length] != '<' && s[length] != '|' && s[length] != '"' && s[length] != '\'') && s[length])
				length++;
		}
	}
	else if (sp->char_type == '\'' || sp->char_type == '"')
	{
		while (s[length+1] != sp->char_type  && s[length])
			length++;
	}
	//printf("%d\n", length);
	str = (char *)malloc((length + 1) * sizeof(char));
	if (!str)
		return (0);
	if (sp->char_type == ' ')
	{
		if (*s == '>' || *s == '<' || *s == '|')
			while ((s[i] == sp->sep_type) && s[i])
			{
				str[i] = s[i];
				i++;
			}
		else
			while ((s[i] != sp->char_type && s[i] != ' ' && s[i] != '>' && s[i] != '<' && s[i] != '|' && s[i] != '"' && s[i] != '\'') && s[i])
			{
				str[i] = s[i];
				i++;
			}
	}
	else if (sp->char_type == '\'' || sp->char_type == '"')
	{
		while (s[i+1] != sp->char_type && s[i])
		{
			str[i] = s[i+1];
			i++;
		}
	}
	str[i] = '\0';
	return (str);
}

t_split_param	*ft_split_param_initialize(char *s)
{
	t_split_param	*sp;

	sp = (t_split_param *)malloc(sizeof(t_split_param));
	if (!sp)
		exit (1);
	sp->s = s;
	sp->error = 0;
	sp->count_1 = word_count(sp);
	sp->char_type = '\0';
	sp->sep_type = '\0';
	sp->str_1 = (char **)malloc((sp->count_1 + 1) * sizeof(char *));
	if (!sp->str_1)
		exit (1);
	return (sp);
}

void	ft_split_parser_check_separators(t_split_param *sp, char *s)
{
	if (*s == '\'' || *s == '"')
	{
		sp->char_type = *s;
		s++;
	}
	else
		sp->char_type = ' ';
	if (*s == '<')
		sp->sep_type = '<';
	else if (*s == '>')
		sp->sep_type = '>';
	else if (*s == '|')
		sp->sep_type = '|';
	else
		sp->sep_type = '\0';
}

t_split	*ft_split_parser(char *s)
{
	t_split_param	*sp;
	t_split			*split;
	t_split			*result;

	sp = ft_split_param_initialize(s);
	split = (t_split *)malloc((sp->count_1 + 1) * sizeof(t_split));
	if (!split)
		exit (1);
	result = split;
	while (sp->count_1 > 0)
	{
		while (*s == ' ' && *s)
			s++;
		ft_split_parser_check_separators(sp, s);
		//printf("s:%c -- sep:%c -- char:%c -- ", *s, sp->sep_type, sp->char_type);
		*(sp->str_1) = fill_array(s, sp);
		split->str = *(sp->str_1);
		//printf("%s\n", (sp->str_1));
		split->char_type = sp->char_type;
		sp->str_1++;
		split++;
		sp->count_1--;
		if ((sp->char_type == '\'' || sp->char_type == '"') && *s)
		{
			s++;
			while (*s != sp->char_type && *s)
				s++;
			if (*s == '\'' || *s == '"')
				s++;
		}
		else if (sp->char_type == ' ' && *s)
		{
			if ((*s == '<' || *s == '>' || *s == '|') && *s)
			{
				while ((*s == sp->sep_type) && *s)
					s++;
			}
			else
			{
				while ((*s != ' ' && *s != '<' && *s != '>' && *s != '|' && *s != '"' && *s != '\'' ) && *s)
					s++;
			}
		}
	}
	//printf("----------------------------\n");
	*(sp->str_1) = 0;
	split->str = *(sp->str_1);
	result->error = sp->error;
	return (result);
}

//int main(void)
//{
//	t_split	*split;
//	
//	split = ft_split_parser("hola>>||que");
//	while (split->str)
//	{
//		printf("%s -- %c\n", split->str, split->char_type);
//		split++;
//	}
//
//	//"hola que '' tal \"'estas. las''\" asdj 'cac c'    " - 7
//	//"hola que '' tal \"'estas. las''\" asdj 'cac\"\" c'    'hol\"' pepe  '\"' " - 10
//	//"ho 'la que '''\"\"'" - 4 Quote Error
//	//"ho 'la que '''\"''''''\"''" - 5
//
//    //hola que '' tal \"'estas. las''\" asdj 'cacc'   'hol\"' pepe
//}
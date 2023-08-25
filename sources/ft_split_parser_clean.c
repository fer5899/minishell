
#include "../minishell.h"

void	ft_check_for_pipes(t_split_param *sp)
{
	if (*(sp->s) == '|')
	{
		if (sp->pipe == 0)
		{
			sp->count_2++;
			sp->is_word = 1;
		}
		sp->pipe++;
		if (sp->pipe > 1)
			sp->error = 1;
		sp->after_sep = 1;
	}
	else
		sp->pipe = 0;
}

void	ft_check_for_redirections(t_split_param *sp)
{
	if (*(sp->s) == '<')
	{
		if (sp->red_l == 0)
		{
			sp->count_2++;
			sp->is_word = 1;
		}
		sp->red_l++;
		if (sp->red_l > 2)
			sp->error = 1;
		sp->after_sep = 1;
	}
	else
		sp->red_l = 0;
	if (*(sp->s) == '>')
	{
		if (sp->red_r == 0)
		{
			sp->count_2++;
			sp->is_word = 1;
		}
		sp->red_r++;
		if (sp->red_r > 2)
			sp->error = 1;
		sp->after_sep = 1;
	}
	else
		sp->red_r = 0;
}

void	normal_character_check(t_split_param *sp)
{
	if (sp->after_sep == 1 && *(sp->s) != '>' 
		&& *(sp->s) != '<' && *(sp->s) != '|')
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

static void	word_count_options(t_split_param *sp)
{
	ft_check_for_redirections(sp);
	ft_check_for_pipes(sp);
	if ((*(sp->s) == '"' || *(sp->s) == '\'') && !sp->inside_quotes)
	{
		sp->inside_quotes = 1;
		sp->quote_type = *(sp->s);
		if (sp->after_sep == 1)
			sp->after_sep = 0;
		sp->count_2++;
		sp->is_word = 1;
	}
	else if (*(sp->s) == sp->quote_type && sp->inside_quotes)
	{
		sp->inside_quotes = 0 ;
		sp->quote_type = '\0';
		sp->is_word = 0;
	}
	else if ((*(sp->s) == ' ' && !sp->inside_quotes))
		sp->is_word = 0;
	else
		normal_character_check(sp);	//not a special character
}

//printf("%c --> c:%d - iq:%d - iw:%d - qt:%c - rl:%d - rf:%d - p:%d\n",*(sp->s), sp->count_2, sp->inside_quotes, sp->is_word, sp->quote_type, sp->red_l, sp->red_r, sp->pipe);

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
		//printf("%c --> c:%d - iq:%d - iw:%d - qt:%c - rl:%d - rf:%d - p:%d - as:%d\n",*(sp->s), sp->count_2, sp->inside_quotes, sp->is_word, sp->quote_type, sp->red_l, sp->red_r, sp->pipe, sp->after_sep);
		sp->s++;
	}
	if (sp->inside_quotes == 1)
		sp->error = 1;
	return (sp->count_2);
}

int	ft_fill_array_lenght(char *s, t_split_param *sp)
{
	int		length;

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
			while ((s[length] != ' ' && s[length] != '>' && s[length] != '<' 
					&& s[length] != '|' && s[length] != '"'
					&& s[length] != '\'') && s[length])
				length++;
		}
	}
	else if (sp->char_type == '\'' || sp->char_type == '"')
	{
		while (s[length + 1] != sp->char_type && s[length])
			length++;
	}
	return (length);
}

int	ft_fill_array_extra(char *s, t_split_param *sp, char *str)
{
	int	i;

	i = 0;
	if (s[i] == '>' || s[i] == '<' || s[i] == '|')
	{
		sp->join_arg = 0;
		while ((s[i] == sp->sep_type) && s[i])
		{
			str[i] = s[i];
			i++;
		}
	}
	else
	{
		while ((s[i] != sp->char_type && s[i] != ' ' && s[i] != '>' 
				&& s[i] != '<' && s[i] != '|' && s[i] != '"' 
				&& s[i] != '\'') && s[i])
		{
			str[i] = s[i];
			i++;
		}
	}
	return (i);
}

static char	*fill_array(char *s, t_split_param *sp)
{
	int		i;
	int		length;
	char	*str;

	i = 0;
	length = ft_fill_array_lenght(s, sp);
	str = (char *)ft_calloc((length + 1), sizeof(char));
	if (!str)
		return (0);
	if (sp->char_type == ' ')
		i = ft_fill_array_extra(s, sp, str);
	else if (sp->char_type == '\'' || sp->char_type == '"')
	{
		while (s[i + 1] != sp->char_type && s[i])
		{
			str[i] = s[i + 1];
			i++;
		}
	}
	str[i] = '\0';
	return (str);
}

t_split_param	*ft_split_param_initialize(char *s)
{
	t_split_param	*sp;

	sp = (t_split_param *)ft_calloc(1, sizeof(t_split_param));
	if (!sp)
		exit (1);
	sp->s = s;
	sp->error = 0;
	sp->count_1 = word_count(sp);
	sp->char_type = '\0';
	sp->sep_type = '\0';
	sp->join_arg = 0;
	sp->str_1 = (char **)ft_calloc((sp->count_1 + 1), sizeof(char *));
	if (!sp->str_1)
		exit (1);
	return (sp);
}

void	ft_check_for_separators(t_split_param *sp, char *s)
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

t_split_total	*ft_inizialize_split_total(char *s)
{
	t_split_total	*st;

	st = (t_split_total *)ft_calloc(1, sizeof(t_split_total));
	st->sp = ft_split_param_initialize(s);
	st->split = (t_split *)ft_calloc((st->sp->count_1 + 1), sizeof(t_split));
	if (!st->split)
		exit (1);
	st->str_1 = st->sp->str_1;
	return (st);
}

char	*ft_skip_filled_word(t_split_total *st, char *s)
{
	if ((st->sp->char_type == '\'' || st->sp->char_type == '"') && *s)
	{
		s++;
		while (*s != st->sp->char_type && *s)
			s++;
		if (*s == '\'' || *s == '"')
			s++;
	}
	else if (st->sp->char_type == ' ' && *s)
	{
		if ((*s == '<' || *s == '>' || *s == '|') && *s)
		{
			while ((*s == st->sp->sep_type) && *s)
				s++;
		}
		else
		{
			while ((*s != ' ' && *s != '<' && *s != '>' 
					&& *s != '|' && *s != '"' && *s != '\'' ) && *s)
				s++;
		}
	}
	return (s);
}

void	ft_modify_count_and_sp(t_split_total *st)
{
	st->split->str = *(st->sp->str_1);
	st->split->error = 0;
	st->split->char_type = st->sp->char_type;
	st->split->join_arg = st->sp->join_arg;
	st->sp->str_1++;
	st->split++;
	st->sp->count_1--;
}

void	ft_split_all_words(char *s, t_split_total *st)
{
	while (*s)
	{
		if (*s == ' ' && *s)
		{
			st->sp->join_arg = 0;
			while (*s == ' ' && *s)
				s++;
		}
		else
			st->sp->join_arg = 1;
		ft_check_for_separators(st->sp, s);
		*(st->sp->str_1) = fill_array(s, st->sp);
		ft_modify_count_and_sp(st);
		s = ft_skip_filled_word(st, s);
		st->sp->join_arg = 1;
	}
	*(st->sp->str_1) = 0;
}

int	ft_count_args(t_split *split, int count)
{
	while (split->str)
	{
		if (*split->str != '>' && *split->str != '<' && *split->str != '|'
			&& (split + 1)->join_arg && (split + 1)->join_arg == 1)
			count--;
		split++;
	}
	return (count);
}

void	aux_join_args(t_split *split, t_split *result, t_master *master)
{
	char	*str_free;

	str_free = result->str;
	(split + 1)->str = expand_env_variables((split + 1)->str, master);
	result->str = ft_strjoin(result->str, (split + 1)->str);
	free(str_free);
	free((split + 1)->str);
	if (split->char_type == '\'' || (split + 1)->char_type == '\'' 
		|| split->char_type == '"' || (split + 1)->char_type == '"')
		result->char_type = '\'';
	else
		result->char_type = split->char_type;
}

void	ft_go_through_args(t_split *split, t_split *result, int count, t_master *master)
{
	while (count >= 0)
	{
		*result = *split;
		if (result->char_type != '\'')
			result->str = expand_env_variables(result->str , master);
		if (*split->str != '>' && *split->str != '<' && *split->str != '|'
			&& (split + 1) && (split + 1)->join_arg == 1)
		{
			while ((split + 1) && (split + 1)->join_arg == 1)
			{
				aux_join_args(split, result, master);
				count--;
				split++;
			}
			result++;
		}
		else
		{
			count--;
			result++;
		}	
		split++;
	}
}

t_split *ft_expand_and_join_args(t_split *split, int count, t_master *master)
{
	t_split *result;
	t_split *result_last;
	t_split *split_free;

	split_free = split;
	count = ft_count_args(split, count);
	result = (t_split *)ft_calloc((count + 1), sizeof(t_split));
	if (!result)
		exit (1);
	result_last = result;
	ft_go_through_args(split, result, count, master);
	free(split_free);
	return (result_last);
}

t_split	*ft_split_parser(char *s, t_master *master)
{
	t_split_total	*st;
	t_split			*result;
	int				count;

	st = ft_inizialize_split_total(s);
	result = st->split;
	count = st->sp->count_1;
	//ft_printf("count = %d\n", st->sp->count_1);

	ft_split_all_words(s, st);

	st->split->str = *(st->sp->str_1);
	if (st->sp->error == 0)
		result = ft_expand_and_join_args(result, count, master);
	result->error = st->sp->error;
	free(st->sp);
	free(st->str_1);
	free(st);
	return (result);
}

//printf("s:%c -- sep:%c -- char:%c -- ", *s, sp->sep_type, sp->char_type);

int main(void)
{
	t_split	*split;
	t_split	*split_free;
	char	*str;
	t_master	*master;

	master = inicialize_struct();
	int i = 0;
	//split = ft_split_parser("ho>akds ho>'ad' a'dsjf'jdf");
	split = ft_split_parser("cat>ho''la<< pe| o'no que mal' \"$USER\"pepe '$USER' $USER$USERa pepe$USERa$USER$USERa$USER $?hola hola$?hola pepe$USERa? pepe$USER? caca$?$USER caca$?a >> y lo |'pe'te", master);
	char *bash[] = {"cat", ">" ,"hola", "<<", "pe", "|", "ono que mal", "alvgomezpepe", "$USER", "alvgomez", "pepealvgomezalvgomez", "0hola", "hola0hola", "pepe?", "pepealvgomez?", "caca0alvgomez", "caca0a", ">>", "y", "lo", "|", "pete", NULL};
	char *nota;
	split_free = split;
	while (split->str)
	{
		if (split->error)
			printf("error: %d\n", split->error);
		if (str_equal(split->str, bash[i]))
			nota = "OK";
		else
			nota = "MAAL";
		printf("%s -- %c -- %s --> %s\n", split->str, split->char_type, bash[i], nota);
		str = split->str;
		free(str);
		split++;
		i++;
	}
	free(split_free);
	ft_free_env_list(master);

	//"hola que '' tal \"'estas. las''\" asdj 'cac c'    " - 7
	//"hola que '' tal \"'estas. las''\" asdj 'cac\"\" c'    'hol\"' pepe  '\"' " - 10
	//"hola que '' tal \"'estas. las''\" asdj 'cac\"\" c'  'fin'al al'fin' c'o'c  'hol\"' pepe  '\"' " - 13
	//"ho 'la que '''\"\"'" - 4 Quote Error
	//"ho 'la que '''\"''''''\"''" - 5

    //hola que '' tal \"'estas. las''\" asdj 'cacc'   'hol\"' pepe
}
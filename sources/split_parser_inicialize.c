#include "../minishell.h"

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

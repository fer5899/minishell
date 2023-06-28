
#ifndef MINISHELL_H
# define MINISHELL_H

# include "Libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_env
{
	char	*key;
	char	*value;
}				t_env;

typedef struct s_data
{
	int		type;
	char	*str;
}				t_data;

typedef struct s_master
{
	t_list	*env_lst;
	t_list	*parsed_lst;

}				t_master;

//Main

//Split env
char	    **ft_split_env(char const *s, char c);

//Env variables minishell
t_master	*inicialize_struct(void);
char	    *get_env_variable(char *key, t_master *master);
void	    print_env_list(t_master *master);

//Free minishell
void	    ft_free_env(void *env);
void	    ft_free_env_list(t_master *master);
void	    ft_free_data(void *data);
void	    ft_free_data_list(t_master *master);

//Split parser 1 - the first parser for single and double quotes

#endif
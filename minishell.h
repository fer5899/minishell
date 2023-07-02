
#ifndef MINISHELL_H
# define MINISHELL_H

# include "Libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>

# define STD_IN_OUT 1000
# define MAX_PIPES 1001

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
	int		fds[MAX_PIPES][2];
	pid_t	pids[MAX_PIPES];
}				t_master;

enum e_prs_type
{
	prog_name,
	prog_arg,
	in_redir,
	heredoc,
	heredoc_q,
	out_red,
	out_red_app,
	pipe_
};

enum e_pipe_ends
{
	rd,
	wr
};

//Main

#endif
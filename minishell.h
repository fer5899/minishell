
#ifndef MINISHELL_H
# define MINISHELL_H

# include "Libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

# define MAX_PIPES 1000
# define STD_IN_OUT MAX_PIPES
# define TEMP_PATH "heredoc.tmp"

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
	int		fds[MAX_PIPES + 1][2];
	pid_t	pids[MAX_PIPES + 1];
	int		n_pipes;
}				t_master;

enum e_prs_type
{
	prog_name_,
	prog_arg_,
	in_redir_,
	heredoc_,
	heredoc_q_,
	out_red_,
	out_red_app_,
	pipe_
};

enum e_pipe_ends
{
	rd,
	wr
};

//Main

void	fatal_error(void);
void	redirection_error(char *str);
void	init_pipes(t_master *data);
void	executor(t_master *data);
void	heredoc(char *delim, int type, t_master *data);
void	set_redirection(int type, char *str, t_master *data);
void	set_all_redirections(t_master *data, t_list *lst);
void	set_pipe_redirection(t_master *data, int cmd_idx);
char	*expand_env_vars(char *str, t_master *data); // sustituir por función de álvaro
void	find_next_cmd(t_list **lst);
void	init_pipes(t_master *data);
void	count_pipes(t_master *data);
char	**get_prog_args(t_list *lst);

// TESTING
void	print_lst(void *nd);

#endif
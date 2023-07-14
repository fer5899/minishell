
#ifndef MINISHELL_H
# define MINISHELL_H

# include "Libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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
	int		cmd_idx;
	int		heredoc_idx;
	int		nargs;
	int		exit_code;
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
	pipe_,
};

enum e_pipe_ends
{
	rd,
	wr,
};

//Main

void	fatal_error(void);
void	file_error(char *str, int code);
void	init_pipes(t_master *data);
void	executor(t_master *data);
void	heredoc(t_master *data);
void	set_redirection(int type, char *str, t_master *data);
void	set_all_redirections(t_master *data, t_list *lst);
void	set_pipe_redirection(t_master *data);
char	*expand_env_vars(char *str, t_master *data); // sustituir por función de álvaro
void	find_next_cmd(t_list **lst);
void	init_pipes(t_master *data);
void	count_pipes(t_master *data);
char	**get_pargs(t_master *data, t_list *lst);
char	*get_pname(t_list *lst);
char	**get_path_arr(t_master *data);
char	*get_prog_path(t_list *lst, char **path_arr);
char	**get_env_arr(t_master *data);
char	*get_tmp_path(t_master *data);
void	close_fds(t_master *data);
void	input_heredoc(char *delim, int type, t_master *data);
void	get_all_input_heredoc(t_master *data);
void	catch_exit_code(t_master *data);
void	pwd(void);
void	echo(t_master *data, char **args);
void	print_env(void *nd);
void	env(t_master *data);
char	*get_pwd(void);
void	cd(t_master *data, char **args);

// TESTING
void	print_lst(void *nd);
void	print_str_arr(char **arr);

#endif
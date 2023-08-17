
#ifndef MINISHELL_H
# define MINISHELL_H

# include "Libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <signal.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <sys/stat.h>

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
	char	char_type;
}				t_data;

typedef struct s_master
{
	t_list	*env_lst;
	t_list	*parsed_lst;
	char	**args;
	int		fds[MAX_PIPES + 1][2];
	pid_t	pids[MAX_PIPES + 1];
	int		n_pipes;
	int		cmd_idx;
	int		heredoc_idx;
	int		nargs;
	int		exit_code;
}				t_master;

typedef struct s_split_param
{
	int		count_1;
	int		count_2;
	int		inside_quotes;
	int		is_word;
	char	quote_type;
	char	**str_1;
	char	*s;
	char	char_type;
	int		red_l;
	int		red_r;
	int		pipe;
	char	sep_type;
	int		after_sep;
	int		error;
}				t_split_param;

typedef struct s_split
{
	char	*str;
	char	char_type;
	int		error;
}				t_split;

typedef struct s_split_total
{
	t_split_param	*sp;
	t_split			*split;
	char			**str_1;
}				t_split_total;

typedef struct s_init_list
{
	int		arg_flag;
	t_list	*list;
	t_list	*new;
	t_data	*data;
	int		i;
}				t_init_list;

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

void	fatal_error(t_master *d);
void	file_error(t_master *d, char *path, char *err_str, int code);
void	init_pipes(t_master *d);
void	executor(t_master *d);
void	heredoc(t_master *d);
void	set_redirection(int type, char *str, t_master *d);
void	set_all_redirections(t_master *d, t_list *lst);
void	set_pipe_redirection(t_master *d);
char	*expand_env_vars(char *str, t_master *d); // sustituir por función de álvaro
void	find_next_cmd(t_list **lst);
void	init_pipes(t_master *d);
void	count_pipes(t_master *d);
void	get_pargs(t_master *d, t_list *lst);
char	*get_pname(t_master *d, t_list *lst);
char	**get_path_arr(t_master *d);
char	*get_prog_path(t_master *d, t_list *lst, char **path_arr);
char	**get_env_arr(t_master *d);
char	*get_tmp_path(t_master *d);
void	close_fds(t_master *d);
void	input_heredoc(char *delim, int type, t_master *d);
void	get_all_input_heredoc(t_master *d);
void	catch_exit_code(t_master *d);
void	pwd(t_master *d);
void	echo(t_master *d, char **args);
void	print_env(void *nd);
char	*get_pwd(t_master *d);
void	cd(t_master *d);
int		is_long(char *str);
int		get_exit_status(char *str);
int		str_equal(char *str1, char *str2);
t_env	*init_env_data(char *key, char *value);
void	free_env(void *env_data);
void	update_env(t_master *d, char *key, char *value, int is_unset);
void	exit_builtin(t_master *d);
void	env_builtin(t_master *d);
void	print_export(void *nd);
int		is_valid_env_key(char *key);
void	export_unset(t_master *d, char **args, int is_unset);
void	export_unset_error(t_master *d, char *arg, int is_unset);
void	print_sorted_env(t_list *env);
void	*dup_env_data(void *env_data);
void	free_master_exit(t_master *d, int exit_code);
void	in_redirection(char *str, t_master *d);
void	get_echo_arg(char ***args, int *nl);


// TESTING
void	print_lst(void *nd);
void	print_str_arr(char **arr);
int		is_long_size(char *str);
int		get_long_digits(char *str);
int		check_limit(char *str);
char	*get_key(char *arg);
char	*get_value(char *arg);

//Split env
char		**ft_split_env(char const *s, char c);

//Env variables minishell
t_master	*inicialize_struct(void);
char		*get_env_variable(char *key, t_master *master);
void		print_env_list(t_master *master);

//Free minishell
void		ft_free_env(void *env);
void		ft_free_env_list(t_master *master);
void		ft_free_data(void *data);
void		ft_free_data_list(t_list *parsed_lst);

//Split parser - the first parser for single and double quotes
t_split		*ft_split_parser(char *s);


// Expand env variables
char		*expand_env_variables(char *str, t_master *master);

#endif
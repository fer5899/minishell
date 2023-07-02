#include "../minishell.h"

void	in_redir(void)
{

}
void	out_redir(void)
{
	
}

void	out_redir_append(void)
{
	
}

void	heredoc(char *delim)
{
	
}

void	heredoc_quotes(char *delim)
{
	int		tmp_fd;
	char	*input;
	int		input_len;

	tmp_fd = open(TEMP_PATH, O_WRONLY|O_CREAT|O_EXCL|O_TRUNC, 0600);
	input = get_next_line(0);
	input_len = ft_strlen(input);
	write(tmp_fd, input, input_len);
	while (input_len != ft_strlen(delim) && ft_strncmp(input, delim, input_len))
	{
		// repeat above and free
	}
	close(tmp_fd);
	tmp_fd = open(TEMP_PATH, O_RDONLY);
	unlink(TEMP_PATH);
	dup2(tmp_fd, 0);
	close(tmp_fd);
}

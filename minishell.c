
#include "minishell.h"

int	main(void)
{
	char	*command;

	while (1)
	{
		command = readline(">> ");
		if (command == NULL)
		{
			printf("\n");
			break ;
		}
		if (ft_strlen(command) == 4 && ft_strncmp(command, "exit", 4) == 0)
		{
			free(command);
			break ;
		}
		else
		{
			system(command);
			free(command);
		}
	}
	return (0);
}

#include "../inc/minishell.h"

int main(void)
{
	static char *line_read = (char *)NULL;

	printf("Welcome! You can exit by pressing Ctrl+C at any time...\n");
	while (1 == 1)
	{
		line_read = readline("minishell: ");
		printf("i read: %s\n", line_read);
		if (ft_strncmp(line_read, "exit", 4) == 0)
		{
			free(line_read);
			exit(EXIT_SUCCESS);
		}
		if (line_read && *line_read)
    		add_history(line_read);
		free(line_read);
	}
	return (0);
}

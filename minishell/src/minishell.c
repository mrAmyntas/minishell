// input goed verwerken, combinaties uitproberen

#include "../inc/minishell.h"

void	ft_echo(t_info *info)
{
	info->ret = 1;
}

void	ft_pwd(t_info *info)
{
	info->ret = 2;
}

void	ft_cd(t_info *info)
{
	info->ret = 3;
}


int	ft_redirect_in(t_info *info, int i)
{
	if (info->lines[i + 1] == NULL)
		i = 3;
	return (i);
}

int	ft_find_redirect(t_info *info)
{
	int	i;

	i = 0;
	while(info->lines[i] != NULL)
	{
		if (ft_strncmp(info->lines[i], "<", 1) == 0)
			return(ft_redirect_in(info, i));
//		if (ft_strncmp(info->lines[i], ">", 1) == 0)
//			return(ft_redirect_out(info, i));
		i++;
	}
	return (15);
}

int	ft_find_command(t_info *info, char **env)
{
	if (ft_strncmp(info->line_read, "echo", 4) == 0)
		return (exec(info->line_read, env));
	if (ft_strncmp(info->line_read, "cd", 2) == 0)
		return (exec(info->line_read, env));
	if (ft_strncmp(info->line_read, "pwd", 3) == 0)
		return (exec(info->line_read, env));
	if (ft_strncmp(info->line_read, "exit", 3) == 0)
		exit(0);
	return (15);
}
void	parseline(t_info *info, char **av, char **env)
{
		info->lines = ft_split(info->line_read, ' ');
		info->redirect = ft_find_redirect(info);
		info->cmd = ft_find_command(info, env);
		if (info->cmd == 15)
		{
			printf("minishell: command not found: %s\n", info->line_read);
			return ;
		}
		
}

int main(int ac, char **av, char **env)
{
	t_info info;

	//ft_init_struct(&info, av, env);
	printf("\033[1;33mWelcome! You can exit by pressing Ctrl+C at any time...\n");
	while (1 == 1)
	{
		info.line_read = readline("\033[0;33mminishell: \033[0m");
		if (!info.line_read[0])
			continue ;
		parseline(&info, av, env);
		if (info.line_read && *info.line_read)
    		add_history(info.line_read);
		free(info.line_read);
	}
	return (0);
}

// echo cd pwd export unset env exit
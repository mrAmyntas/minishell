/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 13:23:35 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/01/27 14:07:59 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_free(t_info *info)
{
	int	i;

	i = 0;
	free(info->line_read);
	info->line_read = NULL;
	while (info->tokens[i] != NULL)
	{
		free(info->tokens[i]);
		info->tokens[i] = NULL;
		i++;
	}
	free(info->token_state);
	free(info->tokens);
	info->t_pos = 0;
	info->p_pos = 0;
	info->exit_status = 0;
}

void	free_info(t_info *info)
{
	int	i;

	i = 0;
	//while (info->export[i])
	//{
	//	free (info->export[i]);
	//	i++;
	//}
	//free (info->export);
	//i = 0;
	while (info->env[i])
	{
		free (info->env[i]);
		i++;
	}
	free (info->env);
	free (info->pwd);
	free (info->home);
}

void	ft_init_struct(t_info *info, char **av, char **env)
{
	size_t	len;

	len = 10;
	info->av = av;
	get_env(info, env);
	info->line_read = NULL;
	info->cmd = 0;
	info->redirect = 0;
	info->ret = 0;
	info->t_pos = 0;
	info->p_pos = 0;
	info->fd_std[0] = dup(0);
	info->fd_std[1] = dup(1);
	info->home = ft_strdup(getenv("HOME"));
	info->pwd = malloc(sizeof(char *) * len);
	info->exit_status = 0;
	while (!getcwd(info->pwd, len))
	{
		len += 10;
		free (info->pwd);
		info->pwd = malloc(sizeof(char *) * len);
	}
	sort_export(info);
}

void	handle_sig(int signum)
{
	//printf("|%i|\n", signum);
	if (signum == SIGINT)
		write(1, "\n", 23);
	return ;
}

int	minishell(t_info *info)
{
	while (1 == 1)
	{
		signal(SIGINT, &handle_sig);
//		printf("hoi\n");
		signal(SIGQUIT, &handle_sig);
		info->line_read = readline("\033[0;33mminishell: \033[0m");
//		printf("hoi2\n");
		if (!info->line_read)
			break ;
		if (!info->line_read[0])
			continue ;
		lexer(info);
		info->ret = parser(info);
		if (info->ret == -1 || info->tokens[0] == NULL)
		{
			ft_free(info);
			continue ;
		}
		//if (!ft_strncmp(info->line_read, "break", 4)) // om leaks te checken
		//	break ;
		info->cmd = check_redirect_v2(info, 0, ft_strstrlen(info->tokens, "|", 0), 0);//check_redirect(&info);
		dup2(info->fd_std[0], 0);
		dup2(info->fd_std[1], 1);
		if (info->cmd == 15)
			printf("minishell: command not found: %s\n", info->line_read);
		if (info->line_read && *info->line_read)
    		add_history(info->line_read);
		ft_free(info);
	}
	return (info->cmd);
}
int main(int ac, char **av, char **env)
{
	t_info	info;

	ft_init_struct(&info, av, env);
	printf("\033[1;33mWelcome! You can exit by pressing Ctrl+D at any time...\033[1;33m\n");
	minishell(&info);
	free_info(&info);
	//system("leaks minishell");
//	rl_clear_history();
	return (0);
}

// tokens: < > << >> | $
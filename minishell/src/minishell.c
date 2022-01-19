/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/16 16:55:33 by bhoitzin      #+#    #+#                 */
<<<<<<< HEAD
/*   Updated: 2022/01/19 11:40:22 by bhoitzin      ########   odam.nl         */
=======
/*   Updated: 2022/01/19 11:42:54 by mgroen        ########   odam.nl         */
>>>>>>> 9fe908a9de3703db38d4f5594c0510f81c506a87
/*                                                                            */
/* ************************************************************************** */

// input goed verwerken, combinaties uitproberen

#include "../inc/minishell.h"

void	ft_free(t_info *info)
{
	int	i;

	i = 0;
	free(info->line_read);
	while (info->tokens[i] != NULL)
	{
		free(info->tokens[i]);
		info->tokens[i] = NULL;
		i++;
	}
	//{
	//	printf("i:%d\n", i);
	//	free(info->tokens[i]);
	//	info->tokens[i] = NULL;
	//	i++;
	//}
	free(info->tokens);
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
	while (!getcwd(info->pwd, len))
	{
		len += 10;
		free (info->pwd);
		info->pwd = malloc(sizeof(char *) * len);
	}
	sort_export(info);
}

int main(int ac, char **av, char **env)
{
	t_info info;

	ft_init_struct(&info, av, env);
	printf("\033[1;33mWelcome! You can exit by pressing Ctrl+C at any time...\n");
	while (1 == 1)
	{
		info.line_read = readline("\033[0;33mminishell: \033[0m");
		if (!info.line_read[0])
			continue ;
		lexer(&info);
		info.ret = parser(&info);
		if (info.ret == -1)
		{
			ft_free(&info);
			info.p_pos = 0;
			info.t_pos = 0;
			continue ;
		}
		//if (!ft_strncmp(info.line_read, "break", 4)) // om leaks te checken
		//	break ;
		if (info.tokens[0] == NULL)
			continue ;
		info.cmd = check_redirect(&info); //some temp bullshit to help check if milans work is working
		dup2(info.fd_std[0], 0);
		dup2(info.fd_std[1], 1);
		if (info.cmd == 15)
			printf("minishell: command not found: %s\n", info.line_read);
		if (info.line_read && *info.line_read)
    		add_history(info.line_read);
		ft_free(&info);
		info.p_pos = 0;
		info.t_pos = 0;
	}
	free_info(&info);
	//system("leaks minishell");
	return (0);
}
// echo cd pwd export unset env exit
// tokens: < > << >> | $
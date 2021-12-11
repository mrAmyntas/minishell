/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:35 by bhoitzin      #+#    #+#                 */
/*   Updated: 2021/12/11 19:16:13 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// input goed verwerken, combinaties uitproberen

#include "../inc/minishell.h"

void	ft_free(t_info *info)
{
	int	i;

	free(info->line_read);
	i = 0;
	while (info->tokens[i] != NULL)
	{
		free(info->tokens[i]);
		info->tokens[i] = NULL;
		i++;
	}
	free(info->tokens);
}

void	ft_init_struct(t_info *info, char **av, char **env)
{
	info->av = av;
	info->env = env;
	info->line_read = NULL;
	info->cmd = 0;
	info->redirect = 0;
	info->ret = 0;
	info->t_pos = 0;
	info->p_pos = 0;
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
		info.cmd = ft_find_command(&info); //some temp bullshit to help check if milans work is working
		if (info.cmd == 15)
			printf("minishell: command not found: %s\n", info.line_read);
		if (info.line_read && *info.line_read)
    		add_history(info.line_read);
		ft_free(&info);
		info.p_pos = 0;
		info.t_pos = 0;
	}
	return (0);
}
// echo cd pwd export unset env exit
// tokens: < > << >> | $
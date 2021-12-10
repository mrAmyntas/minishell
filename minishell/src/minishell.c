/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:35 by bhoitzin      #+#    #+#                 */
/*   Updated: 2021/12/10 11:41:33 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// input goed verwerken, combinaties uitproberen

#include "../inc/minishell.h"

void	ft_init_struct(t_info *info, char **av, char **env)
{
	info->av = av;
	info->env = env;
	info->line_read = NULL;
	info->cmd = 0;
	info->redirect = 0;
	info->ret = 0;
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
		parseline(&info);
		if (info.line_read && *info.line_read)
    		add_history(info.line_read);
		free(info.line_read);
	}
	return (0);
}

// echo cd pwd export unset env exit
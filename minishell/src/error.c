/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 15:05:11 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/01/27 17:36:20 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	set_error(t_info *info, int error_type, char *str)
{
	info->exit_status = error_type;
	if (error_type == 258)
		printf("minishell: syntax error\n");
}

void	ft_error(t_info *info, int i, char *cmd)
{
	if (i == -1)
	{
		printf("minishell: malloc error\n");
		ft_free(info);
		exit(1);
	}
	if (i == -2)
	{
		printf("minishell: syntax error: unclosed quote\n");
		ft_free(info);
		minishell(info);
	}
	if (info->exit_status == 1)
		perror("");
//	if (info->exit_status == 258)
//		printf("minishell: syntax error\n");
	if (info->exit_status == 127)
		printf("minishell: command not found\n");
	else
		info->exit_status = 0;
	return ;
}

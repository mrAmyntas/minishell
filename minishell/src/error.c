/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 15:05:11 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/01/27 16:43:03 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	set_error(t_info *info, int error_type)
{
	info->exit_status = error_type;
	if (error_type == 258)
		printf("minishell: syntax error\n");
}

void	ft_error(t_info *info, int i)
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
		printf("minishell: %s: No such file or directory\n", info->tokens[i]);
	if (info->exit_status == 258)
		printf("minishell: syntax error\n");
	if (info->exit_status == 127)
		printf("minishell: command not found\n");
	else
		info->exit_status = 0;
	return ;
}

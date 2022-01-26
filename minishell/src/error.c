/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 15:05:11 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/01/26 17:20:30 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_error(t_info *info, int error_type)
{
	if (error_type == 1)
		perror("malloc error:");
	if (error_type == 2)
	{
		info->exit_status = 1;
		printf("minishell: unclosed quote\n");
	}
	if (error_type == 3)
	{
		info->exit_status = 258;
		printf("minishell: syntax error\n");
	}
	ft_free(info);
	minishell(info);
}

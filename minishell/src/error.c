/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 15:05:11 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/01/28 13:09:45 by mgroen        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	set_error(t_info *info, int error_type, char *str)
{
	int	i;

	i = ft_strlen(str);
	if (info->exit_msg != NULL)
	{
		free(info->exit_msg);
		info->exit_msg = NULL;
	}
	info->exit_msg = (char *)malloc(sizeof(char) * (i + 1));
	ft_strlcpy(info->exit_msg, str, i + 1);
	info->exit_status = error_type;
	if (error_type == 258)
		printf("minishell: syntax error\n");
}

void	ft_error(t_info *info, int i)
{
	//printf("check\n");
	if (i == -1)
	{
		printf("minishell: malloc error\n");
		ft_free(info);
		exit(1);
	}
	else if (i == -2)
	{
		printf("minishell: syntax error: unclosed quote\n");
		ft_free(info);
		minishell(info);
	}
	else if (info->exit_status == 1)
	{
		write(2, "minishell: ", 12);
		perror(info->exit_msg);
	}
	else if (info->exit_status == 127)
	{
		write(2, "minishell: ", 12);
		write(2, info->exit_msg, ft_strlen(info->exit_msg));
		write(2, ": command not found\n", 20);
	}
	else if (info->exit_status == 258)
		return;
	else
		info->exit_status = 0;
	return ;
}

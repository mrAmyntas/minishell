/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 15:05:11 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/02 20:24:58 by bhoitzin      ########   odam.nl         */
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
	{
		write(2, "minishell: syntax error near ", 30);
		write(2, "near unexpected token `newline'\n", 33);
	}
}

void	ft_error2(t_info *info, int i)
{
	if (i == -1)
	{
		write(2, "minishell: malloc error\n", 25);
		ft_free(info);
		exit(1);
	}
	if (i == -2)
	{
		write(2, "minishell: syntax error: unclosed quote\n", 41);
		ft_free(info);
		minishell(info);
		rl_clear_history();
		exit(1);
	}
	if (i == -3)
	{
		write(2, "minishell: syntax error: no process after pipe\n", 48);
		ft_free(info);
		minishell(info);
		rl_clear_history();
		exit(1);
	}
}

void	ft_error(t_info *info, int i)
{
	if (i < 0)
		ft_error2(info, i);
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
		return ;
	else
		info->exit_status = 0;
	return ;
}

int	check_nosuchdir(t_info *info)
{
	int	i;
	DIR	*ret;

	i = 0;
	while (info->tokens[i] != NULL)
	{
		if ((ft_strncmp(info->tokens[i], "cd", 2) == 0
				&& ft_strlen(info->tokens[i]) == 2))
		{
			ret = opendir(info->tokens[i + 1]);
			if (ret == NULL)
			{
				set_error(info, 1, info->tokens[i + 1]);
				ft_error(info, 0);
				ft_free(info);
				exit(1);
			}
			closedir(ret);
			return (0);
		}
		i++;
	}
	return (1);
}

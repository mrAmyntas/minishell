/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 15:05:11 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/03 12:34:33 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	syntax_error(t_info *info, int token)
{
	char	c;

	write(2, "minishell: syntax error near unexpected", 40);
	if (token == 0)
		write(2, " token `newline'\n", 18);
	else
	{
		c = check_char_token(info, token, 0);
		write(2, " token `", 8);
		write(2, &c, 1);
		write(2, "`\n", 2);
		ft_free(info);
		minishell(info);
		rl_clear_history();
		exit(1);
	}
}

void	set_error(t_info *info, int error_type, char *str, int token)
{
	int		i;

	i = ft_strlen(str);
	if (info->exit_msg != NULL)
	{
		free(info->exit_msg);
		info->exit_msg = NULL;
	}
	info->exit_msg = (char *)malloc(sizeof(char) * (i + 1));
	if (info->exit_msg == NULL)
		ft_error(info, -1);
	ft_strlcpy(info->exit_msg, str, i + 1);
	info->exit_status = error_type;
	if (error_type == 258)
		syntax_error(info, token);
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
		free(info->exit_msg);
		info->exit_msg = NULL;
	}
	else if (info->exit_status == 127)
	{
		write(2, "minishell: ", 12);
		write(2, info->exit_msg, ft_strlen(info->exit_msg));
		free(info->exit_msg);
		info->exit_msg = NULL;
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
				set_error(info, 1, info->tokens[i + 1], 0);
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

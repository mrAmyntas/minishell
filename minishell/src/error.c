/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 15:05:11 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/09 17:23:40 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	syntax_error(t_info *info, int token)
{
	int	c;

	write(2, "minishell: syntax error near unexpected", 40);
	if (token == 0)
		write(2, " token `newline'\n", 18);
	else
	{
		c = check_char_token(info, token, 0);
		write(2, " token `", 8);
		if (c == 129 || c == 130)
		{
			c = check_char(info, 0, "<");
			if (check_char_token(info, token, 0) == 130)
				c = check_char(info, 0, ">");
			write(2, &c, 1);
		}
		write(2, &c, 1);
		write(2, "`\n", 2);
	}
	return ;
}

// pass through the errortype and a token. Token helps decide
// which error message to write to mimic bash. the type is to
// set correct exit status
void	set_error(t_info *info, int error_type, char *str, int token)
{
	int		i;

//	printf("status_set_error:%d error type%d\n", g_sig.exit_status, error_type);
//	if (g_sig.exit_status == 258)
//		return ;
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
	g_sig.exit_status = error_type;
	g_sig.exit_status2 = error_type;
//	dprintf(2, "set_error: exst:%d\n", g_sig.exit_status);
	if (error_type == 258)
		syntax_error(info, token);
	else if (token < 4)
		ft_error(info, token);
}

void	ft_error2(t_info *info, int i)
{
	if (i == -1)
	{
		write(2, "minishell: malloc error\n", 25);
		exit(1);
	}
	if (i == -2 || i == -3)
	{
		g_sig.exit_status = 258;
		g_sig.exit_status2 = 258;
		write(2, "minishell: syntax error: ", 26);
		if (i == -2)
			write(2, "unclosed quote\n", 16);
		else
			write(2, "no process after pipe\n", 23);
	}
}

void	invalid_identifier(t_info *info)
{
	write(2, "minishell: export: '", 20);
	write(2, info->exit_msg, ft_strlen(info->exit_msg));
	write(2, "': not a valid identifier\n", 26);
	g_sig.exit_status = 1;
	g_sig.exit_status2 = 1;
}

void	ft_error(t_info *info, int i)
{
//	printf("i:%d exit_status:%d\n", i, g_sig.exit_status);
	if (i <= -1 && i >= -3)
		ft_error2(info, i);
	else if (g_sig.exit_status == 258)
		return ;
	if (g_sig.exit_status == 2)
		invalid_identifier(info);
	else if (g_sig.exit_status == 1 || g_sig.exit_status == 126)
	{
		write(2, "minishell: ", 12);
		if (g_sig.exit_status == 1)
			write(2, "cd: ", 4);
		write(2, info->exit_msg, ft_strlen(info->exit_msg));
		if (i == -4)
			perror(" ");
		if (i == -5)
			write(2, ": Permission denied or is a directory\n", 39);
	}
	else if (g_sig.exit_status == 127)
	{
		write(2, "minishell: ", 12);
		write(2, info->exit_msg, ft_strlen(info->exit_msg));
		if (i == -4)
			write(2, ": command not found\n", 20);
		if (i == -5)
			write(2, ": No such file or directory\n", 29);
	}
	return ;
}

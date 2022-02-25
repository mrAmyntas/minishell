/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 15:05:11 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/25 10:52:42 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	syntax_error(t_info *info, int token)
{
	int	c;

	write(2, "minishell: syntax error near unexpected", 40);
	if (token == 4 || token == 0)
		write(2, " token `newline'\n", 18);
	else
	{
		c = check_char_token(info, token, 0);
		write(2, " token `", 8);
		if (c == 129 || c == 130)
		{
			c = check_char(0, "<");
			if (check_char_token(info, token, 0) == 130)
				c = check_char(0, ">");
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

	if (info->error_done != 0)
		return ;
	info->error_done = 1;
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
	if (error_type == 258)
		syntax_error(info, token);
	else if (token < 4)
	{
		dup2(info->fd_std[2], 2);
		ft_error(info, token);
	}
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
		free_stuff(info);
		minishell(info);
		rl_clear_history();
		exit(0);
	}
}

void	invalid_identifier(t_info *info)
{
	write(2, "export: '", 10);
	write(2, info->exit_msg, ft_strlen(info->exit_msg));
	write(2, "': not a valid identifier\n", 26);
	g_sig.exit_status = 1;
	g_sig.exit_status2 = 1;
}

void	ft_error(t_info *info, int i)
{
	if (i <= -1 && i >= -3)
		return (ft_error2(info, i));
	write(2, "minishell: ", 12);
	if (g_sig.exit_status == 1 || g_sig.exit_status == 126)
	{
		if (g_sig.exit_status == 1 && i == -4)
			write(2, "cd: ", 4);
		write(2, info->exit_msg, ft_strlen(info->exit_msg));
		if (i == -4)
			perror(" ");
		if (i == -6)
			write(2, ": No such file or directory\n", 29);
		if (i == -5)
			write(2, ": Permission denied or is (not) a directory\n", 45);
	}
	else if (g_sig.exit_status == 127)
	{
		write(2, info->exit_msg, ft_strlen(info->exit_msg));
		if (i == -4)
			write(2, ": command not found\n", 20);
		if (i == -5)
			write(2, ": No such file or directory\n", 29);
	}
	else if (g_sig.exit_status == 2)
		invalid_identifier(info);
}

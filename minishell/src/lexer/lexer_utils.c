/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/02 13:37:25 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/02 13:47:04 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_char(t_info *info, int i, char *line_read)
{
	if (line_read[i] == '<')
	{
		if (line_read[i + 1] == '<')
			return (C_DLESSER);
		return (C_LESSER);
	}
	if (line_read[i] == '>')
	{
		if (line_read[i + 1] == '>')
			return (C_DGREATER);
		return (C_GREATER);
	}
	if (line_read[i] == '|')
		return (C_PIPE);
	if (line_read[i] == '$')
		return (C_DOLLAR);
	if (line_read[i] == '\'')
		return (C_QUOTE);
	if (line_read[i] == '\"')
		return (C_DQUOTE);
	if (line_read[i] == '\n')
		return (C_NEWLINE);
	if (line_read[i] == ' ')
		return (C_SPACE);
	return (C_NORMAL);
}

int	check_char_token(t_info *info, int i, int j)
{
	if (info->tokens[i][j] == '<')
	{
		if (info->tokens[i][j + 1] == '<')
			return (C_DLESSER);
		return (C_LESSER);
	}
	if (info->tokens[i][j] == '>')
	{
		if (info->tokens[i][j + 1] == '>')
			return (C_DGREATER);
		return (C_GREATER);
	}
	if (info->tokens[i][j] == '|')
		return (C_PIPE);
	if (info->tokens[i][j] == '$')
		return (C_DOLLAR);
	if (info->tokens[i][j] == '\'')
		return (C_QUOTE);
	if (info->tokens[i][j] == '\"')
		return (C_DQUOTE);
	if (info->tokens[i][j] == '\n')
		return (C_NEWLINE);
	if (info->tokens[i][j] == ' ')
		return (C_SPACE);
	return (C_NORMAL);
}

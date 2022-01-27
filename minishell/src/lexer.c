/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 19:03:32 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/01/27 18:00:23 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// str join leaks

#include "../inc/minishell.h"

char	*ft_remove_spaces(t_info *info)
{
	int		i;
	int		j;
	char	*buff;

	i = 0;
	while (info->line_read[i] == ' ')
		i++;
	if (i == 0)
		return (info->line_read);
	j = ft_strlen(info->line_read);
	buff = (char *)malloc((sizeof(char) * j) - i + 1);
	j = 0;
	while (info->line_read[i] != '\0')
	{
		buff[j] = info->line_read[i];
		i++;
		j++;
	}
	buff[j] = '\0';
	free(info->line_read);
	info->line_read = NULL;
	return (buff);
}

int	check_char(t_info *info, int i)
{
	if (info->line_read[i] == '<')
	{
		if (info->line_read[i + 1] == '<')
			return (C_DLESSER);
		return (C_LESSER);
	}
	if (info->line_read[i] == '>')
	{
		if (info->line_read[i + 1] == '>')
			return (C_DGREATER);
		return (C_GREATER);
	}
	if (info->line_read[i] == '|')
		return (C_PIPE);
	if (info->line_read[i] == '$')
		return (C_DOLLAR);
//	if (info->line_read[i] == ';')
//		return (C_SEMICOLON);
//	if (info->line_read[i] == '\\')
//		return (C_BACKSLASH);
	if (info->line_read[i] == '\'')
		return (C_QUOTE);
	if (info->line_read[i] == '\"')
		return (C_DQUOTE);
	if (info->line_read[i] == '\n')
		return (C_NEWLINE);
	if (info->line_read[i] == ' ')
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
//	if (info->tokens[i][j] == ';')
//		return (C_SEMICOLON);
//	if (info->tokens[i][j] == '\\')
//		return (C_BACKSLASH);
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

void	store_char(t_info *info, int i)
{
	int j;
	char *temp;

// leak checker thinks this matters but i dont
//	if (info->tokens[info->t_pos])
//	{
//		printf("token:%s, i:%d\n", info->tokens[info->t_pos], i);
//		free(info->tokens[info->t_pos]);
//		info->tokens[info->t_pos] = NULL;
//	}
	info->tokens[info->t_pos] = (char *)malloc(sizeof(char) * 2);
	if (info->tokens[info->t_pos] == NULL)
		ft_error(info, -1);
	info->tokens[info->t_pos][0] = info->line_read[i];
	info->tokens[info->t_pos][1] = '\0';
//	printf("store:%c i:%d\n", info->tokens[info->t_pos][0], info->t_pos);
	info->t_pos++;
	info->p_pos++;
}

void	store_string(t_info *info, int i)
{
	int	j;

	if (i == 0)
		return (store_char(info, i));
	if (check_char(info, i - 1) == 0 || info->p_pos == 0)
	{
		info->tokens[info->t_pos] = (char *)malloc(sizeof(char) * (i - info->p_pos + 1));
		if (info->tokens[info->t_pos] == NULL)
			ft_error(info, -1);
		j = 0;
		while (info->p_pos < i)
		{
			info->tokens[info->t_pos][j] = info->line_read[info->p_pos];
			info->p_pos++;
			j++;
		}
		info->tokens[info->t_pos][j] = '\0';
		info->t_pos++;
	}
	if (info->line_read[i] != '\0' )
		store_char(info, i);
}

int	store_input(t_info *info)
{
	int	i;
	int j;

	i = 0;
	while (info->line_read[i] != '\0')
	{
		info->char_type = check_char(info, i);
		if (info->char_type != 0)
			store_string(info, i);
		i++;
	}
	store_string(info, i);
	int p = 0;
	while (info->tokens[p] != NULL)
	{
//		printf("stored = %s i:%d\n", info->tokens[p], p);
		p++;
	}
	return (0);
}

void	lexer(t_info *info)
{
	int	i;

	info->line_read = ft_remove_spaces(info); // leading spaces are ignored
	i = ft_strlen(info->line_read);
	info->tokens = (char **)malloc(sizeof(char *) * (i + 2)); // could pre-scan inputline to malloc precise amount required
	if (info->tokens == NULL)									//require +2 for the empty quote removal that jumps 2 in front to not go out of bound
		ft_error(info, -1);
	info->token_state = (int *)malloc(sizeof(int) * (i + 2));
	if (info->token_state == NULL)
		ft_error(info, -1);
	while (i + 1 >= 0)
	{
		info->tokens[i + 1] = NULL;
		info->token_state[i + 1] = -1;
		i--;
	}
	store_input(info); // interpret line_read
}

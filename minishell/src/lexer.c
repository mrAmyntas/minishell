/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 19:03:32 by bhoitzin      #+#    #+#                 */
/*   Updated: 2021/12/11 18:57:49 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

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
	if (info->line_read[i] == '\0')
		return (C_EOF);
	return (C_NORMAL);
}

void	store_char(t_info *info, int i)
{
	int j;

	j = check_char(info, i);
	if (j == C_SPACE)
	{
		info->p_pos++;
		return ;
	}
	info->tokens[info->t_pos] = (char *)malloc(sizeof(char) * 2);
	if (info->tokens[info->t_pos] == NULL)
		ft_error(1);
	info->tokens[info->t_pos][0] = info->line_read[i];
	info->tokens[info->t_pos][1] = '\0';
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
			ft_error(1);
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
	if (i > 0 && info->line_read[i] != '\0' )
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
		printf("stored = %s\n", info->tokens[p]);
		p++;
	}
	return (0);
}

void	lexer(t_info *info)
{
	int	i;

	info->line_read = ft_remove_spaces(info); // leading spaces are ignored
	i = ft_strlen(info->line_read);
	info->tokens = (char **)malloc(sizeof(char *) * (i + 1)); // could pre-scan inputline to malloc precise amount required
	if (info->tokens == NULL)
		ft_error(1);
	while (i >= 0)
	{
		info->tokens[i] = NULL;
		i--;
	}
	store_input(info); // interpret line_read
}

/* 
int	store_string_dquote(t_info *info, int i, int store_pos)
{
	int	j;

	if (i - store_pos < 2) // nothing between the quotes
	{
		info->p_pos++;
		return (i - store_pos);
	}
	info->tokens[info->t_pos] = (char *)malloc(sizeof(char) * (i - store_pos));
	if (info->tokens[info->t_pos] == NULL)
		ft_error(1);
	j = 0;
	while (info->p_pos < i)
	{
		info->tokens[info->t_pos][j] = info->line_read[info->p_pos];
		info->p_pos++;
		j++;
	}
	info->tokens[info->t_pos][j] = '\0';
	info->t_pos++;
	info->p_pos++;
	return (i - store_pos);
}

int	store_string_quote(t_info *info, int i, int store_pos)
{
	int	j;

	if (i - store_pos < 2) // nothing between the quotes
	{
		info->p_pos++;
		return (i - store_pos);
	}
	info->tokens[info->t_pos] = (char *)malloc(sizeof(char) * (i - store_pos));
	if (info->tokens[info->t_pos] == NULL)
		ft_error(1);
	j = 0;
	while (info->p_pos < i)
	{
		info->tokens[info->t_pos][j] = info->line_read[info->p_pos];
		info->p_pos++;
		j++;
	}
	info->tokens[info->t_pos][j] = '\0';
	info->t_pos++;
	info->p_pos++;
	return (i - store_pos);
}

int	iter_quotes(t_info *info, int i, int char_type, int store_pos)
{
	if (char_type == C_QUOTE)
	{
		i++;
		while (check_char(info, i) != C_QUOTE && info->line_read[i] != '\0')
			i++;
		if (info->line_read[i] == '\0')
			ft_error(2);
		return(store_string_quote(info, i, store_pos));
	}
	else
	{
		i++;
		while (check_char(info, i) != C_DQUOTE && info->line_read[i] != '\0')
		{
			if (check_char(info, i) == C_DOLLAR)
				expand_dollar(info, i);
			i++;
		}
		if (info->line_read[i] == '\0')
			ft_error(2);
		return (store_string_dquote(info, i, store_pos));
	}
} // loop till next quote, if no next quote (eof) ret error
*/
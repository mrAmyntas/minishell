/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 19:03:32 by bhoitzin      #+#    #+#                 */
/*   Updated: 2021/12/10 19:08:06 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	store_string_quote(t_info *info, int i)
{
	
}

void	store_char(t_info *info, int i)
{
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
		info->tokens[info->t_pos] = (char *)malloc((sizeof(char) * i) - info->p_pos + 1);
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

void	expand_dollar(t_info *info, int i)
{
	return ;
}


int	iter_quotes(t_info *info, int i, int char_type)
{
	if (char_type == CHAR_QUOTE)
	{
		while (check_char(info, i) != CHAR_QUOTE && info->line_read[i] != '\0')
			i++;
		if (info->line_read[i] == '\0')
			ft_error(2);
		store_string(info, i);
	}
	if (char_type == CHAR_DQUOTE)
	{
		while (check_char(info, i) != CHAR_DQUOTE && info->line_read[i] != '\0')
		{
			if (check_char(info, i) == CHAR_DOLLAR)
				expand_dollar(info, i);
			i++;
		if (info->line_read[i] == '\0')
			ft_error(2);
		store_string(info, i);
		}
	}
} // loop till next quote, if no next quote (eof) go and 

int	ft_lexer(t_info *info)
{
	int	i;
	int j;

	i = 0;
	while (info->line_read[i] != '\0')
	{
		info->char_type = check_char(info, i);
		if (info->char_type == CHAR_DQUOTE || info->char_type == CHAR_QUOTE)
			i = iter_quotes(info, i, info->char_type);
		else if (info->char_type != 0)
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
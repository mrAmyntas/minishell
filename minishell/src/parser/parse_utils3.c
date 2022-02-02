/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_utils3.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/17 11:22:30 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/02 14:47:04 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	cut_quotes2(t_info *info, int pos, int first, int last)
{
	while (info->tokens[pos][first + 1] != '\0')
	{
		info->tokens[pos][first] = info->tokens[pos][first + 1];
		first++;
	}
	info->tokens[pos][first] = '\0';
	while (info->tokens[pos][last] != '\0')
	{
		info->tokens[pos][last - 1] = info->tokens[pos][last];
		last++;
	}
	info->tokens[pos][last - 1] = '\0';
}

// continue finding quotes on next char in string (ret last - 1)
int	cut_quotes(t_info *info, int i, char c, int j)
{
	int	last;
	int	first;

	first = j;
	while (info->tokens[i][j + 1] != c)
		j++;
	last = j + 1;
	cut_quotes2(info, i, first, last);
	return (last - 1);
}

void	cut_dollar(t_info *info, int i, int start, int end)
{
	int	j;

	j = 0;
	while (info->tokens[i][end + 1 + j] != '\0')
	{
		info->tokens[i][start] = info->tokens[i][end + 1 + j];
		start++;
		j++;
	}
	info->tokens[i][start] = '\0';
}

static void	merge_tokens(t_info *info, int i, char c)
{
	free(info->tokens[i]);
	info->tokens[i] = (char *)malloc(sizeof(char) * 3);
	info->tokens[i][0] = c;
	info->tokens[i][1] = c;
	info->tokens[i][2] = '\0';
	free(info->tokens[i + 1]);
	info->tokens[i + 1] = NULL;
	realloc_copy(info, i + 1, 1);
}

void	find_dgreater_dlesser(t_info *info)
{
	int	i;

	i = 0;
	while (info->tokens[i] != NULL && info->tokens[i + 1] != NULL)
	{
		if (info->tokens[i][0] == '<' && info->tokens[i + 1][0] == '<')
			merge_tokens(info, i, '<');
		if (info->tokens[i][0] == '>' && info->tokens[i + 1][0] == '>')
			merge_tokens(info, i, '>');
		i++;
	}
}

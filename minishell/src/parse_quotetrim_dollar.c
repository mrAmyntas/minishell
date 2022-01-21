/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_quotetrim_dollar.c                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/17 11:22:30 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/01/21 12:45:11 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	expandexitstatus(t_info *info, int i)
{
	// $ at tokens[i] '?' at i+1 -> need to put exitstatus + whatever comes after the ? onthe same line
	// on i, and remove i+1
}

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

int	cut_quotes(t_info *info, int i, char c, int j)
{
	int last;
	int first;

	first = j;
	while (info->tokens[i][j + 1] != c)
		j++;
	last = j + 1;
	cut_quotes2(info, i, first, last);
	return (last - 1); // continue finding quotes on next char in string
}

void	remove_quotes(t_info *info)
{
	int	i;
	int	j;

	i = 0;
	while (info->tokens[i] != NULL)
	{
		j = 0;
		while (info->tokens[i][j] != '\0')
		{
			if (info->tokens[i][j] == '\'' || info->tokens[i][j] == '\"')
			{
				j = cut_quotes(info, i, info->tokens[i][j], j);
				continue ;
			}
			j++;
		}
		if (ft_strlen(info->tokens[i]) == 0)
		{
			free(info->tokens[i]);
			info->tokens[i] = NULL;
			realloc_copy(info, i, 1);
		}
		i++;
	}
}

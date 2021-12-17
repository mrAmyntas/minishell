/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_quotetrim_dollar.c                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/17 11:22:30 by bhoitzin      #+#    #+#                 */
/*   Updated: 2021/12/17 15:42:49 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	expandexitstatus(t_info *info, int i)
{
	// $ at tokens[i] '?' at i+1 -> need to put exitstatus + whatever comes after the ? onthe same line
	// on i, and remove i+1
}

static void	trim_quotes2(t_info *info, int pos, int first, int last)
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

void	trim_quotes(t_info *info, int pos, int len)
{
	int	first;
	int last;
	int	i;

	i = 0;
	while (info->tokens[pos][i] != '\0')
	{
		if (info->tokens[pos][i] == C_QUOTE || info->tokens[pos][i] == C_DQUOTE)
			break ;
		i++;
	}
	first = i;
	i = len - 1;
	while (i >= 0)
	{
		if (info->tokens[pos][i] == C_QUOTE || info->tokens[pos][i] == C_DQUOTE)
			break ;
		i--;
	}
	last = i;
	trim_quotes2(info, pos, first, last);	
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:31 by bhoitzin      #+#    #+#                 */
/*   Updated: 2021/12/11 19:48:33 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	checkquotes(t_info *info, int i, int store_pos)
{
	while(i - 1 >= 0)
	{
		if (info->tokens[i][0] == C_QUOTE || info->tokens[i][0] == C_DQUOTE)
			break ;
		i--;
	}
	if (i == 0)
		return (0);
	return (0);
}

void	expand_dollar(t_info *info)
{
	int	i;

	i = 0;
	while (info->tokens[i] != NULL)
	{
		if (info->tokens[i][0] == C_DOLLAR)
		{
			if (i != 0 && checkquotes(info, i, i) == 0)
				return ;
		}
	}
}
//3.230 Name
//In the shell command language, a word consisting solely of underscores, digits, 
//and alphabetics from the portable character set. The first character of a name is not a digit.
// https://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_230
// https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html#tag_02_06_02



int	check_unclosed(t_info *info, int i, int j)
{
	if (info->tokens[i][0] == C_QUOTE)
	{
		while (info->tokens[j] != NULL)
		{
			if (info->tokens[j][0] == C_QUOTE)
				break ;
			j++;
		}
	}
	else
	{
		while (info->tokens[j] != NULL)
		{
			if (info->tokens[j][0] == C_DQUOTE)
				break ;
			j++;
		}
	}
	if (info->tokens[j] == NULL)
		return (-1);
	return (j);
}

void	merge_quotes(t_info *info, int i, int j)
{
	int	n;
	int	p;

	p = i + 1;
	n = 1;
	while (p < j - 1)
	{
		info->tokens[i + 1] = ft_strjoin(info->tokens[i + 1], info->tokens[i + n + 1]);
		p++;
		n++;
	}
}

void	cut_lines(t_info *info, int i, int j)
{
	
}

int	parse_quotes(t_info *info)
{
	int	i;
	int	ret;

	i = 0;
	while (info->tokens[i] != NULL)
	{
		if (info->tokens[i][0] == C_QUOTE || info->tokens[i][0] == C_DQUOTE)
		{
			ret = check_unclosed(info, i, i + 1); // i is pos quote 1, ret is pos quote 2
			if (ret == -1)
				return (-1);
			merge_quotes(info, i, ret);
			//cut_lines(info, i, ret);
			i = ret;
			
		}
		i++;
	}
	return (0);
}

int	parser(t_info *info)
{
	int	ret;

	ret = parse_quotes(info);
	if (ret == -1)
		return (-1);
//	expand_dollar(info);
//	remove_quotes(info);
//	fix_order(info);
	printf("after merge\n");
	int p = 0;
	while (info->tokens[p] != NULL)
	{
		printf("stored = %s\n", info->tokens[p]);
		p++;
	}
	return (0);
}

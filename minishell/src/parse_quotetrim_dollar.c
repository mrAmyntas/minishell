/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_quotetrim_dollar.c                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/17 11:22:30 by bhoitzin      #+#    #+#                 */
/*   Updated: 2021/12/17 12:38:53 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	expand_dollar(t_info *info, int i, int pos)
{
	char	*name;

	/*name = get_val(info, info->tokens[pos_dollar + 1]);
	if (name == NULL)
	{
		free (info->tokens[pos_dollar]);
		info->tokens[pos_dollar] = NULL;
		realloc_copy(info, pos_dollar, 1);
		free (info->tokens[pos_dollar]);
		info->tokens[pos_dollar] = NULL;
		realloc_copy(info, pos_dollar, 1);
	}
	else
	{
		i = check_before_after(info, pos_dollar, pos_dollar);
		if (i == 2 || i == 1)
		{
			info->tokens[pos_dollar - 1] = ft_strjoinbas(info->tokens[pos_dollar - 1], name);
			free (info->tokens[pos_dollar]);
			info->tokens[pos_dollar] = NULL;
			realloc_copy(info, pos_dollar, 1);
			free (info->tokens[pos_dollar]);
			info->tokens[pos_dollar] = NULL;
			realloc_copy(info, pos_dollar, 1);
		}
		else
		{
			i = ft_strlen(name);
			free (info->tokens[pos_dollar]);
			info->tokens[pos_dollar] = (char *)malloc(i + 1);
			ft_strlcpy(info->tokens[pos_dollar], name, i + 1);
			free (info->tokens[pos_dollar + 1]);
			info->tokens[pos_dollar + 1] = NULL;
			realloc_copy(info, pos_dollar + 1, 1);
		}
	}*/
}

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

/*
void	check_dollar(t_info *info)
{
	int	i;

	if (ret[0] == -1)
	{
		if (check_char_token(info, first_q) == C_DOLLAR)
			expand_dollar(info, first_q, ret);
	}
	else if (check_char_token(info, first_q) == C_DQUOTE)
	{
		i = 0;
		while (i < ret[0] - first_q - 1)
		{
			if (check_char_token(info, first_q + 1 + i) == C_DOLLAR)
				expand_dollar(info, first_q + 1 + i, ret);
			i++;
		}
	}
}
*/
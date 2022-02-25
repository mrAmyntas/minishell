/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_quotes.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/02 13:57:56 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/24 11:32:26 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	check_empty_quotes(t_info *info, int first_q, int last_q)
{
	if (last_q - first_q == 1)
	{
		free(info->tokens[first_q]);
		info->tokens[first_q] = NULL;
		free(info->tokens[last_q]);
		info->tokens[last_q] = NULL;
		realloc_copy(info, first_q, 2);
		realloc_copy(info, last_q, 2);
		joinwithnormalbefore(info, first_q);
		return (1);
	}
	return (0);
}

static int	check_before_after(t_info *info, int first_q, int last_q, int ret)
{
	if (first_q != 0)
	{
		if (info->tokens[first_q - 1] != NULL
			&& check_char_token(info, first_q - 1, 0) == C_NORMAL)
		{
			ret++;
			if (info->tokens[last_q + 1] != NULL
				&& check_char_token(info, last_q + 1, 0) == C_NORMAL)
				ret++;
			return (ret);
		}
	}
	if (info->tokens[last_q + 1] != NULL
		&& check_char_token(info, last_q + 1, 0) == C_NORMAL)
		ret = 3;
	return (ret);
}

static int	check_unclosed(t_info *info, int i, int j)
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
		return (-2);
	return (j);
}

static void	merge_quotes(t_info *info, int q_1, int q_2, int n)
{
	int	i;

	if (n == 1 || n == 2)
		q_1--;
	if (n == 2 || n == 3)
		q_2++;
	i = q_1;
	while (i < q_2)
	{
		info->tokens[q_1] = ft_strjoinbas(info->tokens[q_1],
				info->tokens[q_1 + 1]);
		free(info->tokens[q_1 + 1]);
		info->tokens[q_1 + 1] = NULL;
		realloc_copy(info, q_1 + 1, 1);
		i++;
	}
}

// i is pos quote 1, ret is pos quote 2
//n == 0: no normal chars before or after quotes
//n == 1, only before, n = 2, after & before 
//n = 3 only after
int	parse_quotes(t_info *info, int i)
{
	int	n;
	int	ret;

	while (info->tokens[i] != NULL)
	{
		ret = -1;
		if (info->tokens[i][0] == C_QUOTE || info->tokens[i][0] == C_DQUOTE)
		{
			ret = check_unclosed(info, i, i + 1);
			if (ret == -2)
				ft_error(info, -2);
			if (check_empty_quotes(info, i, ret) == 1)
				continue ;
			n = check_before_after(info, i, ret, 0);
			merge_quotes(info, i, ret, n);
			if (n == 1 || n == 2)
				continue ;
		}
		i++;
	}
	return (0);
}

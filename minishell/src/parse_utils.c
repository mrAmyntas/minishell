/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/17 11:17:21 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/01/28 15:09:13 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


void	joinwithbefore(t_info *info, int first_q)
{
	if (first_q != 0)
	{
		if (info->tokens[first_q] != NULL)
		{
			info->tokens[first_q - 1] = ft_strjoinbas(info, info->tokens[first_q - 1], info->tokens[first_q]);
			free (info->tokens[first_q]);
			info->tokens[first_q] = NULL;
			realloc_copy(info, first_q, 1);
		}
	}
}

int	join_tokens(t_info *info, int pos)
{
	int	ret;

	ret = 0;
	if (info->tokens[pos + 1] != NULL)
	{
		if (info->tokens[pos + 1][0] == C_DQUOTE || info->tokens[pos + 1][0] == C_QUOTE
			|| check_char_token(info, pos + 1, 0) == C_NORMAL)
			joinwithbefore(info, pos + 1);
	}
	if (pos != 0)
	{
		if (info->tokens[pos - 1][0] == C_DQUOTE || info->tokens[pos - 1][0] == C_QUOTE
			|| check_char_token(info, pos - 1, 0) == C_NORMAL)
		{
			joinwithbefore(info, pos);
			ret++;
		}
	}
	return (ret);
}

void	joinwithnormalbefore(t_info *info, int first_q)
{
	if (first_q != 0)
	{
		if (info->tokens[first_q] != NULL)
		{
			if (check_char_token(info, first_q, 0) == C_NORMAL && check_char_token(info, first_q - 1, 0) == C_NORMAL)
			{
				info->tokens[first_q - 1] = ft_strjoinbas(info, info->tokens[first_q - 1], info->tokens[first_q]);
				free (info->tokens[first_q]);
				info->tokens[first_q] = NULL;
				realloc_copy(info, first_q, 1);
			}
		}
	}
}

int	check_empty_quotes(t_info *info, int first_q, int last_q)
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

void	realloc_copy(t_info *info, int start, int incr)
{
	while (info->tokens[start + incr] != NULL)
	{
		info->tokens[start] = (char *)malloc(1 + ft_strlen(info->tokens[start + incr]));
		if (info->tokens[start] == NULL)
		{
			printf("test7\n");
			ft_error(info, -1);
		}
		ft_strlcpy(info->tokens[start], info->tokens[start + incr], 1 + ft_strlen(info->tokens[start + incr]));
		info->token_state[start] = info->token_state[start + incr];
		free(info->tokens[start + incr]);
		info->tokens[start + incr] = NULL;
		start = start + incr;
		
	}
}

int	check_before_after(t_info *info, int first_q, int last_q)
{
	int	ret;
	int	i;

	ret = 0;
	if (first_q != 0)
	{
		if (info->tokens[first_q - 1] != NULL && check_char_token(info, first_q - 1, 0) == C_NORMAL)
		{
			ret++;
			if (last_q != i)
			{
				if (info->tokens[last_q + 1] != NULL && check_char_token(info, last_q + 1, 0) == C_NORMAL)
					ret++;
				return (ret);
			}
		}
	}
	if (last_q != i)
	{
		if (info->tokens[last_q + 1] != NULL && check_char_token(info, last_q + 1, 0) == C_NORMAL)
			ret = 3;
	}
	return (ret);
}

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
		return (-2);
	return (j);
}

void	merge_quotes(t_info *info, int first_q, int last_q, int n)
{
	int	i;

	if (n == 1 || n == 2)
		first_q--;
	if (n == 2 || n == 3)
		last_q++;
	i = first_q;
	while (i < last_q)
	{
		info->tokens[first_q] = ft_strjoinbas(info, info->tokens[first_q], info->tokens[first_q + 1]);
		free(info->tokens[first_q + 1]);
		info->tokens[first_q + 1] = NULL;
		realloc_copy(info, first_q + 1, 1);
		i++;
	}
}

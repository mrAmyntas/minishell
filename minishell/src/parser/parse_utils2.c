/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_utils2.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/17 11:26:27 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/10 19:37:11 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//3.230 Name
//In the shell command language, a word
//consisting solely of underscores, digits, 
//and alphabetics from the portable character set.
//The first character of a name is not a digit.
//https://pubs.opengroup.org/onlinepubs/
// 009695399/basedefs/xbd_chap03.html#tag_03_230
//https://pubs.opengroup.org/onlinepubs/
// 009695399/utilities/xcu_chap02.html#tag_02_06_02

static int	check_next_token(t_info *info, int i)
{
	int	j;

	if (info->tokens[i + 1] != NULL)
	{
		j = 0;
		while (info->tokens[i + 1][j] != '\0')
		{
			if (info->tokens[i + 1][j] == '\''
				|| info->tokens[i + 1][j] == '\"')
				return (1);
			j++;
		}
	}
	return (0);
}

static void	join_quoted_tokens2(t_info *info, int i)
{
	info->tokens[i] = ft_strjoinbas(info, info->tokens[i], info->tokens[i + 1]);
	free(info->tokens[i + 1]);
	info->tokens[i + 1] = NULL;
	realloc_copy(info, i + 1, 1);
}

// ret = 0 = no joining, 1 = join with next
void	join_quoted_tokens(t_info *info, int ret)
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
				ret = check_next_token(info, i);
				if (ret == 1)
				{
					join_quoted_tokens2(info, i);
					break ;
				}
			}
			j++;
		}
		if (ret == 1)
			continue ;
		i++;
	}
}

void	find_syntax_error(t_info *info)
{
	int	i;

	i = 0;
	while (info->tokens[i] != NULL)
	{
		if ((info->token_state[i] == 1 && check_char_token(info, i, 0)
				!= C_PIPE && info->token_state[i + 1] == 1)
			|| ((check_char_token(info, i, 0) == C_GREATER
					|| (check_char_token(info, i, 0) == C_LESSER)
					|| (check_char_token(info, i, 0) == C_DGREATER)
					|| (check_char_token(info, i, 0) == C_DLESSER))
				&& info->tokens[i + 1] == NULL))
		{
			if (info->tokens[i + 1] == NULL)
				set_error(info, 258, NULL, 0);
			else
				set_error(info, 258, NULL, i + 1);
			free_stuff(info);
			minishell(info);
			rl_clear_history();
			exit(0);
		}
		i++;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:31 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/25 13:42:27 by mgroen        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// to-do: unset needs to check valid identifiers

#include "../../inc/minishell.h"

static void	remove_quotes(t_info *info)
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

static void	remove_spaces(t_info *info)
{
	int	i;

	i = 0;
	while (info->tokens[i] != NULL)
	{
		if (info->tokens[i][0] == ' ' && info->tokens[i][1] == '\0')
		{
			free(info->tokens[i]);
			info->tokens[i] = NULL;
			if (info->tokens[i + 1] != NULL)
			{
				realloc_copy(info, i, 1);
				continue ;
			}
		}
		i++;
	}
}

static void	unclosed_pipe(t_info *info)
{
	int	i;

	i = 0;
	while (info->tokens[i] != NULL)
		i++;
	if (i != 0)
	{
		if (info->tokens[i - 1][0] == '|')
			ft_error(info, -3);
	}
}

// 3 = quoted | 1 = special char | 0 = normal chars
static void	set_token_state(t_info *info)
{
	int	i;

	i = 0;
	while (info->tokens[i] != NULL)
	{
		if (info->token_state[i] == 2)
		{
			i++;
			continue ;
		}
		if (info->tokens[i][0] == '\'' || info->tokens[i][0] == '\"')
			info->token_state[i] = 3;
		else if (check_char_token(info, i, 0) != C_NORMAL
			&& check_char_token(info, i, 0) != C_DOLLAR)
			info->token_state[i] = 1;
		else
			info->token_state[i] = 0;
		i++;
	}
}

void	parser(t_info *info)
{
	g_sig.exit_status2 = g_sig.exit_status;
	parse_quotes(info, 0);
	unclosed_pipe(info);
	find_dgreater_dlesser(info);
	expand(info);
	join_quoted_tokens(info, 0);
	remove_spaces(info);
	set_token_state(info);
	find_syntax_error(info);
	remove_quotes(info);
}

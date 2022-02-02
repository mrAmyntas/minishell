/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/21 11:21:03 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/02 15:07:29 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	expand_exitstatus(t_info *info, int i)
{
	free(info->tokens[i]);
	info->tokens[i] = NULL;
	info->tokens[i] = ft_itoa(info->exit_status);
	free(info->tokens[i + 1]);
	info->tokens[i + 1] = NULL;
	realloc_copy(info, i + 1, 1);
}

static void	remove_dollar(t_info *info, int i)
{
	free(info->tokens[i]);
	info->tokens[i] = NULL;
	realloc_copy(info, i, 1);
}

static void	check_after_dollar(t_info *info, int i)
{
	if (info->tokens[i + 1] != NULL)
	{
		if (info->tokens[i + 1][0] == '?')
			expand_exitstatus(info, i);
		else if (check_char_token(info, i + 1, 0) == C_NORMAL)
			expand_token_dollar(info, i);
		else if (check_char_token(info, i + 1, 0) == C_QUOTE
			|| check_char_token(info, i + 1, 0) == C_DQUOTE)
			remove_dollar(info, i);
	}
}

// if i+1 == NULL (ret=-1) -> leave $ untouched
// if i+1 == quotes (ret 2) -> remove $
void	expand(t_info *info)
{
	int	i;
	int	j;

	i = 0;
	while (info->tokens[i] != NULL)
	{
		j = 0;
		while (info->tokens[i][j] != '\0')
		{
			if (info->tokens[i][j] == '\"')
			{
				check_dollar_in_quotes(info, i);
				break ;
			}
			j++;
		}
		if (info->tokens[i][0] == '$')
		{
			check_after_dollar(info, i);
			i = i - join_tokens(info, i);
		}
		i++;
	}
}

/*void	expand(t_info *info)
{
	int	i;
	int	j;

	i = 0;
	while (info->tokens[i] != NULL)
	{
		j = 0;
		while (info->tokens[i][j] != '\0')
		{
			if (info->tokens[i][j] == '\"')
			{
				check_dollar_in_quotes(info, i);
				break ;
			}
			j++;
		}
		if (info->tokens[i][0] == '$')
		{
			if (check_after_dollar(info, i) == 0)
				expand_token_dollar(info, i);
			else if (check_after_dollar(info, i) == 1)
				expand_exitstatus(info, i);
			else if (check_after_dollar(info, i) == 2)
				remove_dollar(info, i);
			i = i - join_tokens(info, i);
		}
		i++;
	}
}*/
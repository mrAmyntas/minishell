/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_utils2.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/17 11:26:27 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/02 15:45:39 by bhoitzin      ########   odam.nl         */
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

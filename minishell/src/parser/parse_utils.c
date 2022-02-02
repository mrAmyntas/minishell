/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/17 11:17:21 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/02 14:27:07 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_strjoinbas(t_info *info, char *s1, char const *s2)
{
	char	*buff;
	int		j;
	int		i;

	if ((s1 == NULL) || (s2 == NULL))
		return (NULL);
	buff = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (buff == NULL)
		ft_error(info, -1);
	j = 0;
	while (s1[j] != '\0')
	{
		buff[j] = s1[j];
		j++;
	}
	i = j;
	while (s2[j - i] != '\0')
	{
		buff[j] = s2[j - i];
		j++;
	}
	buff[j] = '\0';
	free(s1);
	s1 = NULL;
	return (buff);
}

void	joinwithbefore(t_info *info, int first_q)
{
	if (first_q != 0)
	{
		if (info->tokens[first_q] != NULL)
		{
			info->tokens[first_q - 1] = ft_strjoinbas(info,
					info->tokens[first_q - 1], info->tokens[first_q]);
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
		if (info->tokens[pos + 1][0] == C_DQUOTE || info->tokens[pos + 1][0]
			== C_QUOTE || check_char_token(info, pos + 1, 0) == C_NORMAL)
			joinwithbefore(info, pos + 1);
	}
	if (pos != 0)
	{
		if (info->tokens[pos - 1][0] == C_DQUOTE || info->tokens[pos - 1][0]
			== C_QUOTE || check_char_token(info, pos - 1, 0) == C_NORMAL)
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
			if (check_char_token(info, first_q, 0) == C_NORMAL
				&& check_char_token(info, first_q - 1, 0) == C_NORMAL)
			{
				info->tokens[first_q - 1] = ft_strjoinbas(info,
						info->tokens[first_q - 1], info->tokens[first_q]);
				free (info->tokens[first_q]);
				info->tokens[first_q] = NULL;
				realloc_copy(info, first_q, 1);
			}
		}
	}
}

void	realloc_copy(t_info *info, int start, int incr)
{
	while (info->tokens[start + incr] != NULL)
	{
		info->tokens[start] = (char *)malloc(1
				+ ft_strlen(info->tokens[start + incr]));
		if (info->tokens[start] == NULL)
			ft_error(info, -1);
		ft_strlcpy(info->tokens[start], info->tokens[start + incr],
			1 + ft_strlen(info->tokens[start + incr]));
		info->token_state[start] = info->token_state[start + incr];
		free(info->tokens[start + incr]);
		info->tokens[start + incr] = NULL;
		start = start + incr;
	}
}

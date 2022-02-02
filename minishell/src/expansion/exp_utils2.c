/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exp_utils2.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/02 14:51:35 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/02 18:10:55 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*get_name(t_info *info, int i, int j)
{
	char	*str;
	int		k;

	k = check_name(info, i, j);
	if (k == j)
		return (NULL);
	str = (char *)malloc(k - j + 1);
	ft_strlcpy2(str, info->tokens[i], k - j + 1, j);
	return (str);
}

char	*get_rest(t_info *info, int i, int j)
{
	char	*str;
	int		k;

	k = check_name(info, i, j);
	str = (char *)malloc(ft_strlen(info->tokens[i]) + 2);
	ft_strlcpy2(str, info->tokens[i], ft_strlen(info->tokens[i]) - k + 1, k);
	if (j != 0)
	{
		k = ft_strlen(str);
		str[k + 2] = '\0';
		while (k >= 0)
		{
			str[k + 1] = str[k];
			k--;
		}
		str[0] = '\"';
	}
	return (str);
}

static void	remove_dollar_and_name(t_info *info, char *rest, int i)
{
	free (info->tokens[i]);
	info->tokens[i] = NULL;
	realloc_copy(info, i, 1);
	free (info->tokens[i]);
	info->tokens[i] = NULL;
	if (rest != NULL)
	{
		info->tokens[i] = (char *)malloc(sizeof(char) * ft_strlen(rest) + 1);
		ft_strlcpy(info->tokens[i], rest, ft_strlen(rest) + 1);
		free(rest);
	}
	else
		realloc_copy(info, i, 1);
}

static void	expand_and_merge(t_info *info, char *name, char *rest, int i)
{
	free(info->tokens[i]);
	info->tokens[i] = (char *)malloc(ft_strlen(name) + 1);
	ft_strlcpy(info->tokens[i], name, ft_strlen(name) + 1);
	if (rest != NULL)
	{
		info->tokens[i] = ft_strjoinbas(info, info->tokens[i], rest);
		free(rest);
	}
	free(info->tokens[i + 1]);
	info->tokens[i + 1] = NULL;
	realloc_copy(info, i + 1, 1);
}

void	expand_token_dollar(t_info *info, int i)
{
	char	*name;
	char	*rest;
	char	*temp;
	int		pos;

	pos = check_name(info, i + 1, 0);
	if (pos == 0)
	{
		free (info->tokens[i]);
		info->tokens[i] = NULL;
		realloc_copy(info, i, 1);
		cut_dollar(info, i, 0, 0);
		return ;
	}
	temp = get_name(info, i + 1, 0);
	rest = get_rest(info, i + 1, 0);
	name = get_val(info, temp);
	free(temp);
	if (name == NULL)
		remove_dollar_and_name(info, rest, i);
	else
		expand_and_merge(info, name, rest, i);
}

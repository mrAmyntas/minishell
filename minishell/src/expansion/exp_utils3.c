/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exp_utils3.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/02 15:50:02 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/09 18:39:06 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

size_t	ft_strlcpy2(char *dest, const char *src, size_t dstsize, int start)
{
	size_t	i;

	if (src == NULL)
		return (0);
	i = 0;
	if (dstsize > 0)
	{
		while ((i < dstsize - 1) && (src[start] != '\0'))
		{
			dest[i] = src[start];
			i++;
			start++;
		}
		dest[i] = '\0';
	}
	return (ft_strlen(src));
}

// j = pos $ n = j + 2
static void	expand_ex_status_str(t_info *info, int i, int j, int n)
{
	char	*temp;
	char	*str;
	int		k;

	temp = realloc_token(info, i, 10);
	str = ft_itoa(g_sig.exit_status);
	k = 0;
	while (str[k] != '\0')
	{
		info->tokens[i][j] = str[k];
		k++;
		j++;
	}
	while (temp[n] != '\0')
	{
		info->tokens[i][j] = temp[n];
		n++;
		j++;
	}
	info->tokens[i][j] = '\0';
	info->token_state[i] = 2;
	free(str);
	str = NULL;
	free(temp);
	temp = NULL;
}

void	check_dollar_in_quotes(t_info *info, int i)
{
	int	j;

	j = 0;
	while (info->tokens[i][j] != '\0')
	{
		if (info->tokens[i][j] == '\"')
		{
			j++;
			while (info->tokens[i][j] != '\"')
			{
				if (info->tokens[i][j] == '$' && info->tokens[i][j + 1] == '?')
					expand_ex_status_str(info, i, j, j + 2);
				if (info->tokens[i][j] == '$')
					expand_str_dollar(info, i, j);
				j++;
			}
		}
		j++;
	}
}

void	update_expand_exit_status(t_info *info)
{
	int	i;

	i = 0;
	while (info->tokens[i] != NULL)
	{
		if (info->token_state[i] == 2)
		{
			free(info->tokens[i]);
			info->tokens[i] = NULL;
			info->tokens[i] = ft_itoa(g_sig.exit_status);
		}
		i++;
	}
}

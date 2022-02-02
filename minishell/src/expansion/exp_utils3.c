/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exp_utils3.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/02 15:50:02 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/02 17:41:45 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"


static void	expand_ex_status_str(t_info *info, int i, int j, int n) // j = pos $ n = j + 2
{
	char	*temp;
	char	*str;
	int		k;

	temp = realloc_token(info, i, 10);
	str = ft_itoa(info->exit_status);
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
	free(str);
	str = NULL;
	free(temp);
	temp = NULL;
}

void	check_dollar_in_quotes(t_info *info, int i)
{
	int j;

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

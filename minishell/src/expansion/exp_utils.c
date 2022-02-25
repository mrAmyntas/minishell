/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exp_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/02 14:51:38 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/24 11:22:00 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*realloc_token(t_info *info, int i, int len)
{
	char	*temp;

	if (len < 0)
		len = 0;
	len = ft_strlen(info->tokens[i]) + len + 1;
	temp = (char *)malloc(sizeof(char) * len);
	if (temp == NULL)
		ft_error(info, -1);
	ft_strlcpy(temp, info->tokens[i], ft_strlen(info->tokens[i]) + 1);
	free(info->tokens[i]);
	info->tokens[i] = (char *)malloc(sizeof(char) * len);
	if (info->tokens[i] == NULL)
		ft_error(info, -1);
	ft_strlcpy(info->tokens[i], temp, ft_strlen(temp) + 1);
	return (temp);
}

int	check_name(t_info *info, int i, int j)
{
	if ((info->tokens[i][j] >= 'a' && info->tokens[i][j] <= 'z') ||
		(info->tokens[i][j] >= 'A' && info->tokens[i][j] <= 'Z') ||
		info->tokens[i][j] == '_')
	{
		while ((info->tokens[i][j] >= '0' && info->tokens[i][j] <= '9') ||
			(info->tokens[i][j] >= 'a' && info->tokens[i][j] <= 'z') ||
			(info->tokens[i][j] >= 'A' && info->tokens[i][j] <= 'Z') ||
			info->tokens[i][j] == '_')
			j++;
	}
	return (j);
}

void	expand_str_dollar3(t_info *info, int i, char *name, int end)
{
	int		j;
	int		start;
	char	*temp;

	start = 0;
	while (info->tokens[i][start] != '$')
		start++;
	temp = realloc_token(info, i, ft_strlen(name) - (end - start + 1));
	j = 0;
	while (name[j] != '\0')
	{
		info->tokens[i][start] = name[j];
		j++;
		start++;
	}
	while (temp[end + 1] != '\0')
	{
		info->tokens[i][start] = temp[end + 1];
		end++;
		start++;
	}
	info->tokens[i][start] = '\0';
	free(temp);
	temp = NULL;
}

//cut_dollar: cut the $+normal chars i.e. token[i] = rest
// else:replace $+normal with name
void	expand_str_dollar2(t_info *info, int i, int start, int pos)
{
	char	*name;
	char	*temp;

	temp = get_name(info, i, start);
	name = get_val(info, temp);
	free(temp);
	temp = NULL;
	if (name == NULL)
		cut_dollar(info, i, start - 1, pos - 1);
	else
		expand_str_dollar3(info, i, name, pos - 1);
}

//read normal chars until find a non-normal char, cant start with a digit
//pos is then the first illegal char or the last ", start the start (not $)
void	expand_str_dollar(t_info *info, int i, int pos)
{
	int	start;

	start = pos + 1;
	pos = check_name(info, i, start);
	if (start == pos)
	{
		if (info->tokens[i][pos] == '\"')
			return ;
		cut_dollar(info, i, start - 1, pos);
		return ;
	}
	if (start <= pos)
		expand_str_dollar2(info, i, start, pos);
}

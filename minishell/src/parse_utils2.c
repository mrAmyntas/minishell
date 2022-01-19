/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_utils2.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/17 11:26:27 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/01/19 16:36:43 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*ft_strjoinbas(char *s1, char const *s2)
{
	char	*buff;
	int		x;
	int		i;

	if ((s1 == NULL) || (s2 == NULL))
		return (NULL);
	x = ft_strlen(s1) + ft_strlen(s2) + 1;
	buff = (char*)malloc(sizeof(char) * x);
	if (buff == NULL)
		ft_error(1);
	x = 0;
	while (s1[x] != '\0')
	{
		buff[x] = s1[x];
		x++;
	}
	i = 0;
	while (s2[i] != '\0')
	{
		buff[x] = s2[i];
		i++;
		x++;
	}
	buff[x] = '\0';
	free(s1);
	s1 = NULL;
	return (buff);
}

//3.230 Name
//In the shell command language, a word consisting solely of underscores, digits, 
//and alphabetics from the portable character set. The first character of a name is not a digit.
// https://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_230
// https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html#tag_02_06_02

void	cut_dollar(t_info *info, int i, int start, int end)
{
	int	j;

	j = 0;
	while (start <= end)
	{
		info->tokens[i][start] = info->tokens[i][end + 1 + j];
		if (info->tokens[i][start] == '\0')
			return ;
		start++;
		j++;
	}
}

static char	*realloc_token(t_info *info, int i, int len)
{
	char *temp;

	if (len < 0)
		len = 0;
	temp = (char *)malloc(sizeof(char) * (ft_strlen(info->tokens[i]) + len + 1));
	if (temp == NULL)
		ft_error(1);
	ft_strlcpy(temp, info->tokens[i], ft_strlen(info->tokens[i]) + 1);
	printf("temp:%s\n", temp);
	free (info->tokens[i]);
	info->tokens[i] = (char *)malloc(sizeof(char) * (ft_strlen(info->tokens[i]) + len + 1));
	if (info->tokens[i] == NULL)
		ft_error(1);
	ft_strlcpy(info->tokens[i], temp, ft_strlen(temp));
	printf("temp:%s\n", temp);
	return (temp);
}

void	expand_str_dollar3(t_info *info, int i, char *name, int end)
{
	int		j;
	int		start;
	char	*temp;

	start = 0;
	while (info->tokens[i][start] != '$')
		start++;
	temp = realloc_token(info, i, ft_strlen(name) - end - start + 1);
	j = 0;
//	printf("1:%s\n", info->tokens[i]);
//	printf("temp:%s\n", temp);
	while (name[j] != '\0')
	{
		info->tokens[i][start] = name[j];
		j++;
		start++;
	}
//	printf("2:%s\n", info->tokens[i]);
	while (temp[end + 1] != '\0')
	{
		info->tokens[i][start] = temp[end + 1];
		end++;
		start++;
	}
	info->tokens[i][start] = '\0';
//	printf("3:%s\n", info->tokens[i]);
}

void	remove_quotes(t_info *info)
{
	int	i;
	int	j;

	i = 0;
	while (info->tokens[i] != NULL)
	{
		if (info->tokens[i][0] == '\'' || info->tokens[i][0] == '\"')
			trim_quotes(info, i, ft_strlen(info->tokens[i]));
		i++;
	}
}
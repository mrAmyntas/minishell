/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_utils2.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/17 11:26:27 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/01/28 15:54:33 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*ft_strjoinbas(t_info *info, char *s1, char const *s2)
{
	char	*buff;
	int		x;
	int		i;

	if ((s1 == NULL) || (s2 == NULL))
		return (NULL);
	x = ft_strlen(s1) + ft_strlen(s2) + 1;
	buff = (char*)malloc(sizeof(char) * x);
	if (buff == NULL)
	{
		printf("test8\n");
		ft_error(info, -1);
	}
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
	while (info->tokens[i][end + 1 + j] != '\0')
	{
		info->tokens[i][start] = info->tokens[i][end + 1 + j];
		start++;
		j++;
	}
	info->tokens[i][start] = '\0';
}


static int	check_next_token(t_info *info, int i)
{
	int	j;

	if (info->tokens[i + 1] != NULL)
	{
		j = 0;
		while (info->tokens[i + 1][j] != '\0')
		{
			if (info->tokens[i + 1][j] == '\'' || info->tokens[i + 1][j] == '\"')
				return (1);
			j++;
		}
	}
	return (0);
}

void	join_quoted_tokens2(t_info *info, int i)
{
	info->tokens[i] = ft_strjoinbas(info, info->tokens[i], info->tokens[i + 1]);
	free(info->tokens[i + 1]);
	info->tokens[i + 1] = NULL;
	realloc_copy(info, i + 1, 1);
}

void	join_quoted_tokens(t_info *info)
{
	int	i;
	int	j;
	int	ret;

	ret = 0;
	i = 0;
	while (info->tokens[i] != NULL)
	{
		j = 0;
		while (info->tokens[i][j] != '\0')
		{
			if (info->tokens[i][j] == '\'' || info->tokens[i][j] == '\"')
			{
				ret = check_next_token(info, i); // 0 = no joining, 1 = join with next
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

int	check_name(t_info *info, int i, int j)
{
	if ((info->tokens[i][j] >= 'a' && info->tokens[i][j] <= 'z') ||
		(info->tokens[i][j] >= 'A' && info->tokens[i][j] <= 'Z') ||
		info->tokens[i][j] == '_')
	{
		while ((info->tokens[i][j] >= '0' && info->tokens[i][j] <= '9') || 
			(info->tokens[i][j] >= 'a'&& info->tokens[i][j] <= 'z') ||
			(info->tokens[i][j] >= 'A' && info->tokens[i][j] <= 'Z') ||
			info->tokens[i][j] == '_')
			j++;
	}
	return (j);
}

void	check_nosuchdir(t_info *info)
{
	int	i;
	DIR	*ret;

	i = 0;
	while (info->tokens[i] != NULL)
	{
		if ((ft_strncmp(info->tokens[i], "cd", 2) == 0 && ft_strlen(info->tokens[i]) == 2))
		{
			ret = opendir(info->tokens[i + 1]);
			if (ret == NULL)
			{
				set_error(info, 1, info->tokens[i + 1]);
				ft_error(info, 1);
				ft_free(info);
				minishell(info);
				rl_clear_history();
				exit(1);
			}
			closedir(ret);
		}
		i++;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exp_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/02 14:51:38 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/02 14:56:40 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*realloc_token(t_info *info, int i, int len)
{
	char *temp;

	if (len < 0)
		len = 0;
	len = ft_strlen(info->tokens[i]) + len + 1;
	temp = (char *)malloc(sizeof(char) * len);
	if (temp == NULL)
	{
		printf("test1\n");
		ft_error(info, -1);
	}
	ft_strlcpy(temp, info->tokens[i], ft_strlen(info->tokens[i]) + 1);
	free (info->tokens[i]);
	info->tokens[i] = (char *)malloc(sizeof(char) * len);
	if (info->tokens[i] == NULL)
	{
		printf("test2\n");
		ft_error(info, -1);
	}
	ft_strlcpy(info->tokens[i], temp, ft_strlen(temp) + 1);
//	printf("after realloc_token:\ntemp:%s\ninfo->tokens[i]:%s\n", temp, info->tokens[i]);
	return (temp);
}

void	expand_ex_status_str(t_info *info, int i, int j, int n) // j = pos $ n = j + 2
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

void	add_dquotes(t_info *info, char *buf, int len, int i)
{
	int	j;

	free(info->tokens[i]);
	info->tokens[i] = NULL;
	info->tokens[i] = (char *)malloc(sizeof(char) * len + 3);
	info->tokens[i][0] = '\"';
	info->tokens[i][len + 1] = '\"';
	info->tokens[i][len + 2] = '\0';
	j = 0;
	while (buf[j] != '\0')
	{
		info->tokens[i][j + 1] = buf[j];
		j++;
	}
}

void	reset_token(t_info *info, int i)
{
	int	j;

	free(info->tokens[i]);
	info->tokens[i] = NULL;
	info->tokens[i] = (char *)malloc(sizeof(char) * 3);
	info->tokens[i][0] = '<';
	info->tokens[i][1] = '<';
	info->tokens[i][2] = '\0';
	j = 0;
}

char	*expand_buf(t_info *info, char *buf, int i)
{
	int	j;

	if (info->token_state[i + 1] == 0)
	{
		add_dquotes(info, buf, ft_strlen(buf), i); // make info->tokens[i] = buf with dquotes around it
		check_dollar_in_quotes(info, i);
		free(buf);
		buf = NULL;
		buf = (char *)malloc(sizeof(char) * (ft_strlen(info->tokens[i]) - 1));
		j = 0;
		while (info->tokens[i][j + 2] != '\0')
		{
			buf[j] = info->tokens[i][j + 1];
			j++;
		}
		buf[j] = '\0';
		reset_token(info, i); // turn info->tokens[i] back into heredoc '<<'
	}
	return (buf);
}

static size_t	ft_strlcpy2(char *dest, const char *src, size_t dstsize, int start)
{
	size_t i;

//	printf("cpy2:\nsize:%ld start:%d\n", dstsize, start);
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

void	expand_token_dollar(t_info *info, int i)
{
	char	*name;
	char	*rest;
	char	*temp;
	int		pos;

	pos = check_name(info, i + 1, 0);
//	printf("pos:%d\n", pos);
	if (pos == 0)
	{
		free (info->tokens[i]);
		info->tokens[i] = NULL;
		realloc_copy(info, i, 1);
		cut_dollar(info, i, 0, 0);
		return ;
	}
	temp = get_name(info, i + 1, 0);
//	printf("name:%s\n", name);
	rest = get_rest(info, i + 1, 0); // (e.g. $USER'kek')
	name = get_val(info, temp);
	free(temp);
//	printf("name:%s rest:%s\n", name, rest);
	if (name == NULL)
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
	else
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
}

void	expand_str_dollar3(t_info *info, int i, char *name, int end)
{
	int		j;
	int		start;
	char	*temp;

	start = 0;
	while (info->tokens[i][start] != '$')
		start++;
//	printf("ft_strlen(name):%d - end:%d - start:%d + 1 = %d\n", ft_strlen(name), end, start, (ft_strlen(name) - (end - start + 1)));
	temp = realloc_token(info, i, ft_strlen(name) - (end - start + 1));
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
	free(temp);
//	printf("3:%s\n", info->tokens[i]);
}
void	expand_str_dollar2(t_info *info, int i, int start, int pos)
{
	char	*name;
	char	*rest;
	char	*temp;

	temp = get_name(info, i, start);
//	printf("name:%s\n", name);
	name = get_val(info, temp);
	free(temp);
//	printf("name:%s\n", name);
//	printf("name:%s rest:%s\n", name, rest);
	if (name == NULL)
	{
		//cut the $+normal chars i.e. token[i] = rest
		cut_dollar(info, i, start - 1, pos - 1);
	}
	else
	{
		//replace $+normal with name
		expand_str_dollar3(info, i, name, pos - 1);
	}
}

void	expand_str_dollar(t_info *info, int i, int pos)
{
	int start;

	start = pos + 1;
	//read normal chars until find a non-normal char, cant start with a digit
	pos = check_name(info, i, start);
	//pos is now the first illegal char or the last ", start the start (not $)
//	printf("start:%d, pos:%d\n", start, pos);
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expansion.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/21 11:21:03 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/01/21 15:47:12 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	*realloc_token(t_info *info, int i, int len)
{
	char *temp;

	if (len < 0)
		len = 0;
	temp = (char *)malloc(sizeof(char) * (ft_strlen(info->tokens[i]) + len + 1));
	if (temp == NULL)
		ft_error(1);
	ft_strlcpy(temp, info->tokens[i], ft_strlen(info->tokens[i]) + 1);
	free (info->tokens[i]);
	info->tokens[i] = (char *)malloc(sizeof(char) * (ft_strlen(info->tokens[i]) + len + 1));
	if (info->tokens[i] == NULL)
		ft_error(1);
	ft_strlcpy(info->tokens[i], temp, ft_strlen(temp));
	return (temp);
}

void	remove_dollar(t_info *info, int i)
{
	free(info->tokens[i]);
	info->tokens[i] = NULL;
	realloc_copy(info, i, 1);
}

void	expand_exitstatus(t_info *info, int i)
{

}

static size_t	ft_strlcpy2(char *dest, const char *src, size_t dstsize, int start)
{
	size_t i;

	//printf("cpy2:\nsize:%ld start:%d\n", dstsize, start);
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
	name = get_name(info, i + 1, 0);
//	printf("name:%s\n", name);
	rest = get_rest(info, i + 1, 0); // (e.g. $USER'kek')
	name = get_val(info, name);
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
			info->tokens[i] = ft_strjoinbas(info->tokens[i], rest);
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
	temp = realloc_token(info, i, ft_strlen(name) - end - start + 1);
	j = 0;
	//printf("1:%s\n", info->tokens[i]);
	//printf("temp:%s\n", temp);
	while (name[j] != '\0')
	{
		info->tokens[i][start] = name[j];
		j++;
		start++;
	}
	//printf("2:%s\n", info->tokens[i]);
	while (temp[end + 1] != '\0')
	{
		info->tokens[i][start] = temp[end + 1];
		end++;
		start++;
	}
	info->tokens[i][start] = '\0';
	//printf("3:%s\n", info->tokens[i]);
}
void	expand_str_dollar2(t_info *info, int i, int start, int pos)
{
	char	*name;
	char	*rest;

	name = get_name(info, i, start);
	printf("name:%s\n", name);
//	rest = get_rest(info, i, start);
	name = get_val(info, name);
	printf("name:%s\n", name);
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
	printf("start:%d, pos:%d\n", start, pos);
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
//3.230 Name
//In the shell command language, a word consisting solely of underscores, digits, 
//and alphabetics from the portable character set. The first character of a name is not a digit.

int	check_after_dollar(t_info *info, int i)
{
	if (info->tokens[i + 1] != NULL)
	{
		if (info->tokens[i + 1][0] == '?')
			return (1);
		if (check_char_token(info, i + 1, 0) == C_NORMAL)
			return (0);
		if (check_char_token(info, i + 1, 0) == C_QUOTE || check_char_token(info, i + 1, 0) == C_DQUOTE)
			return (2);
	}
	return (-1);
}

void	check_dollar_in_quotes(t_info *info, int i)
{
	int j;
	int	k;

	j = 0;
	while (info->tokens[i][j] != '\0')
	{
		if (info->tokens[i][j] == '\"')
		{
			j++;
			k = j;
			while (info->tokens[i][j] != '\"')
			{
				if (info->tokens[i][j] == '$')
					expand_str_dollar(info, i, k);
				j++;
			}
		}
		j++;
	}
}

void	expansion(t_info *info)
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
				expand_token_dollar(info, i); // if i+1 == NULL (ret=-1) -> leave $ untouched
			else if (check_after_dollar(info, i) == 1) // if i+1 == quotes (ret 2) -> remove $
				expand_exitstatus(info, i);
			//else if (check_after_dollar(info, i) == -1)
			//	join_quoted_tokens2(info, i);
			else if (check_after_dollar(info, i) == 2)
				remove_dollar(info, i);
			i = i - join_tokens(info, i);
		}
		i++;
	}
}

/* void	check_dollar_in_quotes(t_info *info, int i)
{
	int j;

	j = 0;
	printf("check\n");
	while (info->tokens[i][j] != '\0')
	{
		if (info->tokens[i][j] == '\'')
		{
			while (info->tokens[i][j] != '\'')
				j++;
		}
		if (info->tokens[i][j] == '\"')
		{
			while (info->tokens[i][j + 1] != '$' && info->tokens[i][j + 1] != '\"')
				j++;
			printf("j:%d\n", j);
			if (info->tokens[i][j + 1] == '$')
				expand_str_dollar(info, i, j);
		}
		j++;
	}
}*/
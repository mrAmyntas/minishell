/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:31 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/01/20 13:04:12 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


void	remove_spaces(t_info *info)
{
	int	i;

	i = 0;
	while (info->tokens[i] != NULL)
	{
		if (info->tokens[i][0] == ' ' && info->tokens[i][1] == '\0')
		{
			if (info->tokens[i + 1] != NULL)
			{
				realloc_copy(info, i, 1);
				continue ;
			}
		}
		i++;
	}
}

void	set_token_state(t_info *info)
{
	int	i;

	i = 0;
	while (info->tokens[i] != NULL)
	{
		if (info->tokens[i][0] == '\'' || info->tokens[i][0] == '\"')
			info->token_state[i] = 0; // 0 = non-special-chars
		else if (check_char_token(info, i, 0) != C_NORMAL)
			info->token_state[i] = 1; // 1 = special char
//		else
//			info->token_state[i] = 3; // 3 = normal chars
		i++;
	}
}

void	expand_exitstatus(t_info *info, int i)
{

}

static size_t	ft_strlcpy2(char *dest, const char *src, size_t dstsize, int start)
{
	size_t i;

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
	int		temp;

	temp = j;
	while (check_char_token(info, i, j) == C_NORMAL && info->tokens[i][j] != '\0')
		j++;
	str = (char *)malloc(j + 1);
	j = j - temp;
	ft_strlcpy2(str, info->tokens[i], j + 1, temp);
	return (str);
}

char	*get_rest(t_info *info, int i, int j)
{
	char	*str;
	int		k;

	k = 0;
	while (check_char_token(info, i, j) == C_NORMAL && info->tokens[i][j] != '\0')
		j++;
	if (info->tokens[i][j] == '\0')
		return (NULL);
	k = j;
	while (info->tokens[i][k] != '\0')
		k++;
	str = (char *)malloc(k + 1);
	ft_strlcpy2(str, info->tokens[i], k - j - 1, j + 1);
	return (str);
}

void	expand_token_dollar(t_info *info, int i)
{
	char	*name;
	char	*rest;

	if (info->tokens[i + 1] == NULL)
		return ;
	name = get_name(info, i + 1, 0);
	rest = get_rest(info, i + 1, 0); // (e.g. $USER'kek')
	name = get_val(info, name);
	if (name == NULL)
	{
		free (info->tokens[i]);
		info->tokens[i] = NULL;
		realloc_copy(info, i, 1);
		free (info->tokens[i]);
		info->tokens[i] = NULL;
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

void	expand_str_dollar2(t_info *info, int i, int start, int pos)
{
	char	*name;
	char	*rest;

	name = get_name(info, i, start);
	//printf("name:%s\n", name);
	rest = get_rest(info, i, start);
	name = get_val(info, name);
	//printf("name:%s\n", name);
	if (name == NULL)
	{
		//cut the $+normal chars
		cut_dollar(info, i, start - 1, pos);
	}
	else
	{
		//replace $+normal with name
		expand_str_dollar3(info, i, name, pos);
	}
}

void	expand_str_dollar(t_info *info, int i, int pos)
{
	int start;

	start = pos + 1;
	//read normal chars until find a non-normal char, cant start with a digit
	if (check_char_token(info, i, pos + 1) == C_NORMAL && ft_isdigit(info->tokens[i][pos + 1]) == 0)
	{
		while (info->tokens[i][pos + 1] != '\"')
		{
			if (check_char_token(info, i, pos + 1) != C_NORMAL)
				break ;
			pos++;
		}
	} // pos is now the last part of the Name, start the start (not $)
	//printf("start:%d, pos:%d\n", start, pos);
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

	j = 0;
	while (info->tokens[i][j] != '\0')
	{
		if (info->tokens[i][j] == '$')
			expand_str_dollar(info, i, j);
		j++;
	}
}

void	check_dollar_token(t_info *info)
{
	int	i;
	int	j;

	i = 0;
	int p = 0;
	//printf("in check_d_t\n");
	while (info->tokens[p] != NULL)
	{
		//printf("stored = %s\n", info->tokens[p]);
		p++;
	}
	while (info->tokens[i] != NULL)
	{
		if (info->tokens[i][0] == '\"')
			check_dollar_in_quotes(info, i);
		else if (info->tokens[i][0] == '$')
		{
			if (check_after_dollar(info, i) == 0)
				expand_token_dollar(info, i); // if i+1 == NULL -> leave $ untouched
			if (check_after_dollar(info, i) == 1) // if i+1 == quotes (ret 2) -> remove $
				expand_exitstatus(info, i);
			i = i - join_tokens(info, i);
		}
		i++;
	}
}

int	parse_quotes(t_info *info, int i)
{
	int	n;
	int	ret;

	while (info->tokens[i] != NULL)
	{
		ret = -1;
		if (info->tokens[i][0] == C_QUOTE || info->tokens[i][0] == C_DQUOTE)
		{
			ret = check_unclosed(info, i, i + 1); // i is pos quote 1, ret is pos quote 2
			if (ret == -1)
				return (-1);
			if (check_empty_quotes(info, i, ret) == 1) //removes the empty quotes from tokens
				continue ;
			n = check_before_after(info, i, ret); //n == 0: no normal chars before or after quotes  n == 1, only before, n = 2, after & before n = 3 only after
			merge_quotes(info, i, ret, n);
			if (n == 1 || n == 2)
				continue ;
		}
		i++;
	}
	return (0);
}

void	find_dgreater_dlesser(t_info *info)
{
	int	i;

	i = 0;
	while (info->tokens[i] != NULL && info->tokens[i + 1] != NULL)
	{
		if (info->tokens[i][0] == '<' && info->tokens[i + 1][0] == '<')
		{
			free(info->tokens[i]);
			info->tokens[i] = (char *)malloc(sizeof(char) * 3);
			info->tokens[i][0] = '<';
			info->tokens[i][1] = '<';
			info->tokens[i][2] = '\0';
			free(info->tokens[i + 1]);
			info->tokens[i + 1] = NULL;
			realloc_copy(info, i + 1, 1);
		}
		if (info->tokens[i][0] == '>' && info->tokens[i + 1][0] == '>')
		{
			free(info->tokens[i]);
			info->tokens[i] = (char *)malloc(sizeof(char) * 3);
			info->tokens[i][0] = '>';
			info->tokens[i][1] = '>';
			info->tokens[i][2] = '\0';
			free(info->tokens[i + 1]);
			info->tokens[i + 1] = NULL;
			realloc_copy(info, i + 1, 1);
		}
		i++;
	}
}
// to-do: $ expansion in a quoted "" string + meerdere quoted strings achte elkaar plakken (e.g. 'echo''hoi')
// + ${}  ??
// crash bij export PID=$$
// p'w'd -> quotes niet aan uiteinden ook verwijderen
// Name = not just 'not special chars' but also not other none-alphanumerics
int	parser(t_info *info)
{
	int	ret;

	ret = parse_quotes(info, 0);
	if (ret == -1)
	{
		printf("checkret-1\n");
		return (-1);
	}
	find_dgreater_dlesser(info);
	int p = 0;
	//printf("after quotes\n");
	while (info->tokens[p] != NULL)
	{
		//printf("stored = %s\n", info->tokens[p]);
		p++;
	}
	check_dollar_token(info);
	//printf("check\n");
	//printf("after dollar\n");
	p = 0;
	while (info->tokens[p] != NULL)
	{
		//printf("stored = %s\n", info->tokens[p]);
		p++;
	}
	//printf("check\n");
	join_quoted_tokens(info);
	//printf("after join_quoted_tokens\n");
	p = 0;
	while (info->tokens[p] != NULL)
	{
		//printf("stored = %s\n", info->tokens[p]);
		p++;
	}
	remove_spaces(info);
	set_token_state(info);
	remove_quotes(info);
	printf("after parser\n");
	p = 0;
	while (info->tokens[p] != NULL)
	{
		printf("stored = %s  state = %d\n", info->tokens[p], info->token_state[p]);
		p++;
	}
	return (0);
}

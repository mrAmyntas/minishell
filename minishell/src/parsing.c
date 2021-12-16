/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:31 by bhoitzin      #+#    #+#                 */
/*   Updated: 2021/12/16 18:33:08 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	*ft_strjoinbas(char *s1, char const *s2)
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

int	checkquotes(t_info *info, int i, int store_pos)
{
	while(i - 1 >= 0)
	{
		if (info->tokens[i][0] == C_QUOTE || info->tokens[i][0] == C_DQUOTE)
			break ;
		i--;
	}
	if (i == 0)
		return (0);
	return (0);
}

void	expand_dollar(t_info *info, int pos_dollar, int *ret)
{
	char	*name;
	int		i;

	name = get_val(info, info->tokens[pos_dollar + 1]);
	if (name == NULL)
	{
		free (info->tokens[pos_dollar]);
		info->tokens[pos_dollar] = NULL;
		realloc_copy(info, pos_dollar, 1);
		free (info->tokens[pos_dollar]);
		info->tokens[pos_dollar] = NULL;
		realloc_copy(info, pos_dollar, 1);
		ret[0] = ret[0] - 1;
	}
	else
	{
		i = check_before_after(info, pos_dollar, pos_dollar);
		if (i == 2 || i == 1)
		{
			info->tokens[pos_dollar - 1] = ft_strjoinbas(info->tokens[pos_dollar - 1], name);
			free (info->tokens[pos_dollar]);
			info->tokens[pos_dollar] = NULL;
			realloc_copy(info, pos_dollar, 1);
			free (info->tokens[pos_dollar]);
			info->tokens[pos_dollar] = NULL;
			realloc_copy(info, pos_dollar, 1);
			ret[0] = ret[0] - 2;
		}
		else
		{
			i = ft_strlen(name);
			free (info->tokens[pos_dollar]);
			info->tokens[pos_dollar] = (char *)malloc(i + 1);
			ft_strlcpy(info->tokens[pos_dollar], name, i + 1);
			free (info->tokens[pos_dollar + 1]);
			info->tokens[pos_dollar + 1] = NULL;
			realloc_copy(info, pos_dollar + 1, 1);
			ret[0]--;
		}
	}
}

//3.230 Name
//In the shell command language, a word consisting solely of underscores, digits, 
//and alphabetics from the portable character set. The first character of a name is not a digit.
// https://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_230
// https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html#tag_02_06_02

void	check_dollar(t_info *info, int first_q, int *ret, int out)
{
	int	i;

	if (ret[0] == -1)
	{
		if (check_char_token(info, first_q) == C_DOLLAR)
			expand_dollar(info, first_q, ret);
	}
	else if (check_char_token(info, first_q) == C_DQUOTE)
	{
		i = 0;
		while (i < ret[0] - first_q - 1)
		{
			if (check_char_token(info, first_q + 1 + i) == C_DOLLAR)
				expand_dollar(info, first_q + 1 + i, ret);
			i++;
		}
	}
}

int	check_unclosed(t_info *info, int i, int j)
{
	if (info->tokens[i][0] == C_QUOTE)
	{
		while (info->tokens[j] != NULL)
		{
			if (info->tokens[j][0] == C_QUOTE)
				break ;
			j++;
		}
	}
	else
	{
		while (info->tokens[j] != NULL)
		{
			if (info->tokens[j][0] == C_DQUOTE)
				break ;
			j++;
		}
	}
	if (info->tokens[j] == NULL)
		return (-1);
	return (j);
}

void	trim_quotes2(t_info *info, int pos, int first, int last)
{
	while (info->tokens[pos][first + 1] != '\0')
	{
		info->tokens[pos][first] = info->tokens[pos][first + 1];
		first++;
	}
	info->tokens[pos][first] = '\0';
	while (info->tokens[pos][last] != '\0')
	{
		info->tokens[pos][last - 1] = info->tokens[pos][last];
		last++;
	}
	info->tokens[pos][last - 1] = '\0';
}

void	trim_quotes(t_info *info, int pos, int len)
{
	int	first;
	int last;
	int	i;

	i = 0;
	while (info->tokens[pos][i] != '\0')
	{
		if (info->tokens[pos][i] == C_QUOTE || info->tokens[pos][i] == C_DQUOTE)
			break ;
		i++;
	}
	first = i;
	i = len - 1;
	while (i >= 0)
	{
		if (info->tokens[pos][i] == C_QUOTE || info->tokens[pos][i] == C_DQUOTE)
			break ;
		i--;
	}
	last = i;
	trim_quotes2(info, pos, first, last);	
}

void	merge_cut_quotes(t_info *info, int first_q, int last_q, int n)
{
	int	i;

	if (n == 1 || n == 2)
		first_q--;
	if (n == 2 || n == 3)
		last_q++;
	i = first_q;
	while (i < last_q)
	{
		info->tokens[first_q] = ft_strjoinbas(info->tokens[first_q], info->tokens[first_q + 1]);
		free(info->tokens[first_q + 1]);
		info->tokens[first_q + 1] = NULL;
		realloc_copy(info, first_q + 1, 1);
		i++;
	}
	trim_quotes(info, first_q, ft_strlen(info->tokens[first_q]));
}

void	ft_checkthosequotes(t_info *info, int pos)
{
	int	i;
	int	n;
	int	store;
	int	ret;

	printf("??\n");
	i = pos;
	while (info->tokens[i] != NULL)
	{
		ret = -1;
		if (info->tokens[i][0] == C_QUOTE || info->tokens[i][0] == C_DQUOTE)
		{
			ret = check_unclosed(info, i, i + 1); // i is pos quote 1, ret is pos quote 2
			if (ret == -1)
				ft_error(2);
			if (check_empty_quotes(info, i, ret) == 1)
				continue ;
			check_dollar(info, i, &ret, 0);
			n = check_before_after(info, i, ret); //n == 0: no normal chars before or after quotes  n == 1, only before, n = 2, after & before n = 3 only after
			merge_cut_quotes(info, i, ret, n);
			store = i;
			if (n == 1 || n == 2)
				continue ;
		}
		else
			check_dollar(info, i, &ret, 1);
		i++;
	}
	return ;
}

int	check_before_after(t_info *info, int first_q, int last_q)
{
	int	ret;
	int	i;

	i = ft_strlen(info->line_read) + 1;
	if (last_q != i && info->tokens[last_q + 1] != NULL)
	{
		while (check_char_token(info, last_q + 1) == C_QUOTE || check_char_token(info, last_q + 1) == C_DQUOTE)
		{
			ft_checkthosequotes(info, last_q + 1);
			if (first_q != 0)
			{
				if (info->tokens[first_q - 1] != NULL && check_char_token(info, first_q - 1) == C_NORMAL)
					return (2);
			}
			return (3);
		}
	}
	ret = 0;
	if (first_q != 0)
	{
		if (info->tokens[first_q - 1] != NULL && check_char_token(info, first_q - 1) == C_NORMAL)
		{
			ret++;
			if (last_q != i)
			{
				if (info->tokens[last_q + 1] != NULL && check_char_token(info, last_q + 1) == C_NORMAL)
					ret++;
				return (ret);
			}
		}
	}
	if (last_q != i)
	{
		if (info->tokens[last_q + 1] != NULL && check_char_token(info, last_q + 1) == C_NORMAL)
			ret = 3;
	}
	return (ret);
}

void	realloc_copy(t_info *info, int start, int incr)
{
	while (info->tokens[start + incr] != NULL)
	{
		info->tokens[start] = (char *)malloc(1 + ft_strlen(info->tokens[start + incr]));
		if (info->tokens[start] == NULL)
			ft_error(1);
		ft_strlcpy(info->tokens[start], info->tokens[start + incr], 1 + ft_strlen(info->tokens[start + incr]));
		free(info->tokens[start + incr]);
		info->tokens[start + incr] = NULL;
		start = start + incr;
	}
}

void	joinwithnormalbefore(t_info *info, int first_q, int last_q)
{
	if (first_q != 0)
	{
		if (info->tokens[first_q] != NULL)
		{
			if (check_char_token(info, first_q) == C_NORMAL && check_char_token(info, first_q - 1) == C_NORMAL)
			{
				info->tokens[first_q - 1] = ft_strjoinbas(info->tokens[first_q - 1], info->tokens[first_q]);
				free (info->tokens[first_q]);
				info->tokens[first_q] = NULL;
				realloc_copy(info, first_q, 1);
			}
		}
	}
}

int	check_empty_quotes(t_info *info, int first_q, int last_q)
{
	if (last_q - first_q == 1)
	{
		free(info->tokens[first_q]);
		info->tokens[first_q] = NULL;
		free(info->tokens[last_q]);
		info->tokens[last_q] = NULL;
		realloc_copy(info, first_q, 2);
		realloc_copy(info, last_q, 2);
		joinwithnormalbefore(info, first_q, last_q);
		return (1);
	}
	return (0);
}

int	parse_quotes(t_info *info, int ret)
{
	int	i;
	int	n;
	int	store;

	i = 0;
	store = 0;
	while (info->tokens[i] != NULL)
	{
		ret = -1;
		if (info->tokens[i][0] == C_QUOTE || info->tokens[i][0] == C_DQUOTE)
		{
			ret = check_unclosed(info, i, i + 1); // i is pos quote 1, ret is pos quote 2
			if (ret == -1)
				return (-1);
			if (check_empty_quotes(info, i, ret) == 1)
				continue ;
			check_dollar(info, i, &ret, 0);
			n = check_before_after(info, i, ret); //n == 0: no normal chars before or after quotes  n == 1, only before, n = 2, after & before n = 3 only after
			merge_cut_quotes(info, i, ret, n);
			store = i;
			if (n == 1 || n == 2)
				continue ;
		}
		else
			check_dollar(info, i, &ret, 1);
		i++;
	}
	return (0);
}

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

int	parser(t_info *info)
{
	int	ret;

	ret = parse_quotes(info, 0);
	if (ret == -1)
	{
		printf("checkret-1\n");
		return (-1);
	}
	remove_spaces(info);
	printf("after merge\n");
	int p = 0;
	while (info->tokens[p] != NULL)
	{
		printf("stored = %s\n", info->tokens[p]);
		p++;
	}
	return (0);
}

// fix empty quotes after a chain of recurse quotes

/*
void	reorder_lines(t_info *info, int first_q, int last_q, int n_cut)
{
	int	i;
	int	j;

	j = ft_strlen(info->line_read);
	i = 1;
	if (last_q != j)
	{
		while (n_cut > 0 || info->tokens[last_q + i] != NULL)
		{
			free(info->tokens[first_q + i]);
			info->tokens[first_q + i] = NULL;
			info->tokens[first_q + i] = (char *)malloc(sizeof(info->tokens[last_q + i]));
			ft_strlcpy(info->tokens[first_q + i], info->tokens[last_q + i], ft_strlen(info->tokens[last_q + i]));
			free(info->tokens[last_q + i]);
			info->tokens[last_q + i] = NULL;
			n_cut--;
			i++;
		}
	}
}

if (n == 0)
	{
		i = 0;
		while (i < len - 2)
		{
			info->tokens[pos][i] = info->tokens[pos][i + 1];
			i++;
		}
		info->tokens[pos][i] = '\0';
		return ;
	}
 */
/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:31 by bhoitzin      #+#    #+#                 */
/*   Updated: 2021/12/13 20:08:06 by bhoitzin      ########   odam.nl         */
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
		return (NULL);
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

void	expand_dollar(t_info *info, int pos_dollar)
{
	char *name;

	name = getenv(info->tokens[pos_dollar + 1]);
	if (name == NULL)
	{
		//free(info->tokens[pos_dollar]);
		//info->tokens[pos_dollar] = malloc(1);
		//free(info->tokens[pos_dollar + 1]);
		//info->tokens[pos_dollar + 1] = malloc(1);
	}
	else
	{
		//free(info->tokens[pos_dollar]);
		//info->tokens[pos_dollar] = NULL;
		//free(info->tokens[pos_dollar + 1]);
		//info->tokens[pos_dollar + 1] = NULL;
	}

}

//3.230 Name
//In the shell command language, a word consisting solely of underscores, digits, 
//and alphabetics from the portable character set. The first character of a name is not a digit.
// https://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap03.html#tag_03_230
// https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html#tag_02_06_02

void	check_dollar(t_info *info, int first_q, int last_q)
{
	int	i;

	if (check_char_token(info, first_q) == C_DQUOTE)
	{
		i = 0;
		while (i < last_q - first_q - 1)
		{
			if (check_char_token(info, first_q + 1 + i) == C_DOLLAR)
				expand_dollar(info, first_q + 1 + i);
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

int	merge_cut_quotes(t_info *info, int *first_q, int *last_q, int n)
{
	int	i;

	if (n == 1 || n == 2)
		first_q[0]--;
	if (n == 2 || n == 3)
		last_q[0] = last_q[0] + 2;
	i = first_q[0];
	n = 1;
	while (i < last_q[0] - 1)
	{
		info->tokens[first_q[0]] = ft_strjoinbas(info->tokens[first_q[0]], info->tokens[first_q[0] + n]);
		free(info->tokens[first_q[0] + n]);
		info->tokens[first_q[0] + n] = NULL;
		i++;
		n++;
	}
	return (n - 1);
}

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

int	check_before_after(t_info *info, int first_q, int last_q)
{
	int	ret;
	int	i;

	i = ft_strlen(info->line_read) + 1;
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
			}
		}
		return (ret);
	}
	if (last_q != i)
	{
		if (info->tokens[last_q + 1] != NULL && check_char_token(info, last_q + 1) == C_NORMAL)
			ret = 3;
	}
	return (ret);
}

int	check_empty_quotes(t_info *info, int first_q, int last_q)
{
	if (last_q - first_q == 1)
	{
		free(info->tokens[first_q]);
		info->tokens[first_q] = NULL;
		free(info->tokens[last_q]);
		info->tokens[last_q] = NULL;
		while (info->tokens[first_q + 2] != NULL)
		{
			info->tokens[first_q] = (char *)malloc(sizeof(char) * 2);
			info->tokens[first_q][0] = info->tokens[first_q + 2][0];
			info->tokens[first_q][1] = '\0';
			free(info->tokens[first_q + 2]);
			info->tokens[first_q + 2] = NULL;
			first_q = first_q + 2;
		}
		while (info->tokens[last_q + 2] != NULL)
		{
			info->tokens[last_q] = (char *)malloc(sizeof(char) * 2);
			info->tokens[last_q][0] = info->tokens[last_q + 2][0];
			info->tokens[last_q][1] = '\0';
			free(info->tokens[last_q + 2]);
			info->tokens[last_q + 2] = NULL;
			last_q = last_q + 2;
		}
		return (1);
	}
	return (0);
}

int	parse_quotes(t_info *info)
{
	int	i;
	int	ret;
	int	n;

	i = 0;
	while (info->tokens[i] != NULL)
	{
		if (info->tokens[i][0] == C_QUOTE || info->tokens[i][0] == C_DQUOTE)
		{
			ret = check_unclosed(info, i, i + 1); // i is pos quote 1, ret is pos quote 2
			if (ret == -1)
				return (-1);
			if (check_empty_quotes(info, i, ret) == 1)
				continue ;
			check_dollar(info, i, ret);
			n = check_before_after(info, i, ret); //n == 0: no normal chars before or after quotes  n == 1, only before, n = 2, after & before n = 3 only after
			n = merge_cut_quotes(info, &i, &ret, n); // n here returns as the amount of lines joined together
			if (info->tokens[ret + 1] != NULL)
				reorder_lines(info, i, ret, n);
			i = ret - n;
		}
		else
			i++;
	}
	return (0);
}

int	parser(t_info *info)
{
	int	ret;

	ret = parse_quotes(info);
	if (ret == -1)
	{
		printf("checkret-1\n");
		return (-1);
	}
//	expand_dollar(info);
//	remove_quotes(info);
//	fix_order(info);
	printf("after merge\n");
	int p = 0;
	while (info->tokens[p] != NULL)
	{
		printf("stored = %s\n", info->tokens[p]);
		p++;
	}
	return (0);
}



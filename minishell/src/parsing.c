/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:31 by bhoitzin      #+#    #+#                 */
/*   Updated: 2021/12/17 16:37:35 by bhoitzin      ########   odam.nl         */
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
	
}

void	expand_exitstatus(t_info *info, int i)
{

}

char	*get_name(t_info *info, int i)
{
	char	*str;
	int		j;

	j = 0;
	while (check_char_token(info, i, j) == C_NORMAL && info->tokens[i][j] != '\0')
		j++;
	str = (char *)malloc(j + 1);
	ft_strlcpy(str, info->tokens[i], j + 1);
	return (str);
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


char	*get_rest(t_info *info, int i)
{
	char	*str;
	int		j;
	int		k;

	k = 0;
	j = 0;
	while (check_char_token(info, i, j) == C_NORMAL && info->tokens[i][j] != '\0')
		j++;
	if (info->tokens[i][j] == '\0')
		return (NULL);
	k = j;
	while (info->tokens[i][k] != '\0')
		k++;
	str = (char *)malloc(k + 1);
	int ret = ft_strlcpy2(str, info->tokens[i], k - j + 1, j);
	return (str);
}

void	expand_token_dollar(t_info *info, int i)
{
	char	*name;
	char	*rest;

	if (info->tokens[i + 1] == NULL)
		return ;
	name = get_name(info, i + 1);
	rest = get_rest(info, i + 1);
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

void	expand_str_dollar(t_info *info, int i, int pos)
{
	
}

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

void	check_dollar_str(t_info *info)
{
	
}

void	check_dollar_token(t_info *info)
{
	int	i;
	int	j;

	i = 0;
	while (info->tokens[i] != NULL)
	{
		if (info->tokens[i][0] == '$')
		{
			if (check_after_dollar(info, i) == 0)
				expand_token_dollar(info, i); // if i+1 == NULL -> leave $ untouched
			if (check_after_dollar(info, i) == 1) // if i+1 == quotes (ret 2) -> remove $
				expand_exitstatus(info, i);
			i = i - join_tokens(info, i);
		}
		i++;
	}
	check_dollar_str(info);
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

int	parser(t_info *info) // to-do: state assignment. $ expansion in a quoted "" string
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
	//printf("after parse\n");
	while (info->tokens[p] != NULL)
	{
		//printf("stored = %s\n", info->tokens[p]);
		p++;
	}
	check_dollar_token(info);
	set_token_state(info);
	//printf("after dollar\n");
	p = 0;
	while (info->tokens[p] != NULL)
	{
		//printf("stored = %s\n", info->tokens[p]);
		p++;
	}
	remove_spaces(info);
	//printf("after spaces\n");
	p = 0;
	while (info->tokens[p] != NULL)
	{
		//printf("stored = %s\n", info->tokens[p]);
		p++;
	}
	return (0);
}

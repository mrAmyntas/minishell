/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:31 by bhoitzin      #+#    #+#                 */
/*   Updated: 2021/12/17 12:36:18 by bhoitzin      ########   odam.nl         */
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

void	check_dollar(t_info *info)
{
	int	i;
	int	j;

	i = 0;
	while (info->tokens[i] != NULL && info->tokens[i + 1] != NULL)
	{
		if (info->tokens[i][0] == '$')
		{
			if (info->tokens[i + 1][0] == '?')
			{
				expandexitstatus(info, i);
				return ;
			}
			if (info->tokens[i + 1][0] == C_DQUOTE)
			{
				j = 1;
				while (info->tokens[i + 1][j] != '\0')
				{
					if (info->tokens[i + 1][j] == C_DOLLAR)
						expand_dollar(info, i + 1, j);
					j++;
				}
			}
			if (info->tokens[i + 1][0] == C_NORMAL)
				expand_dollar(info, i, 0);
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
			merge_quotes(info, i, ret, n); //
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
	check_dollar(info);
	set_token_state(info);
	printf("after merge\n");
	int p = 0;
	while (info->tokens[p] != NULL)
	{
		printf("stored = %s\n", info->tokens[p]);
		p++;
	}
	remove_spaces(info);
	return (0);
}

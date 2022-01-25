/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:31 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/01/25 16:42:36 by bhoitzin      ########   odam.nl         */
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
		printf("in token_state: char: %c\n", info->tokens[i][0]);
		if (info->tokens[i][0] == '\'' || info->tokens[i][0] == '\"')
			info->token_state[i] = 0; // 0 = quoted
		else if (check_char_token(info, i, 0) != C_NORMAL)
			info->token_state[i] = 1; // 1 = special char
		else
			info->token_state[i] = 0; // normal chars
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
// to-do:
// crash bij export PID=$$
// Name = not just 'not special chars' but also not other none-alphanumerics
// echo "$a>" (werkt alleen niet bij milan)
// $?
// add function to milans heredoc loop -> to make the expansion$ work
// echo $  fix
// state voor echo $USERa > out
int	parser(t_info *info)
{
	int	ret;

	int p = 0;
	printf("---------------------------------------------------------------------------------------------\n");
	printf("after lex\n");
	while (info->tokens[p] != NULL)
	{
		printf("stored = %s\n", info->tokens[p]);
		p++;
	}
	//----------------------------------------------
	//            PARSING QUOTES
	//----------------------------------------------
	ret = parse_quotes(info, 0);
	if (ret == -1)
	{
		printf("checkret-1\n");
		return (-1);
	}
	find_dgreater_dlesser(info);
	p = 0;
	printf("---------------------------------------------------------------------------------------------\n");
	printf("after quotes\n");
	while (info->tokens[p] != NULL)
	{
		printf("stored = %s\n", info->tokens[p]);
		p++;
	}

	//----------------------------------------------
	//            EXPANSION
	//----------------------------------------------
	expansion(info);
	printf("---------------------------------------------------------------------------------------------\n");
	printf("after dollar\n");
	p = 0;
	while (info->tokens[p] != NULL)
	{
		printf("stored = %s\n", info->tokens[p]);
		p++;
	}
	//----------------------------------------------
	//            MERGE TOKENS THAT ARE QUOTED
	//----------------------------------------------
	join_quoted_tokens(info);
	printf("---------------------------------------------------------------------------------------------\n");
	printf("after join_quoted_tokens\n");
	p = 0;
	while (info->tokens[p] != NULL)
	{
		printf("stored = %s\n", info->tokens[p]);
		p++;
	}
	//----------------------------------------------
	//            REMOVE SPACES
	//----------------------------------------------
	remove_spaces(info);
	printf("---------------------------------------------------------------------------------------------\n");
	printf("after remove_spaces\n");
	p = 0;
	while (info->tokens[p] != NULL)
	{
		printf("stored = %s state = %d\n", info->tokens[p], info->token_state[p]);
		p++;
	}
	//----------------------------------------------
	//            SET TOKEN STATE
	//----------------------------------------------
	set_token_state(info);
	printf("---------------------------------------------------------------------------------------------\n");
	printf("after set_token_state\n");
	p = 0;
	while (info->tokens[p] != NULL)
	{
		printf("stored = %s state = %d\n", info->tokens[p], info->token_state[p]);
		p++;
	}
	//----------------------------------------------
	//            REMOVE QUOTES
	//----------------------------------------------
	remove_quotes(info);
	printf("---------------------------------------------------------------------------------------------\n");
	printf("after parser\n");
	p = 0;
	while (info->tokens[p] != NULL)
	{
		printf("stored = %s state = %d\n", info->tokens[p], info->token_state[p]);
		p++;
	}
	return (0);
}


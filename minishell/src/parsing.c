/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:31 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/01/28 16:30:00 by bhoitzin      ########   odam.nl         */
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
				free(info->tokens[i]);
				info->tokens[i] = NULL;
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
//		printf("in token_state: char: %c\n", info->tokens[i][0]);
		if (info->tokens[i][0] == '\'' || info->tokens[i][0] == '\"')
			info->token_state[i] = 3; // 0 = quoted
		else if (check_char_token(info, i, 0) != C_NORMAL && check_char_token(info, i, 0) != C_DOLLAR)
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
			if (ret == -2)
				ft_error(info, ret);
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
// crash bij export
// $?
// leaks: ???
// exit status: if it is X -> a call to minishell should NOT reset it, if there is no new command
// write naar stderr ipv printf met de errors
// echo | -> geen syntax error
// cntrl - D werkt niet direct na ctrl-C of cntr-backslash
// cat infinite loop
// set SHLVL = 2
// cd heeft leaks (cd ..)
// cntrl - L after a up or down?
// scrambling of terminal with up/down has to do with whatever is that readline color shit
void	unclosed_pipe(t_info *info)
{
	int	i;

	i = 0;
	while (info->tokens[i] != NULL)
		i++;
	if (i != 0)
	{
		if (info->tokens[i - 1][0] == '|')
			ft_error(info, -3);
	}
}

void	parser(t_info *info)
{
	int	ret;

	int p = 0;
//	printf("---------------------------------------------------------------------------------------------\n");
//	printf("after lex\n");
	while (info->tokens[p] != NULL)
	{
//		printf("stored = %s\n", info->tokens[p]);
		p++;
	}
	//----------------------------------------------
	//            PARSING QUOTES
	//----------------------------------------------
	parse_quotes(info, 0);
	unclosed_pipe(info);
	find_dgreater_dlesser(info);
	p = 0;
//	printf("---------------------------------------------------------------------------------------------\n");
//	printf("after quotes\n");
	while (info->tokens[p] != NULL)
	{
//		printf("stored = %s\n", info->tokens[p]);
		p++;
	}

	//----------------------------------------------
	//            EXPANSION
	//----------------------------------------------
	expansion(info);
//	printf("---------------------------------------------------------------------------------------------\n");
//	printf("after dollar\n");
	p = 0;
	while (info->tokens[p] != NULL)
	{
//		printf("stored = %s\n", info->tokens[p]);
		p++;
	}
	//----------------------------------------------
	//            MERGE TOKENS THAT ARE QUOTED
	//----------------------------------------------
	join_quoted_tokens(info);
//	printf("---------------------------------------------------------------------------------------------\n");
//	printf("after join_quoted_tokens\n");
	p = 0;
	while (info->tokens[p] != NULL)
	{
//		printf("stored = %s\n", info->tokens[p]);
		p++;
	}
	//----------------------------------------------
	//            REMOVE SPACES
	//----------------------------------------------
	remove_spaces(info);
//	printf("---------------------------------------------------------------------------------------------\n");
//	printf("after remove_spaces\n");
	p = 0;
	while (info->tokens[p] != NULL)
	{
//		printf("stored = %s state = %d\n", info->tokens[p], info->token_state[p]);
		p++;
	}
	//----------------------------------------------
	//            SET TOKEN STATE
	//----------------------------------------------
	set_token_state(info);
//	printf("---------------------------------------------------------------------------------------------\n");
//	printf("after set_token_state\n");
	p = 0;
	while (info->tokens[p] != NULL)
	{
//		printf("stored = %s state = %d\n", info->tokens[p], info->token_state[p]);
		p++;
	}
	//----------------------------------------------
	//            REMOVE QUOTES
	//----------------------------------------------
	remove_quotes(info);
//	printf("---------------------------------------------------------------------------------------------\n");
//	printf("after parser\n");
	p = 0;
	while (info->tokens[p] != NULL)
	{
//		printf("stored = %s state = %d\n", info->tokens[p], info->token_state[p]);
		p++;
	}
//	printf("---------------------------------------------------------------------------------------------\n");
}

/*
bash-3.2$ www
bash: www: command not found
bash-3.2$ echo $?
127
bash-3.2$ www
bash: www: command not found
bash-3.2$ echo $? > t1 | echo $? > t2 | www | echo $? > t3
bash: www: command not found
bash-3.2$ cat t1
127
bash-3.2$ cat t2
0
bash-3.2$ cat t3
0 */

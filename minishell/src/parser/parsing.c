/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:31 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/03 13:38:51 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// to-do:

// cd ../minishell werkt neit (pwd update niet goed)
// cd . gaat naar .
// cd ~/minishellrepo no such file
// cd - moet naar OLDPWD

// cd inc -> leaks

// cat/grep infinite loop

// set SHLVL = 2

// protect all mallocs

// should reset exit status to 0 after all succesfull commands
// so also when piping so not re-looping in minishell

// Search and launch the right executable (based on the PATH variable or using a
// relative or an absolute path). e.g. /bin/pwd shoul work 
#include "../../inc/minishell.h"

static void	remove_quotes(t_info *info)
{
	int	i;
	int	j;

	i = 0;
	while (info->tokens[i] != NULL)
	{
		j = 0;
		while (info->tokens[i][j] != '\0')
		{
			if (info->tokens[i][j] == '\'' || info->tokens[i][j] == '\"')
			{
				j = cut_quotes(info, i, info->tokens[i][j], j);
				continue ;
			}
			j++;
		}
		if (ft_strlen(info->tokens[i]) == 0)
		{
			free(info->tokens[i]);
			info->tokens[i] = NULL;
			realloc_copy(info, i, 1);
		}
		i++;
	}
}

static void	remove_spaces(t_info *info)
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

static void	unclosed_pipe(t_info *info)
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

// 3 = quoted | 1 = special char | 0 = normal chars
static void	set_token_state(t_info *info)
{
	int	i;

	i = 0;
	while (info->tokens[i] != NULL)
	{
		if (info->tokens[i][0] == '\'' || info->tokens[i][0] == '\"')
			info->token_state[i] = 3;
		else if (check_char_token(info, i, 0) != C_NORMAL
			&& check_char_token(info, i, 0) != C_DOLLAR)
			info->token_state[i] = 1;
		else
			info->token_state[i] = 0;
		i++;
	}
}

void	parser(t_info *info)
{
	int	ret;

	parse_quotes(info, 0);
	unclosed_pipe(info);
	find_dgreater_dlesser(info);
	expand(info);
	join_quoted_tokens(info, 0);
	remove_spaces(info);
	set_token_state(info);
	remove_quotes(info);
	find_syntax_error(info);
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

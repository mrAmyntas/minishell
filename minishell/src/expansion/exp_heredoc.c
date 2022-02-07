/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exp_heredoc.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/02 14:51:32 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/04 20:17:34 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	add_dquotes(t_info *info, char *buf, int len, int i)
{
	int	j;

	free(info->tokens[i]);
	info->tokens[i] = NULL;
	info->tokens[i] = (char *)malloc(sizeof(char) * len + 3);
	if (info->tokens[i] == NULL)
		ft_error(info, -1);
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

static void	reset_token(t_info *info, int i)
{
	int	j;

	free(info->tokens[i]);
	info->tokens[i] = NULL;
	info->tokens[i] = (char *)malloc(sizeof(char) * 3);
	if (info->tokens[i] == NULL)
		ft_error(info, -1);
	info->tokens[i][0] = '<';
	info->tokens[i][1] = '<';
	info->tokens[i][2] = '\0';
	j = 0;
}

char	*expand_buf(t_info *info, char *buf, int i)
{
	int	j;
	int	x;

	if (info->token_state[i + 1] == 0)
	{
		add_dquotes(info, buf, ft_strlen(buf), i);
		x = info->exit_status;
		info->exit_status = info->exit_status2;
		check_dollar_in_quotes(info, i);
		free(buf);
		buf = NULL;
		buf = (char *)malloc(sizeof(char) * (ft_strlen(info->tokens[i]) - 1));
		if (buf == NULL)
			ft_error(info, -1);
		j = 0;
		while (info->tokens[i][j + 2] != '\0')
		{
			buf[j] = info->tokens[i][j + 1];
			j++;
		}
		buf[j] = '\0';
		reset_token(info, i);
	}
	info->exit_status = x;
	return (buf);
}

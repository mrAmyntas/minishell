/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exp_heredoc.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/02 14:51:32 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/09 20:55:24 by bhoitzin      ########   odam.nl         */
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

static void	expand_ex_status_str(t_info *info, int i, int j, int n)
{
	char	*temp;
	char	*str;
	int		k;

	temp = realloc_token(info, i, 10);
	str = ft_itoa(g_sig.exit_status);
	k = 0;
	while (str[k] != '\0')
	{
		info->tokens[i][j] = str[k];
		k++;
		j++;
	}
	while (temp[n] != '\0')
	{
		info->tokens[i][j] = temp[n];
		n++;
		j++;
	}
	info->tokens[i][j] = '\0';
	free(str);
	str = NULL;
	free(temp);
	temp = NULL;
}

static void	check_dollar_in_quote(t_info *info, int i)
{
	int	j;

	j = 0;
	while (info->tokens[i][j] != '\0')
	{
		if (info->tokens[i][j] == '\"')
		{
			j++;
			while (info->tokens[i][j] != '\"')
			{
				if (info->tokens[i][j] == '$' && info->tokens[i][j + 1] == '?')
					expand_ex_status_str(info, i, j, j + 2);
				if (info->tokens[i][j] == '$')
					expand_str_dollar(info, i, j);
				j++;
			}
		}
		j++;
	}
}
char	*expand_buf(t_info *info, char *buf, int i)
{
	int	j;
	int	x;

	if (info->token_state[i + 1] == 0)
	{
		add_dquotes(info, buf, ft_strlen(buf), i);
		x = g_sig.exit_status;
		g_sig.exit_status = g_sig.exit_status2;
		check_dollar_in_quote(info, i);
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
	g_sig.exit_status = x;
	return (buf);
}

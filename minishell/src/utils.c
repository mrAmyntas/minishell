/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils2.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/24 17:04:11 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/24 17:09:30 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	long_str(char *str1, char *str2)
{
	int	len[2];

	len[0] = ft_strlen(str1);
	len[1] = ft_strlen(str2);
	if (len[0] > len[1])
		return (len[0]);
	return (len[1]);
}

int	ft_strstrlen(char **str, char *c, int i)
{
	while (str[i] && ft_strncmp(str[i], c, long_str(str[i], c)))
		i++;
	return (i);
}

int	ft_strstrlen2(t_info *info, char *c, int i)
{
	while (info->tokens[i] && (ft_strncmp(info->tokens[i], c,
				long_str(info->tokens[i], c)) || info->token_state[i] == 3))
		i++;
	return (i);
}

static void	ft_set_global(t_info *info)
{
	if (info->first_process == 1)
		g_sig.exit_status2 = 0;
	g_sig.sig = 0;
	g_sig.id = 2;
}

void	ft_heredoc(t_info *info, int i)
{
	char	*buf;
	int		pipefd[2];

	ft_set_global(info);
	pipe(pipefd);
	dup2(info->fd_std[0], 0);
	buf = readline("> ");
	while (buf
		&& ft_strncmp(buf, info->tokens[i + 1]
			, long_str(buf, info->tokens[i + 1]))
		&& (!g_sig.sig || g_sig.sig == 1))
	{
		buf = expand_buf(info, buf, i);
		write(pipefd[1], buf, ft_strlen(buf));
		write(pipefd[1], "\n", 1);
		free(buf);
		buf = readline("> ");
	}
	free(buf);
	buf = NULL;
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	g_sig.id = 1;
}

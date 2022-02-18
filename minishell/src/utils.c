/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/18 13:24:18 by bhoitzin      ########   odam.nl         */
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

int	ft_len_to_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] != c && str[i])
		i++;
	if (!str[i])
		return (-1);
	return (i);
}

int	ft_strstrlen(char **str, char *c, int i)
{
	while (str[i] && ft_strncmp(str[i], c, long_str(str[i], c)))
		i++;
	return (i);
}

void	get_env(t_info *info, char **env)
{
	int	i;
	int	env_len;

	i = 0;
	env_len = ft_strstrlen(env, NULL, 0);
	info->env = malloc(sizeof(char **) * (env_len + 1));
	if (info->env == NULL)
		ft_error(info, -1);
	while (env[i])
	{
		info->env[i] = ft_strdup(env[i]);
		i++;
	}
	info->env[i] = NULL;
}

int	ft_heredoc(t_info *info, int i, char *buf, int fd)
{
	if (info->first_process == -1)
		g_sig.exit_status2 = 0;
	fd = open("/tmp/minishell_heredoc", O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (fd < 0)
		return (0);
	dup2(info->fd_std[0], 0);
	buf = readline("> ");
	while (buf
		&& ft_strncmp(buf, info->tokens[i + 1]
			, long_str(buf, info->tokens[i + 1]))
		&& (!g_sig.sig || g_sig.sig == 1))
	{
		buf = expand_buf(info, buf, i);
		write(fd, buf, ft_strlen(buf));
		write(fd, "\n", 1);
		free(buf);
		buf = readline("> ");
	}
	free(buf);
	buf = NULL;
	close(fd);
	fd = open("/tmp/minishell_heredoc", O_RDONLY);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (1);
}

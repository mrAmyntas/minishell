/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/24 17:07:41 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
//void	ft_heredoc(t_info *info, int i)
//{
//	char	*buf;
//	int		fd;
//
//	g_sig.id = 0;
//	fd = open("/tmp/minishell_heredoc", O_RDWR | O_TRUNC | O_CREAT, 0644);
//	if (fd < 0)
//		return ;
//	dup2(info->fd_std[0], 0);
//	buf = readline("> ");
//	while (buf
//		&& ft_strncmp(buf, info->tokens[i + 1]
//			, long_str(buf, info->tokens[i + 1]))
//		&& (!g_sig.sig || g_sig.sig == 1))
//	{
//		buf = expand_buf(info, buf, i);
//		write(fd, buf, ft_strlen(buf));
//		write(fd, "\n", 1);
//		free(buf);
//		buf = readline("> ");
//	}
//	free(buf);
//	close(fd);
//	fd = open("/tmp/minishell_heredoc", O_RDONLY);
//	dup2(fd, STDIN_FILENO);
//	close(fd);
//	g_sig.id = 1;
//}

/*void	ft_heredoc(t_info *info, int i)
{
	char	*buf;
	int		pipefd[2];

	if (g_sig.sig == 3 || g_sig.sig == 2)
		return ;
	g_sig.id = 2;
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
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	g_sig.id = 1;
}*/

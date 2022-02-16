/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirect.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/16 17:21:32 by mgroen        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	**trim_command(t_info *info, int start, int end)
{
	char	**command;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!ft_strncmp(info->tokens[start], "<", 2)
		&& info->token_state[start])
		start += 2;
	if (!ft_strncmp(info->tokens[start], ">", 2)
		&& info->token_state[start])
		start += 2;
	while ((start + i) < end && info->token_state[start + i] != 1)
		i++;
	command = malloc(sizeof(char **) * (i + 1));
	if (command == NULL)
		ft_error(info, -1);
	while (j < i)
	{
		command[j] = ft_strdup(info->tokens[start]);
		j++;
		start++;
	}
	command[j] = NULL;
	return (command);
}

int	redirect(t_info *info, int type, int i)
{
	int	fd;

	if (type == 1)
		fd = open(info->tokens[i + 1], O_RDONLY);
	else if (type == 2)
		fd = open(info->tokens[i + 1], O_RDWR | O_TRUNC | O_CREAT, 0644);
	else
		fd = open(info->tokens[i + 1], O_RDWR | O_APPEND | O_CREAT, 0644);
	if (fd < 0)
	{
		if ((info->tokens[i + 1] == NULL || !(type % 2))
			&& access(info->tokens[i + 1], F_OK) == -1)
			set_error(info, 258, info->tokens[i + 1], 4);
		else
			set_error(info, 1, info->tokens[i + 1], -6);
		return (fd);
	}
	if (type == 2 || type == 4)
		dup2(fd, STDERR_FILENO);
	else if (type == 1)
		dup2(fd, STDIN_FILENO);
	close(fd);
	return (1);
}

void	ft_pipe(t_info *info, int start, int val[3], int oldfd[2])
{
	pid_t	id;
	int		pipefd[2];
	char	**command;
	//int		status;

	pipe(pipefd);
	id = fork();
	if (id == -1)
		perror("in ft_pipe: ");
	if (!id)
	{
		close(pipefd[0]);
		if (!val[1])
			dup2(pipefd[1], 2);
		command = trim_command(info, start, val[2]);
		command[0] = check_path(info, command[0]);
		if (command[0] == NULL)
			exit(1);
		ft_find_command(info, command, 0);
		exit(g_sig.exit_status);
	}
	parent_process(info, pipefd, val[2]);
	close(oldfd[0]);
}

int	find_redirect(t_info *info, int i, int fd[3], int end)
{
	int	pipeloc;

	pipeloc = -1;
	while (info->tokens[i] && i <= end)
	{
		if (!ft_strncmp(info->tokens[i], "<", 2) && info->token_state[i] == 1)
			fd[0] = redirect(info, 1, i);
		if (!ft_strncmp(info->tokens[i], "|", 2) && info->token_state[i] == 1)
			pipeloc = i;
		if (!ft_strncmp(info->tokens[i], ">", 2) && info->token_state[i] == 1)
			fd[1] = redirect(info, 2, i);
		if (!ft_strncmp(info->tokens[i], ">>", 3) && info->token_state[i] == 1)
			fd[1] = redirect(info, 4, i);
		if (!ft_strncmp(info->tokens[i], "<<", 3) && info->token_state[i] == 1)
			ft_heredoc(info, i);
		i++;
	}
	return (pipeloc);
}

void	check_redirect_v2(t_info *info, int start, int end, int oldfd[2])
{
	int		fd[3];
	char	**command;

	fd[0] = 0;
	while (fd[0]++ < 1000000);
	fd[0] = 0;
	fd[1] = 0;
	dup2(info->fd_std[2], 2);
	fd[2] = find_redirect(info, start, fd, end);
	if (fd[2] >= 0)
		return (ft_pipe(info, start, fd, oldfd));
	if (fd[0] < 0 || fd[1] < 0)
		return ;
	command = trim_command(info, start, end);
	command[0] = check_path(info, command[0]);
	return (ft_find_command(info, command, oldfd[0]));
}

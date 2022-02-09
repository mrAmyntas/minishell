/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirect.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/09 14:53:24 by bhoitzin      ########   odam.nl         */
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
	if (!ft_strncmp(info->tokens[start], "<", ft_strlen(info->tokens[start]))
		&& info->token_state[start])
		start += 2;
	while ((start + i) < end && (!info->token_state[start + i]
			|| info->token_state[start + i] == 3))
		i++;
	command = malloc(sizeof(char **) * (i + 1));
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
	else if (type == 4)
		fd = open(info->tokens[i + 1], O_RDWR | O_APPEND | O_CREAT, 0644);
	if (fd < 0)
	{
		if ((info->tokens[i + 1] == NULL || !(type % 2)) && access(info->tokens[i + 1], F_OK) == -1)
			set_error(info, 258, info->tokens[i + 1], 0);
		else
			set_error(info, 1, info->tokens[i + 1], 4);
		return (fd);
	}
	if (type == 2 || type == 4)
		dup2(fd, STDOUT_FILENO);
	else if (type == 1)
		dup2(fd, STDIN_FILENO);
	close(fd);
	return (1);
}

char	*check_path(t_info *info, char *command)
{
	int		loc;
	int		i;
	char	*new;
	DIR		*ret;

	if (!command)
		return (NULL);
	i = ft_strlen(command) - 2;
	while (command[i] != '/' && i > 0)
		i--;
	if (i <= 0)
		return (command);
	ret = opendir(command);
	if (ret != NULL)
	{
		set_error(info, 126, command, -5); // is a directory
		closedir(ret);
		return (NULL);
	}
	if (access(command, X_OK))
	{
		if (access(command, F_OK) == 0)
			set_error(info, 126, command, -5); // exists -> no permission
		else  // doesnt exist
			set_error(info, 127, command, -5);
		return (NULL);
	}
	loc = i + 1;
	new = malloc(sizeof(char *) * (ft_strlen(command) - loc + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (command[loc + i])
	{
		new[i] = command[loc + i];
		i++;
	}
	new[i] = '\0';
	free(command);
	return (new);
}

void	update_exit_status(t_info *info)
{
	int	i;

	i = 0;
	while (info->tokens[i] != NULL)
	{
		if (info->token_state[i] == 2)
		{
			free(info->tokens[i]);
			info->tokens[i] = NULL;
			info->tokens[i] = ft_itoa(info->exit_status);
		}
		i++;
	}
}

void	ft_pipe(t_info *info, int loc_pipe, int start, int fdout)
{
	int		id;
	int		pipefd[2];
	char	**command;

	update_exit_status(info);
	pipe(pipefd);
	id = fork();
	if (id == -1)
		set_error(info, 13, NULL, 4);
	if (id)
	{
		wait(&id);
		close(pipefd[1]);
		dup2(pipefd[0], 0);
		return (check_redirect_v2(info, loc_pipe + 1
				, ft_strstrlen(info->tokens, "|", loc_pipe + 1), pipefd[0]));
	}
	else
	{
		close(pipefd[0]);
		if (!fdout)
			dup2(pipefd[1], 1);
		command = trim_command(info, start, loc_pipe);
		command[0] = check_path(info, command[0]);
		if (command[0] == NULL)
			exit(1);
		ft_find_command(info, command);
		exit(0);
	}
}

int	find_redirect(t_info *info, int i, int fd[2], int end)
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
		if ((fd[0] < 0 || fd[1] < 0) && info->exit_status == 1)
			return (-1);
		i++;
	}
	return (pipeloc);
}

void	check_redirect_v2(t_info *info, int start, int end, int inputfd)
{
	int		fd[2];
	int		pipeloc;
	char	**command;

	fd[0] = 0;
	fd[1] = 0;
	pipeloc = find_redirect(info, start, fd, end);
	if (fd[0] < 0 || fd[1] < 0)
		return (ft_error(info, -4));
	if (pipeloc >= 0)
		return (ft_pipe(info, pipeloc, start, fd[1]));
	command = trim_command(info, start, end);
	command[0] = check_path(info, command[0]);
	return (ft_find_command(info, command));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/01/19 17:05:09 by mgroen        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


int	long_str(char *str1, char *str2)
{
	int len[2];
	
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

int	ft_strstrlen(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	unset_var(t_info *info, char *var)
{
	int	loc;

	loc = 0;
	while (info->env[loc] && strncmp(info->env[loc], var, ft_strlen(var)))
		loc++;
	if (!info->env[loc])
		return ;
	while (info->env[loc + 1])
	{
		free(info->env[loc]);
		info->env[loc] = ft_strdup(info->env[loc + 1]);
		loc++;
	}
	info->env[loc] = NULL;
}

void	sort_export(t_info *info)
{
	int	i;
	int	j;

	i = 1;
	info->export = malloc(sizeof(char **) * (ft_strstrlen(info->env) + 1));
	info->export[0] = ft_strdup(info->env[0]);
	while (info->env[i])
	{
		info->export[i] = NULL;
		j = ft_strstrlen(info->export) - 1;
		while (j >= 0)
		{
			if (ft_strncmp(info->env[i], info->export[j]
					, ft_len_to_char(info->env[i], '=')) > 0)
			{
				put_str(info->env[i], info->export, j + 1);
				break ;
			}
			else if (!j)
				put_str(info->env[i], info->export, 0);
			j--;
		}
		i++;
	}
	info->export[i] = NULL;
}

void	get_env(t_info *info, char **env)
{
	int	i;
	int	env_len;

	i = 0;
	env_len = ft_strstrlen(env) + 1;
	info->env = malloc(sizeof(char **) * env_len);
	while (env[i])
	{
		info->env[i] = ft_strdup(env[i]);
		i++;
	}
	info->env[i] = NULL;
}

int	ft_heredoc(t_info *info, int i)
{
	char	*buf;
	char	*long_buf;

	buf = readline("> ");
	long_buf = ft_strjoin(buf, "\n");
	while (buf && ft_strncmp(buf, info->tokens[i + 1], long_str(buf, info->tokens[i + 1])))
	{
		long_buf = ft_strjoin(long_buf, buf);
		long_buf = ft_strjoin(long_buf, "\n");
		free (buf);
		buf = readline("> ");
	}
	write(1, long_buf, ft_strlen(long_buf));
	free (long_buf);
	return (0);
}

int	redirect(t_info *info, int type, int i)
{
	int	fd;
	
	if (type == 1)
		fd = open(info->tokens[i + 1], O_RDONLY);
	if (type == 2)
		fd = open(info->tokens[i + 1], O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (type == 4)
		fd = open(info->tokens[i + 1], O_RDWR | O_APPEND | O_CREAT, 0644);
	//printf("fd: %i\n", fd);
	if (fd < 0)
		return (fd);
	if (type == 1 || type == 2)
		dup2(fd, type - 1);
	if (type == 3 || type == 4)
		dup2(fd, type - 3);
	return (fd);
}

char	**trim_command(t_info *info, int start, int end)
{
	char	**command;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!ft_strncmp(info->tokens[start], "<", ft_strlen(info->tokens[start])))
		start += 2;
	while ((start + i) < end && ft_strncmp(info->tokens[start + i], ">", ft_strlen(info->tokens[start + i]))
		&& ft_strncmp(info->tokens[start + i], ">>", long_str(info->tokens[start + i], ">>"))
		&& ft_strncmp(info->tokens[start + i], "<<", long_str(info->tokens[start + i], "<<")))
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

int ft_pipe(t_info *info, char **command, int loc_pipe)
{
	int	id;
	int	pipefd[2];
	
	pipe(pipefd);
	id = fork();
	if (id == -1)
		ft_error(4);
	if (id)
	{
		wait(&id);
		close(pipefd[1]);
		dup2(pipefd[0], 0);
		command = trim_command(info, loc_pipe + 1, ft_strstrlen(info->tokens));
		return (ft_find_command(info, command));
	}
	else
	{
		close(pipefd[0]);
		dup2(pipefd[1], 1);
		command = trim_command(info, 0, loc_pipe);
		ft_find_command(info, command);
		exit(0);
	}
}

int	check_redirect(t_info *info)
{
	int		i;
	int		fd[2];
	int		loc_pipe;
	int		heredoc;
	char	**command;

	i = 0;
	loc_pipe = -1;
	heredoc = -1;
	fd[0] = 0;
	fd[1] = 0;
	while (info->tokens[i])
	{
		if (!strncmp(info->tokens[i], "<", ft_strlen(info->tokens[i])))
			fd[0] = redirect(info, 1, i);
		if (!strncmp(info->tokens[i], "|", ft_strlen(info->tokens[i])))
			loc_pipe = i;
		if (!strncmp(info->tokens[i], ">", ft_strlen(info->tokens[i])))
			fd[1] = redirect(info, 2, i);
		if (!strncmp(info->tokens[i], ">>", long_str(info->tokens[i], ">>")))
			fd[1] = redirect(info, 4, i);
		if (!strncmp(info->tokens[i], "<<", long_str(info->tokens[i], "<<")))
			heredoc = i;
		if (fd[0] < 0 || fd[1] < 0)
			perror("");
		i++;
	}
	if (heredoc >= 0)
	{
		ft_heredoc(info, heredoc);
		//return (0);
	}
	if (loc_pipe >= 0)
		return (ft_pipe(info, command, loc_pipe));
	command = trim_command(info, 0, ft_strstrlen(info->tokens));
	return (ft_find_command(info, command));
}

int	ft_find_command(t_info *info, char **command)
{
	int i;

	i = 0;
	//while (command[i])
	//{
	//	printf("%s\n", command[i]);
	//	i++;
	//}
	if (!ft_strncmp(command[0], "echo", long_str(command[0], "echo"))
		|| !ft_strncmp(command[0], "cat", long_str(command[0], "cat"))
		|| !ft_strncmp(command[0], "grep", long_str(command[0], "grep")))
		return (exec(info, command));
	if (!ft_strncmp(command[0], "cd", long_str(command[0], "cd")))
		return (exec_cd(info, command));
	if (!ft_strncmp(command[0], "pwd", long_str(command[0], "pwd")))
		return (exec_pwd(info));
	if (!ft_strncmp(command[0], "export", long_str(command[0], "export")))
		return (exec_export(info, command));
	if (!ft_strncmp(command[0], "unset", long_str(command[0], "unset")))
		return (exec_unset(info, command));
	if (!ft_strncmp(command[0], "env", long_str(command[0], "env")))
		return (exec_env(info));
	if (!ft_strncmp(command[0], "exit", long_str(command[0], "exit")))
		exit(0);
	return (15);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2021/12/17 12:48:22 by mgroen        ########   odam.nl         */
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

int	redirect(t_info *info, int type, char *file)
{
	int	fd;
	
	if (type == 1)
		fd = open(file, O_RDONLY);
	if (type == 2)
		fd = open(file, O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (type == 4)
		fd = open(file, O_RDWR | O_APPEND | O_CREAT, 0644);
	if (fd < 0)
		return (1);
	if (type == 1 || type == 2)
		dup2(fd, type - 1);
	if (type == 3 || type == 4)
		dup2(fd, type - 3);
	//printf("%i\n", fd);
	return (0);
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
	while (ft_strncmp(info->tokens[start + i], ">", ft_strlen(info->tokens[start + i])) && (start + i) < end)
		i++;
	command = malloc(sizeof(char **) * (i + 1));
	while (j < i)
	{
		command[j] = info->tokens[start];
		j++;
		start++;
	}
	command[j] = NULL;
	return (command);
}

int	check_redirect(t_info *info)
{
	int		i;
	int		fd;
	int		loc_pipe;
	int		pipefd[2];
	char	**command;
	int		id;

	i = 0;
	loc_pipe = 0;
	while (info->tokens[i])
	{
		fd = 0;
		if (info->tokens[i][0] == '<')
			fd = redirect(info, 1, info->tokens[i + 1]);
		if (info->tokens[i][0] == '|')
			loc_pipe = i;
		if (info->tokens[i][0] == '>')
			fd = redirect(info, 2, info->tokens[i + 1]);
		if (fd)
			perror("");
		i++;
	}
	if (loc_pipe)
	{
		pipe(pipefd);
		id = fork();
		if (id == -1)
			ft_error(4);
		if (id)
		{
			wait(&id);
			close(pipefd[1]);
			dup2(pipefd[0], 0);
			//write(1, "command na pipe", 15);
			command = trim_command(info, loc_pipe + 1, ft_strstrlen(info->tokens));
			return (ft_find_command(info, command));
			// go to command
		}
		else
		{
			close(pipefd[0]);
			dup2(pipefd[1], 1);
			//write(1, "command voor pipe", 17);
			command = trim_command(info, 0, loc_pipe);
			ft_find_command(info, command);
			exit(0);
			// go to command
		}
		
	}
	command = trim_command(info, 0, ft_strstrlen(info->tokens));
	return (ft_find_command(info, command));
}

int	ft_find_command(t_info *info, char **command)
{
	if (ft_strncmp(info->tokens[0], "echo", 4) == 0)
		return (exec(info, command));
	if (ft_strncmp(info->tokens[0], "cd", 2) == 0)
		return (exec_cd(info, command));
	if (ft_strncmp(info->tokens[0], "pwd", 3) == 0)
		return (exec_pwd(info));
	if (ft_strncmp(info->tokens[0], "export", 6) == 0)
		return (exec_export(info, command));
	if (ft_strncmp(info->tokens[0], "unset", 5) == 0)
		return (exec_unset(info, command));
	if (ft_strncmp(info->tokens[0], "env", 3) == 0)
		return (exec_env(info));
	if (ft_strncmp(info->tokens[0], "exit", 4) == 0)
		exit(0);
	return (15);
}

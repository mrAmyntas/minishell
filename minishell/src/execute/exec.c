/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/02 17:36:10 by mgroen        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*get_path(char *cmd, char **env)
{
	char	*path;
	char	**dirs;
	char	*cmdfile;
	int		i;

	i = 0;
	path = getenv("PATH");
	dirs = ft_split(path, ':');
	while (dirs[i])
	{
		cmdfile = ft_strjoin(dirs[i], "/");
		cmdfile = ft_strjoin(cmdfile, cmd);
		free(dirs[i]);
		if (access(cmdfile, F_OK) == 0)
			break ;
		free(cmdfile);
		i++;
	}
	free(dirs);
	if (dirs[i])
		return (cmdfile);
	return (cmd);
}

int	exec_env(t_info *info, char **command)
{
	int	i;

	i = 0;
	while (info->env[i])
	{
		if (ft_len_to_char(info->env[i], '=') > -1)
		{
			write(1, info->env[i], ft_strlen(info->env[i]));
			write(1, "\n", 1);
		}
		i++;
	}
	return (0);
}

int	exec(t_info *info, char **command)
{
	char	*path;
	int		id;

	id = fork();
	if (id == -1)
		perror("fork error");
	if (id)
	{
		wait(&id);
		return (0);
	}
	path = get_path(command[0], info->env);
	execve(path, command, info->env);
	set_error(info, 127, NULL);
	ft_error(info, 0);
	exit (1);
}

int	exec_pwd(t_info *info, char **command)
{
	char	*pwd;

	pwd = ft_strjoin("PWD=", info->pwd);
	write(1, pwd, ft_strlen(pwd));
	write(1, "\n", 1);
	free (pwd);
	return (0);
}

void	ft_find_command(t_info *info, char **command)
{
	if (!ft_strncmp(command[0], "echo", 5)
		|| !ft_strncmp(command[0], "cat", 4)
		|| !ft_strncmp(command[0], "grep", 5))
		exec(info, command);
	else if (!ft_strncmp(command[0], "cd", 3))
		exec_cd(info, command);
	else if (!ft_strncmp(command[0], "pwd", 4))
		exec_pwd(info, command);
	else if (!ft_strncmp(command[0], "export", 7))
		exec_export(info, command);
	else if (!ft_strncmp(command[0], "unset", 6))
		exec_unset(info, command);
	else if (!ft_strncmp(command[0], "env", 4))
		exec_env(info, command);
	else if (!ft_strncmp(command[0], "exit", 5))
		exit(0);
	else if (command[0])
	{
		set_error(info, 127, command[0]);
		ft_error(info, 0);
	}
	free_strstr(command);
	return ;
}

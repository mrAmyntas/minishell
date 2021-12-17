/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2021/12/17 11:57:20 by mgroen        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

int	exec_export(t_info *info, char **command)
{
	int		i;
	int		j;

	i = 1;
	j = 0;
	while (command[i])
	{
		if (command[i][j] == '\\' || command[i][j] == '|')
			j++;
		if (command[i][j] < 65 || (command[i][j] > 90 && command[i][j] < 95) || command[i][j] > 122 || command[i][j] == 96)
			perror(ft_strjoin(command[i], ": not a valid identifier"));
		else
			add_env(info, command[i]);
		i++;
	}
	i = 0;
	while (info->export[i] && !command[1])
    {
		write(1, "declare -x ", 11);
        write(1, info->export[i], ft_strlen(info->export[i]));
		write(1, "\n", 1);
        i++;
    }
	return (0);
}

int	exec_unset(t_info *info, char **command)
{
	int		i;

	i = 1;
	while (command[i])
	{
		unset_var(info, command[i]);
		i++;
	}
	sort_export(info);
	return (0);
}

int	exec_pwd(t_info *info)
{
	write(1, ft_strjoin("PWD=", info->pwd), ft_strlen(info->pwd) + 4);
	write(1, "\n", 1);
	return (0);
}

int	exec_env(t_info *info)
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

int exec(t_info *info, char **command)
{
    char    *path;
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
    perror("command error");
	exit (1);
}

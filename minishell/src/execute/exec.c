/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/11 18:11:46 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*get_path(char *cmd)
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
	if (dirs[i])
	{
		free(dirs);
		return (cmdfile);
	}
	free(dirs);
	return (cmd);
}

int	exec_env(t_info *info)
{
	int	i;

	i = 0;
	while (info->env[i])
	{
		if (ft_len_to_char(info->env[i], '=') > -1)
		{
			write(2, info->env[i], ft_strlen(info->env[i]));
			write(2, "\n", 1);
		}
		i++;
	}
	return (0);
}

int	exec(t_info *info, char **command)
{
	char	*path;
	pid_t	id;
	int		status;

	id = fork();
	if (id == -1)
		perror("fork error");
	if (id == 0)
	{
		path = get_path(command[0]);
		dup2(2, 1);
		execve(path, command, info->env);
		set_error(info, 127, command[0], -4);
		exit (127);
	}
	else if (waitpid(id, &status, 0) == -1)
	{
        perror("waitpid() failed");
        exit(EXIT_FAILURE);
    }
	if (WIFEXITED(status))
        g_sig.exit_status = WEXITSTATUS(status);
	return (0);
}

int	exec_pwd(t_info *info)
{
	char	*pwd;

	pwd = ft_strjoin("PWD=", info->pwd);
	write(2, pwd, ft_strlen(pwd));
	write(2, "\n", 1);
	free (pwd);
	return (0);
}

void	ft_find_command(t_info *info, char **command, int oldfd)
{
	if (!ft_strncmp(command[0], "echo", 5))
		exec_echo(command);
	else if (!ft_strncmp(command[0], "cd", 3))
		exec_cd(info, command, 0);
	else if (!ft_strncmp(command[0], "pwd", 4))
		exec_pwd(info);
	else if (!ft_strncmp(command[0], "export", 7))
		exec_export(info, command);
	else if (!ft_strncmp(command[0], "unset", 6))
		exec_unset(info, command);
	else if (!ft_strncmp(command[0], "env", 4))
		exec_env(info);
	else if (!ft_strncmp(command[0], "exit", 5))
		exec_exit(info, command);
	else if (command[0])
		exec(info, command);
	if (oldfd)
		close(oldfd);
	free_strstr(command);
}

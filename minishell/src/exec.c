/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2021/12/10 20:09:58 by mgroen        ########   odam.nl         */
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

int	exec_export(t_info *info)
{
	int	i;

	i = 0;
	//if (...)
	//	add_export(info, "HOI=appel");
	while (info->export[i])
    {
        write(1, info->export[i], ft_strlen(info->export[i]));
		write(1, "\n", 1);
        i++;
    }
	return (0);
}

int	exec_unset(t_info *info)
{
	printf("unset\n");
	return (0);
}

int exec(t_info *info)
{
    char    **command;
    char    *path;
	int		id;

	if (ft_strncmp(info->line_read, "export", 6) == 0)
		return (exec_export(info));
	if (ft_strncmp(info->line_read, "unset", 6) == 0)
		return (exec_unset(info));
	id = fork();
	if (id == -1)
		perror("fork error");
	if (id)
	{
		waitpid(id, NULL, 0);
		return (0);
	}
	command = ft_split(info->line_read, ' ');
    path = get_path(command[0], info->env);
	execve(path, command, info->env);
    perror("command error");
	return (1);
}

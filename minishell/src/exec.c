/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2021/12/10 19:19:15 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*ft_strjoin_c(char const *s1, char const *s2)
{
	char	*new_str;
	int		i;
	int		l;

	i = 0;
	l = 0;
	if (!s1 || !s2)
		return (0);
	new_str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!new_str)
		return (0);
	while (s1[i] != '\0')
	{
		new_str[i] = s1[i];
		i++;
	}
	new_str[i] = '/';
	i++;
	while (s2[l] != '\0')
	{
		new_str[i + l] = s2[l];
		l++;
	}
	new_str[i + l] = '\0';
	return (new_str);
}

char	*get_path(char *cmd, char **env)
{
	char	*path;
	char	**dirs;
	char	*cmdfile;
	int		i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	path = env[i] + 5;
	i = 0;
	dirs = ft_split(path, ':');
	while (dirs[i])
	{
		cmdfile = ft_strjoin_c(dirs[i], cmd);
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

int exec(t_info *info)
{
    char    **command;
    char    *path;
	int		id;

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

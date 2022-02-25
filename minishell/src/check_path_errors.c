/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   check_path_errors.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/25 15:40:16 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/25 16:13:35 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	*is_a_dir(t_info *info, DIR *ret, char *command)
{
	set_error(info, 126, command, -5);
	closedir(ret);
	free(command);
	command = NULL;
	return (NULL);
}

static char	**build_path_parts(char *command, char *tmp)
{
	char	**commands;
	int		i;

	i = 0;
	commands = ft_split(command, '/');
	if (command[0] == '/')
	{
		tmp = ft_strdup(commands[0]);
		free(commands[0]);
		commands[0] = ft_strjoin("/", tmp);
		free(tmp);
	}
	while (commands[i])
	{
		commands[i] = ft_strjoinbas(commands[i], "/");
		i++;
	}
	return (commands);
}

static char	*not_a_dir_but_exists(
		t_info *info, int i, char **commands, char *command)
{
	char	*tmp;
	DIR		*ret;

	ret = opendir(commands[i]);
	if (ret == NULL)
	{
		tmp = ft_strdup(commands[i]);
		tmp[ft_strlen(tmp) - 1] = '\0';
		if (access(tmp, F_OK) == 0)
		{
			set_error(info, 126, command, -5);
			free(tmp);
			return (NULL);
		}
		free(tmp);
	}
	else
		closedir(ret);
	return ("OK");
}

static void	concatenate_paths(char **commands, int i)
{
	char	*tmp;

	tmp = ft_strdup(commands[i]);
	free(commands[i]);
	commands[i] = ft_strjoin(commands[i - 1], tmp);
	free(tmp);
	tmp = NULL;
}

char	*check_errors(t_info *info, DIR *ret, char *command, int i)
{
	char	**commands;

	if (ret != NULL)
		return (is_a_dir(info, ret, command));
	if (access(command, X_OK))
	{
		commands = build_path_parts(command, NULL);
		while (commands[i])
		{
			i++;
			if (not_a_dir_but_exists(info, i - 1, commands, command) == NULL)
				break ;
			if (commands[i])
				concatenate_paths(commands, i);
		}
		i--;
		if (access(commands[i], F_OK) != 0)
			set_error(info, 127, command, -5);
		free(command);
		command = NULL;
		free_strstr(commands);
		return (NULL);
	}
	return ("OK");
}

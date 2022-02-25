/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   check_path.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/09 18:40:34 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/25 15:29:36 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// checks whether it is a directory (when it shouldnt)
// whether it has permissions
// and whether it exists at all

void	change_pwd(t_info *info, char *command)
{
	int	i;

	i = 0;
	add_env(info, ft_strjoin("OLDPWD=", info->pwd));
	while (info->env[i] && ft_strncmp(info->env[i], "PWD=", 4))
		i++;
	if (!info->env[i])
		return (add_env(info, ft_strjoin("PWD=", command)));
	free(info->env[i]);
	info->env[i] = ft_strjoin("PWD=", command);
	free(info->pwd);
	info->pwd = ft_strdup(command);
	chdir(info->pwd);
	free_strstr(info->export);
	sort_export(info);
}

static char	*check_errors(t_info *info, DIR *ret, char *command)
{
	char	**commands;
	int		i;
	char	*tmp;

	if (ret != NULL)
	{
		set_error(info, 126, command, -5);
		closedir(ret);
		free(command);
		command = NULL;
		return (NULL);
	}
	if (access(command, X_OK))
	{
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
		i = 0;
		while (commands[i])
		{
			ret = opendir(commands[i]);
			if (ret == NULL)
			{
				tmp = ft_strdup(commands[i]);
				tmp[ft_strlen(tmp) - 1] = '\0';
				if (access(tmp, F_OK) == 0)
				{
					set_error(info, 126, command, -5);
					i++;
					free(tmp);
					break ;
				}
				free(tmp);
			}
			else
				closedir(ret);
			i++;
			if (commands[i])
			{
				tmp = ft_strdup(commands[i]);
				free(commands[i]);
				commands[i] = ft_strjoin(commands[i - 1], tmp);
				free(tmp);
			}
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

static char	*make_new(t_info *info, char *command, int loc, int i)
{
	char	*new;

	new = malloc(sizeof(char *) * (ft_strlen(command) - loc + 1));
	if (!new)
		ft_error(info, -1);
	i = 0;
	while (command[loc + i])
	{
		new[i] = command[loc + i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

char	*check_path(t_info *info, char *command)
{
	int		loc;
	int		i;
	char	*new;
	DIR		*ret;
	char	*str;

	if (!command)
		return (NULL);
	if (ft_strncmp(command, "/", 1) == 0)
	{
		ret = opendir(command);
		str = check_errors(info, ret, command);
		if (str == NULL)
			return (NULL);
	}
	i = ft_strlen(command) - 2;
	while (i > 0 && command[i] != '/')
		i--;
	if (i <= 0)
		return (command);
	ret = opendir(command);
	str = check_errors(info, ret, command);
	if (str == NULL)
		return (NULL);
	loc = i + 1;
	new = make_new(info, command, loc, i);
	free(command);
	command = NULL;
	return (new);
}

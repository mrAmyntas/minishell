/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   check_path.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/09 18:40:34 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/09 18:57:55 by mgroen        ########   odam.nl         */
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
	free (info->env[i]);
	info->env[i] = ft_strjoin("PWD=", command);
	free (info->pwd);
	info->pwd = ft_strdup(command);
	chdir(info->pwd);
	free_strstr(info->export);
	sort_export(info);
}

static char	*check_errors(t_info *info, DIR *ret, char *command)
{
	if (ret != NULL)
	{
		set_error(info, 126, command, -5);
		closedir(ret);
		free(command);
		return (NULL);
	}
	if (access(command, X_OK))
	{
		if (access(command, F_OK) == 0)
			set_error(info, 126, command, -5);
		else
			set_error(info, 127, command, -5);
		free(command);
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
	i = ft_strlen(command) - 2;
	while (command[i] != '/' && i > 0)
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
	return (new);
}

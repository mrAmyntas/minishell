/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   check_path.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/09 18:40:34 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/27 14:52:55 by mgroen        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// checks whether it is a directory (when it shouldnt)
// whether it has permissions
// and whether it exists at all

char	*find_path(char **dirs, char *cmd)
{
	int		i;
	char	*cmdfile;

	i = 0;
	while (dirs[i])
	{
		cmdfile = ft_strjoin(dirs[i], "/");
		cmdfile = ft_strjoinbas(cmdfile, cmd);
		free(dirs[i]);
		dirs[i] = NULL;
		if (access(cmdfile, F_OK) == 0)
			break ;
		free(cmdfile);
		cmdfile = NULL;
		i++;
	}
	return (cmdfile);
}

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
	info->env[i] = ft_strjoin("PWD=/", command);
	free(info->pwd);
	info->pwd = ft_strjoin("/", command);
	chdir(info->pwd);
	free_strstr(info->export);
	sort_export(info);
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
	int		i;
	char	*new;
	DIR		*ret;

	if (!command)
		return (NULL);
	if (ft_strncmp(command, "/", 1) == 0)
	{
		ret = opendir(command);
		if (check_errors(info, ret, command, 0) == NULL)
			return (NULL);
	}
	i = ft_strlen(command) - 2;
	while (i > 0 && command[i] != '/')
		i--;
	if (i <= 0)
		return (command);
	ret = opendir(command);
	if (check_errors(info, ret, command, 0) == NULL)
		return (NULL);
	new = make_new(info, command, (i + 1), i);
	free(command);
	command = NULL;
	return (new);
}

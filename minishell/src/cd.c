/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/01/26 17:18:00 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	pwd_is_home(t_info *info, char **command)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	add_env(info, ft_strjoin("OLDPWD=", info->pwd));
	while (info->env[i] && ft_strncmp(info->env[i], "PWD=", 4))
		i++;
	if (!info->env[i])
		add_env(info, info->pwd);
	while (info->env[j] && ft_strncmp(info->env[j], "HOME=", 5))
		j++;
	free (info->pwd);
	free (info->env[i]);
	if (!info->env[j])
	{
		add_env(info, ft_strjoin("HOME=", info->home));
		info->pwd = ft_strdup(info->home);
	}
	else
		info->pwd = ft_strdup(info->env[j] + 5);
	chdir(info->pwd);
	info->env[i] = ft_strjoin("PWD=", info->pwd);
	sort_export(info);
	free (command[0]);
	free (command);
}

void	trim_last_dir(t_info *info, char **command)
{
	int	i;
	int	len;

	i = 0;
	add_env(info, ft_strjoin("OLDPWD=", info->pwd));
	while (info->env[i] && ft_strncmp(info->env[i], "PWD=", 4))
		i++;
	if (!info->env[i])
		add_env(info, ft_strjoin("PWD=", info->pwd));
	len = ft_strlen(info->pwd);
	while (info->pwd[len] != '/' && len != 5)
		len--;
	info->pwd[len] = '\0';
	chdir(info->pwd);
	free (info->env[i]);
	info->env[i] = ft_strjoin("PWD=", info->pwd);
	sort_export(info);
	free (command[0]);
	free (command);
}

void	change_pwd(t_info *info, char **command)
{
	int	i;

	i = 0;
	add_env(info, ft_strjoin("OLDPWD=", info->pwd));
	while (info->env[i] && ft_strncmp(info->env[i], "PWD=", 4))
		i++;
	if (!info->env[i])
		return (add_env(info, ft_strjoin("PWD=", command[1])));
	free (info->env[i]);
	info->env[i] = ft_strjoin("PWD=", command[1]);
	free (info->pwd);
	info->pwd = ft_strdup(command[1]);
	chdir(info->pwd);
	i = 0;
	while (command[i])
	{
		free (command[i]);
		i++;
	}
	free (command);
	sort_export(info);
}

int	exec_cd(t_info *info, char **command)
{
	int		id;
	int		status;
	char	*path;

	if (command[1] && !ft_strncmp(command[1], "..", 2))
		trim_last_dir(info, command);
	if (command[1] && !ft_strncmp(command[1], "~", 1))
		pwd_is_home(info, command);
	if (!command[1] || !ft_strncmp(command[1], "..", 2) || !ft_strncmp(command[1], "~", 1))
		return (0);
	id = fork();
	if (id == -1)
		ft_error(info, 4);
	if (id)
	{
		if (command[1][0] != '/')
			make_dir(info, &command[1]);
		waitpid(id, &status, 0);
		if (status)
			return (0);
		change_pwd(info, command);
		return (0);
	}
	path = get_path(command[0], info->env);
	info->exit_status = execve(path, command, info->env);
	ft_error(info, 4);
	return (0);
}

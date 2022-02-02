/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/02 17:36:11 by mgroen        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
		add_env(info, ft_strjoin("PWD=", info->pwd));
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
	free_strstr(info->export);
	sort_export(info);
}

void	trim_last_dir(t_info *info, char **command)
{
	int		i;
	int		len;
	char	*temp;

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
	free(info->env[i]);
	info->env[i] = ft_strjoin("PWD=", info->pwd);
	free_strstr(info->export);
	sort_export(info);
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
	free_strstr(info->export);
	sort_export(info);
}

void	exec_cd(t_info *info, char **command)
{
	int		id;
	int		status;
	char	*path;

	if (!command[1])
		return ;
	if (command[1] && !ft_strncmp(command[1], "..", 3))
		return (trim_last_dir(info, command));
	else if (command[1] && !ft_strncmp(command[1], "~", 2))
		return (pwd_is_home(info, command));
	id = fork();
	if (id == -1)
		set_error(info, 1, NULL);
	if (id)
	{
		if (command[1][0] != '/')
			make_dir(info, &command[1]);
		waitpid(id, &status, 0);
		if (status)
			return ;
		change_pwd(info, command);
	}
	path = get_path(command[0], info->env);
	check_nosuchdir(info);
	execve(path, command, info->env);
}

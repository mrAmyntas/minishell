/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/09 17:35:19 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	pwd_is_home(t_info *info, char *command)
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
	free (command);
}

void	pwd_is_old(t_info *info, char *command)
{
	int	i;

	i = 0;
	if (!get_val(info, "OLDPWD"))
		return ;
	while (info->env[i] && ft_strncmp(info->env[i], "PWD=", 4))
		i++;
	if (info->env[i])
		add_env(info, ft_strjoin("PWD=", info->pwd));
	add_env(info, ft_strjoin("PWD=", get_val(info, "OLDPWD")));
	i = 0;
	while (info->env[i] && ft_strncmp(info->env[i], "OLDPWD=", 7))
		i++;
	add_env(info, ft_strjoin("OLDPWD=", info->pwd));
	free (info->pwd);
	info->pwd = get_val(info, "PWD");
	chdir(info->pwd);
	free_strstr(info->export);
	sort_export(info);
	free (command);
}

void	trim_last_dir(t_info *info, char *command)
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
	free (command);
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
	free (info->env[i]);
	info->env[i] = ft_strjoin("PWD=", command);
	free (info->pwd);
	info->pwd = ft_strdup(command);
	chdir(info->pwd);
	free_strstr(info->export);
	sort_export(info);
}

void	exec_cd2(t_info *info, char *command, int i, int x)
{
	char	*path;

	if (command && !ft_strncmp(command, "..", 3))
		return (trim_last_dir(info, command));
	else if (command && !ft_strncmp(command, "~", 2) && !i)
		return (pwd_is_home(info, command));
	else if (command && !ft_strncmp(command, "-", 2) && !i)
		return (pwd_is_old(info, command));
	if (!command || !ft_strncmp(command, ".", 2))
		return ;
	if (i || (!x && !i))
		command = make_dir(info, command);
	change_pwd(info, command);
	free (command);
}

void	exec_cd(t_info *info, char **command)
{
	char	**directions;
	char	*comm;
	int		i;
	int		x;
	
	x = 0;
	if (command[1][0] == '/')
		x = 1;
	directions = ft_split(command[1], '/');
	if (check_nosuchdir(info) == 1)
	{
		free_strstr(directions);
		return ;
	}
	i = 0;
	while (directions[i])
	{
		exec_cd2(info, ft_strdup(directions[i]), i, x);
		free (directions[i]); // hier of voor exec_cd2???
		i++;
	}
	free (directions);
}

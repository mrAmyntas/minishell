/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/03/03 14:56:13 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	pwd_is_home(t_info *info, char *command)
{
	int	i[2];

	i[0] = 0;
	i[1] = 0;
	add_env(info, ft_strjoin("OLDPWD=", info->pwd));
	while (info->env[i[0]] && ft_strncmp(info->env[i[0]], "PWD=", 4))
		i[0]++;
	if (!info->env[i[0]])
		add_env(info, ft_strjoin("PWD=", info->pwd));
	while (info->env[i[1]] && ft_strncmp(info->env[i[1]], "HOME=", 5))
		i[1]++;
	free(info->pwd);
	if (!info->env[i[1]])
	{
		add_env(info, ft_strjoin("HOME=", info->home));
		info->pwd = ft_strdup(info->home);
	}
	else
		info->pwd = ft_strdup(info->env[i[1]] + 5);
	chdir(info->pwd);
	add_env(info, ft_strjoin("PWD=", info->pwd));
	free_strstr(info->export);
	sort_export(info);
	free(command);
	command = NULL;
}

void	pwd_is_old(t_info *info, char *command)
{
	if (!get_val(info, "OLDPWD"))
	{
		free(command);
		command = NULL;
		return (set_error(info, 1, "cd: OLDPWD not set\n", -7));
	}
	add_env(info, ft_strjoin("PWD=", get_val(info, "OLDPWD")));
	add_env(info, ft_strjoin("OLDPWD=", info->pwd));
	free(info->pwd);
	info->pwd = ft_strdup(get_val(info, "PWD"));
	chdir(info->pwd);
	free_strstr(info->export);
	sort_export(info);
	free(command);
	command = NULL;
}

void	trim_last_dir(t_info *info, char *command)
{
	int		i;
	int		len;

	i = 0;
	add_env(info, ft_strjoin("OLDPWD=", info->pwd));
	while (info->env[i] && ft_strncmp(info->env[i], "PWD=", 4))
		i++;
	if (!info->env[i])
		add_env(info, ft_strjoin("PWD=", info->pwd));
	len = ft_strlen(info->pwd);
	while (info->pwd[len] != '/' && len != 1)
		len--;
	info->pwd[len] = '\0';
	chdir(info->pwd);
	add_env(info, ft_strjoin("PWD=", info->pwd));
	free_strstr(info->export);
	sort_export(info);
	free(command);
	command = NULL;
}

void	exec_cd2(t_info *info, char *command, int i, int x)
{
	if (command && !ft_strncmp(command, "..", 3))
		return (trim_last_dir(info, command));
	else if (command && !ft_strncmp(command, "~", 2) && !i)
		return (pwd_is_home(info, command));
	else if (command && !ft_strncmp(command, "-", 2) && !i)
		return (pwd_is_old(info, command));
	if (!command || !ft_strncmp(command, ".", 2))
	{
		free(command);
		command = NULL;
		return ;
	}
	if (i || (!x && !i))
		command = make_dir(info, command);
	change_pwd(info, command);
	free(command);
	command = NULL;
}

void	exec_cd(t_info *info, char **command, int x)
{
	char	**directions;
	int		i;

	if (command[1] && command[1][0] == '/')
		x = 1;
	if (command[1] && check_nosuchdir(info, command) == 1)
		return ;
	directions = ft_split(command[1], '/');
	if (!command[1])
		directions = exec_cd_noarg(command);
	i = 0;
	while (directions[i])
	{
		exec_cd2(info, ft_strdup(directions[i]), i, x);
		free(directions[i]);
		directions[i] = NULL;
		i++;
	}
	free(directions);
	directions = NULL;
}

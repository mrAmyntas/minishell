/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/03 16:44:08 by mgroen        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	pwd_is_home(t_info *info)
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

void	pwd_is_old(t_info *info)
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
}

void	trim_last_dir(t_info *info)
{
	int		i;
	int		len;
	char	*temp;

	i = 0;
	printf("test1\n");
	add_env(info, ft_strjoin("OLDPWD=", info->pwd));
	printf("test2\n");
	while (info->env[i] && ft_strncmp(info->env[i], "PWD=", 4))
		i++;
	printf("test3\n");
	if (!info->env[i])
		add_env(info, ft_strjoin("PWD=", info->pwd));
	printf("test4\n");
	len = ft_strlen(info->pwd);
	printf("test5\n");
	while (info->pwd[len] != '/' && len != 5)
		len--;
	printf("test6\n");
	info->pwd[len] = '\0';
	chdir(info->pwd);
	printf("test7\n");
	free(info->env[i]);
	info->env[i] = ft_strjoin("PWD=", info->pwd);
	printf("test8\n");
	free_strstr(info->export);
	sort_export(info);
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

void	exec_cd2(t_info *info, char *command, int i)
{
	char	*path;

	if (command && !ft_strncmp(command, "..", 3))
		return (trim_last_dir(info));
	else if (command && !ft_strncmp(command, "~", 2) && !i)
		return (pwd_is_home(info));
	else if (command && !ft_strncmp(command, "-", 2) && !i)
		return (pwd_is_old(info));
	if (!command || !ft_strncmp(command, ".", 2))
		return ;
	if (command[0] != '/')
		make_dir(info, &command);
	if (check_nosuchdir(info))
	{
		info->exit_status = 1;
		return ;
	}
	change_pwd(info, command);
}

void	exec_cd(t_info *info, char **command)
{
	char	**directions;
	int		i;

	directions = ft_split(command[1], '/');
	i = 0;
	while (directions[i])
	{
		exec_cd2(info, ft_strdup(directions[i]), i);
		free (directions[i]);
		i++;
	}
	free (directions);
}

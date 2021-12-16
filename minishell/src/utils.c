/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2021/12/15 18:57:44 by mgroen        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_len_to_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] != c && str[i])
		i++;
	if (!str[i])
		return (-1);
	return (i);
}

int	ft_strstrlen(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	unset_var(t_info *info, char *var)
{
	int	loc;

	loc = 0;
	while (info->env[loc] && strncmp(info->env[loc], var, ft_strlen(var)))
		loc++;
	if (!info->env[loc])
		return ;
	while (info->env[loc + 1])
	{
		free(info->env[loc]);
		info->env[loc] = ft_strdup(info->env[loc + 1]);
		loc++;
	}
	info->env[loc] = NULL;
}

void	sort_export(t_info *info)
{
	int	i;
	int	j;

	i = 1;
	info->export = malloc(sizeof(char **) * (ft_strstrlen(info->env) + 1));
	info->export[0] = ft_strdup(info->env[0]);
	while (info->env[i])
	{
		info->export[i] = NULL;
		j = ft_strstrlen(info->export) - 1;
		while (j >= 0)
		{
			if (ft_strncmp(info->env[i], info->export[j]
					, ft_len_to_char(info->env[i], '=')) > 0)
			{
				put_str(info->env[i], info->export, j + 1);
				break ;
			}
			else if (!j)
				put_str(info->env[i], info->export, 0);
			j--;
		}
		i++;
	}
	info->export[i] = NULL;
}

void	get_env(t_info *info, char **env)
{
	int	i;
	int	env_len;

	i = 0;
	env_len = ft_strstrlen(env) + 1;
	info->env = malloc(sizeof(char **) * env_len);
	while (env[i])
	{
		info->env[i] = ft_strdup(env[i]);
		i++;
	}
	info->env[i] = NULL;
}

int	ft_find_command(t_info *info)
{
	if (ft_strncmp(info->line_read, "echo", 4) == 0)
		return (exec(info));
	if (ft_strncmp(info->line_read, "cd", 2) == 0)
		return (exec_cd(info));
	if (ft_strncmp(info->line_read, "pwd", 3) == 0)
		return (exec_pwd(info));
	if (ft_strncmp(info->line_read, "export", 6) == 0)
		return (exec_export(info));
	if (ft_strncmp(info->line_read, "unset", 5) == 0)
		return (exec_unset(info));
	if (ft_strncmp(info->line_read, "env", 3) == 0)
		return (exec(info));
	if (ft_strncmp(info->line_read, "exit", 4) == 0)
		exit(0);
	return (15);
}

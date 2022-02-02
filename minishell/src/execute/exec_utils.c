/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/02 17:36:13 by mgroen        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_var(char **command, int i)
{
	int	j;

	j = 0;
	if (command[i][j] == '\\' || command[i][j] == '|')
			j++;
	if (command[i][j] < 65 || (command[i][j] > 90 && command[i][j] < 95)
		|| command[i][j] > 122 || command[i][j] == 96)
		perror(ft_strjoin(command[i], ": not a valid identifier"));
	else
	{
		while (command[i][j] && command[i][j] != '=')
		{
			if (command[i][j] < 48
				|| (command[i][j] > 57 && command[i][j] < 65)
					|| (command[i][j] > 90 && command[i][j] < 95)
						|| command[i][j] > 122 || command[i][j] == 96)
			{
				perror(ft_strjoin(command[i], ": not a valid identifier"));
				return (1);
			}
			j++;
		}
		return (0);
	}
	return (1);
}

void	unset_var(t_info *info, char *var)
{
	int	loc;

	loc = 0;
	while (info->env[loc] && ft_strncmp(info->env[loc], var, ft_strlen(var)))
		loc++;
	if (!info->env[loc])
		return ;
	while (info->env[loc + 1])
	{
		free(info->env[loc]);
		info->env[loc] = ft_strdup(info->env[loc + 1]);
		loc++;
	}
	free(info->env[loc]);
	info->env[loc] = NULL;
}

int	exec_export(t_info *info, char **command)
{
	int		i;
	int		j;

	i = 1;
	while (command[i])
	{
		if (!check_var(command, i))
			add_env(info, command[i]);
		i++;
	}
	i = 0;
	while (info->export[i] && !command[1])
	{
		write(1, "declare -x ", 11);
		write(1, info->export[i], ft_strlen(info->export[i]));
		write(1, "\n", 1);
		i++;
	}
	return (0);
}

int	exec_unset(t_info *info, char **command)
{
	int		i;

	i = 1;
	while (command[i])
	{
		unset_var(info, command[i]);
		i++;
	}
	free_strstr(info->export);
	sort_export(info);
	return (0);
}

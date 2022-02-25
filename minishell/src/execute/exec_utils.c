/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/25 16:53:02 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_var(char **command, int i, t_info *info)
{
	int	j;

	j = 0;
	if (command[i][j] == '\\' || command[i][j] == '|')
		j++;
	if (command[i][j] < 65 || (command[i][j] > 90 && command[i][j] < 95)
		|| command[i][j] > 122 || command[i][j] == 96)
		set_error(info, 2, command[i], -4);
	else
	{
		while (command[i][j] && command[i][j] != '=')
		{
			if (command[i][j] < 48
				|| (command[i][j] > 57 && command[i][j] < 65)
					|| (command[i][j] > 90 && command[i][j] < 95)
						|| command[i][j] > 122 || command[i][j] == 96)
			{
				set_error(info, 2, command[i], -4);
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
	while (info->env[loc] && (ft_strncmp(info->env[loc], var, ft_strlen(var))
			|| (ft_len_to_char(info->env[loc], '=') != ft_strlen(var)
				&& ft_len_to_char(info->env[loc], '=') > -1)))
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
	char	*temp;

	i = 1;
	while (command[i])
	{
		if (!check_var(command, i, info))
		{
			temp = ft_strdup(command[i]);
			if (!get_val(info, command[i]))
			{
				add_env(info, temp);
				free(temp);
				temp = NULL;
			}
			else
				add_env(info, temp);
		}
		i++;
	}
	print_export(info, command[1]);
	return (0);
}

int	exec_unset(t_info *info, char **command)
{
	int		i;

	i = 1;
	while (command[i])
	{
		if (!check_var(command, i, info))
			unset_var(info, command[i]);
		i++;
	}
	free_strstr(info->export);
	sort_export(info);
	return (0);
}

int	check_nosuchdir(t_info *info, char **command)
{
	int	i;
	DIR	*ret;

	i = 0;
	while (command[i] != NULL)
	{
		if ((ft_strncmp(command[i], "cd", 3) == 0
				&& ft_strlen(command[i]) == 2))
		{
			if (!ft_strncmp(command[i + 1], "~", 2)
				|| !ft_strncmp(command[i + 1], "-", 2))
				return (0);
			ret = opendir(command[i + 1]);
			if (ret == NULL)
			{
				set_error(info, 1, command[i + 1], -4);
				return (1);
			}
			else
				closedir(ret);
			return (0);
		}
		i++;
	}
	return (1);
}

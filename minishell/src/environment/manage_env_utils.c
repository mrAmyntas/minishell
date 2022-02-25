/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   manage_env_utils.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/25 13:24:36 by mgroen        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*make_dir(t_info *info, char *command)
{
	int		i;
	char	*temp;

	i = 0;
	if (!get_val(info, "PWD"))
		add_env(info, ft_strjoin("PWD=", info->pwd));
	temp = ft_strjoin("/", command);
	free(command);
	command = ft_strjoin(get_val(info, "PWD"), temp);
	free(temp);
	temp = NULL;
	return (command);
}

void	free_strstr(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		str[i] = NULL;
		i++;
	}
	if (str)
	{
		free(str);
		str = NULL;
	}
}

void	find_loc(t_info *info, int j, int i)
{
	while (j >= 0)
	{
		if (ft_strncmp(info->env[i], info->export[j]
				, ft_len_to_char(info->env[i], '=')) > 0)
		{
			put_str(info->env[i], info->export, j + 1);
			return ;
		}
		else if (!j)
			put_str(info->env[i], info->export, 0);
		j--;
	}
}

void	sort_export(t_info *info)
{
	int	i;
	int	j;

	i = 1;
	info->export = malloc(sizeof(char **)
			* (ft_strstrlen(info->env, NULL, 0) + 1));
	if (info->export == NULL)
		ft_error(info, -1);
	info->export[0] = ft_strdup(info->env[0]);
	while (info->env[i])
	{
		info->export[i] = NULL;
		j = ft_strstrlen(info->export, NULL, 0) - 1;
		find_loc(info, j, i);
		i++;
	}
	info->export[i] = NULL;
}

void	print_export(t_info *info, char *command)
{
	int	i;

	i = 0;
	while (info->export[i] && !command)
	{
		if (ft_strncmp(info->export[i], "_=", 2))
		{
			write(2, "declare -x ", 11);
			write(2, info->export[i], ft_strlen(info->export[i]));
			write(2, "\n", 1);
		}
		i++;
	}
}

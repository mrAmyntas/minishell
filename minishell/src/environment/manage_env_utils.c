/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   manage_env_utils.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/02 17:37:25 by mgroen        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	make_dir(t_info *info, char **command)
{
	int		i;
	char	*temp;

	i = 0;
	while (info->env[i] && strncmp(info->env[i], "PWD=", 4))
		i++;
	if (!info->env[i])
		add_env(info, info->pwd);
	temp = ft_strjoin("/", *command);
	free (*command);
	*command = ft_strjoin((info->env[i] + 4), temp);
	free (temp);
}

void	free_strstr(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	sort_export(t_info *info)
{
	int	i[2];

	i[0] = 1;
	info->export = malloc(sizeof(char **)
			* (ft_strstrlen(info->env, NULL, 0) + 1));
	info->export[0] = ft_strdup(info->env[0]);
	while (info->env[i[0]])
	{
		info->export[i[0]] = NULL;
		i[1] = ft_strstrlen(info->export, NULL, 0) - 1;
		while (i[1] >= 0)
		{
			if (ft_strncmp(info->env[i[0]], info->export[i[1]]
					, ft_len_to_char(info->env[i[0]], '=')) > 0)
			{
				put_str(info->env[i[0]], info->export, i[1] + 1);
				break ;
			}
			else if (!i[1])
				put_str(info->env[i[0]], info->export, 0);
			i[1]--;
		}
		i[0]++;
	}
	info->export[i[0]] = NULL;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   manage_env_utils.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/09 17:42:31 by mgroen        ########   odam.nl         */
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
	free (command);
	command = ft_strjoin(get_val(info, "PWD"), temp);
	free(temp);
	return (command);
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
	if (info->export == NULL)
		ft_error(info, -1);
	info->export[0] = ft_strdup(info->env[0]);
	while (info->env[i[0]])
	{
		info->export[i[0]] = NULL;
		i[1] = ft_strstrlen(info->export, NULL, 0) - 1;
		while (i[1] >= 0)
		{
			if (ft_strncmp(info->env[i[0]], info->export[i[1]]
					, ft_len_to_char(info->env[i[0]], '=')) > 0)
			//if (ft_strncmp(info->env[i[0]], info->export[i[1]]
			//		, ft_strlen(info->env[i[0]])) > 0)
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

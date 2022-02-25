/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/24 17:04:11 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/25 15:30:27 by bhoitzin      ########   odam.nl         */
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

void	get_env(t_info *info, char **env)
{
	int	i;
	int	env_len;

	i = 0;
	env_len = ft_strstrlen(env, NULL, 0);
	info->env = malloc(sizeof(char **) * (env_len + 1));
	if (info->env == NULL)
		ft_error(info, -1);
	while (env[i])
	{
		info->env[i] = ft_strdup(env[i]);
		i++;
	}
	info->env[i] = NULL;
}

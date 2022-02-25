/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   manage_env.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/25 13:21:59 by mgroen        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*get_val(t_info *info, char *var)
{
	int	i;
	int	len[2];

	if (!var)
		return (NULL);
	i = 0;
	len[0] = ft_len_to_char(var, '=');
	if (len[0] < 0)
		len[0] = ft_strlen(var);
	while (info->env[i])
	{
		len[1] = ft_len_to_char(info->env[i], '=');
		if (len[1] == -1)
			len[1] = ft_strlen(info->env[i]);
		if (!info->env[i]
			|| (!ft_strncmp(info->env[i], var, len[0]) && len[0] == len[1]))
			break ;
		i++;
	}
	if (!info->env[i])
		return (NULL);
	return (info->env[i] + len[0] + 1);
}

void	put_str(char *env, char **export, int j)
{
	int	i;

	i = ft_strstrlen(export, NULL, 0);
	while (i > j)
	{
		free(export[i]);
		export[i] = ft_strdup(export[i - 1]);
		i--;
	}
	free(export[j]);
	export[j] = ft_strdup(env);
}

void	copy_to_env(t_info *info, char **temp, char *new_var)
{
	int	i;
	int	temp_len;

	i = 0;
	temp_len = ft_strstrlen(temp, NULL, 0);
	info->env = malloc(sizeof(char **) * (temp_len + 2));
	if (info->env == NULL)
		ft_error(info, -1);
	while (i < temp_len)
	{
		info->env[i] = ft_strdup(temp[i]);
		free(temp[i]);
		temp[i] = NULL;
		i++;
	}
	free(temp);
	temp = NULL;
	info->env[i] = ft_strdup(new_var);
	info->env[i + 1] = NULL;
}

void	change_val(t_info *info, char *new_var)
{
	int	i;

	i = 0;
	if (ft_len_to_char(new_var, '=') == -1)
		return ;
	while (ft_strncmp(info->env[i], new_var, ft_len_to_char(new_var, '=') + 1))
		i++;
	free(info->env[i]);
	info->env[i] = ft_strdup(new_var);
	if (new_var)
	{
		free(new_var);
		new_var = NULL;
	}
	free_strstr(info->export);
	sort_export(info);
}

void	add_env(t_info *info, char *new_var)
{
	int		i;
	char	**temp;
	int		env_len;

	i = 0;
	if (get_val(info, new_var))
		return (change_val(info, new_var));
	env_len = ft_strstrlen(info->env, NULL, 0);
	temp = malloc(sizeof(char **) * (env_len + 1));
	if (temp == NULL)
		ft_error(info, -1);
	while (i < env_len)
	{
		temp[i] = ft_strdup(info->env[i]);
		free(info->env[i]);
		info->env[i] = NULL;
		i++;
	}
	temp[i] = NULL;
	free(info->env);
	info->env = NULL;
	copy_to_env(info, temp, new_var);
	free_strstr(info->export);
	sort_export(info);
}

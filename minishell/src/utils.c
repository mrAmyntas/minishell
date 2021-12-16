/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:40 by bhoitzin      #+#    #+#                 */
/*   Updated: 2021/12/16 16:36:38 by mgroen        ########   odam.nl         */
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

void	redirect(t_info *info, int type, char *files[2])
{
	int	fd[2];
	
	if (type == 1 || type == 5 || type == 7 || type == 3)
	{
		fd[0] = open(files[0], O_RDONLY);
		dup2(fd[0], 0);
	}
	if (type >= 4)
	{
		fd[1] = open(files[1], O_RDWR | O_TRUNC | O_CREAT, 0644);
		dup2(fd[1], 1);
	}
	if (fd[0] < 0 || fd[1] < 0)
		ft_error(3);
}

int	ft_find_command(t_info *info)
{
	int		i;
	int		type;
	char	*files[2];

	i = 0;
	type = 0;
	while (info->tokens[i])
	{
		if (info->tokens[i][0] == '<')
		{
			files[0] = ft_strdup(info->tokens[i + 1]);
			type += 1;
		}
		if (info->tokens[i][0] == '|')
			type += 2;
		if (info->tokens[i][0] == '>')
		{
			files[1] = ft_strdup(info->tokens[i + 1]);
			type += 4;
		}
		i++;
	}
	if (type != 2 && type != 0)
		redirect(info, type, files);
	if (ft_strncmp(info->tokens[0], "echo", 4) == 0)
		return (exec(info));
	if (ft_strncmp(info->tokens[0], "cd", 2) == 0)
		return (exec_cd(info));
	if (ft_strncmp(info->tokens[0], "pwd", 3) == 0)
		return (exec_pwd(info));
	if (ft_strncmp(info->tokens[0], "export", 6) == 0)
		return (exec_export(info));
	if (ft_strncmp(info->tokens[0], "unset", 5) == 0)
		return (exec_unset(info));
	if (ft_strncmp(info->tokens[0], "env", 3) == 0)
		return (exec_env(info));
	if (ft_strncmp(info->tokens[0], "exit", 4) == 0)
		exit(0);
	return (15);
}

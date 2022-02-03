/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   free.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/02 18:20:54 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/03 12:52:16 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_free(t_info *info)
{
	int	i;

	i = 0;
	while (info->tokens[i] != NULL)
	{
		free(info->tokens[i]);
		info->tokens[i] = NULL;
		i++;
	}
	free(info->tokens);
	free(info->token_state);
	info->t_pos = 0;
	info->p_pos = 0;
}

void	free_info(t_info *info)
{
	int	i;

	i = 0;
	free_strstr(info->export);
	while (info->env[i])
	{
		free (info->env[i]);
		i++;
	}
	free (info->env);
	free (info->pwd);
	free (info->home);
}

void	ft_init_struct(t_info *info, char **av, char **env)
{
	size_t	len;

	len = 10;
	info->av = av;
	get_env(info, env);
	info->redirect = 0;
	info->ret = 0;
	info->t_pos = 0;
	info->p_pos = 0;
	info->fd_std[0] = dup(0);
	info->fd_std[1] = dup(1);
	info->home = ft_strdup(getenv("HOME"));
	info->pwd = malloc(sizeof(char *) * len);
	if (info->pwd == NULL)
		ft_error(info, -1);
	info->exit_status = 0;
	while (!getcwd(info->pwd, len))
	{
		len += 10;
		free (info->pwd);
		info->pwd = malloc(sizeof(char *) * len);
		if (info->pwd == NULL)
			ft_error(info, -1);
	}
	sort_export(info);
}

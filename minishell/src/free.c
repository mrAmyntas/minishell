/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   free.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/02 18:20:54 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/10 13:44:06 by mgroen        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_stuff(t_info *info)
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

void	set_shlvl(t_info *info)
{
	int		num;
	char	*lvl;

	lvl = get_val(info, "SHLVL");
	if (!lvl)
		add_env(info, "SHLVL=1");
	else
	{
		num = ft_atoi(lvl);
		num += 1;
		lvl = ft_itoa(num);
		add_env(info, ft_strjoin("SHLVL=", lvl));
	}
	free(lvl);
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
	info->fd_std[2] = dup(2);
	info->home = ft_strdup(getenv("HOME"));
	info->pwd = malloc(sizeof(char *) * len);
	if (info->pwd == NULL)
		ft_error(info, -1);
	while (!getcwd(info->pwd, len))
	{
		len += 10;
		free (info->pwd);
		info->pwd = malloc(sizeof(char *) * len);
		if (info->pwd == NULL)
			ft_error(info, -1);
	}
	sort_export(info);
	set_shlvl(info);
}

void	parent_process(t_info *info, int pipefd[2], int loc_pipe)
{
	close(pipefd[1]);
	dup2(pipefd[0], 0);
	update_expand_exit_status(info);
	check_redirect_v2(info, loc_pipe + 1,
		ft_strstrlen(info->tokens, "|", loc_pipe + 1), pipefd);
}

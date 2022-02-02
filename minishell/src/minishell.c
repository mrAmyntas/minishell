/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 13:23:35 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/02 15:24:53 by mgroen        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_export(t_info *info)
{
	int	i;

	i = 0;
	while (info->export[i])
	{
		free (info->export[i]);
		i++;
	}
	free (info->export);
}

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
	while (info->export[i])
	{
		free (info->export[i]);
		i++;
	}
	free (info->export);
	i = 0;
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
	info->exit_status = 0;
	while (!getcwd(info->pwd, len))
	{
		len += 10;
		free (info->pwd);
		info->pwd = malloc(sizeof(char *) * len);
	}
	sort_export(info);
}

void	handle_sig(int signum)
{
	if (signum == SIGINT)
	{
		rl_on_new_line();
		rl_redisplay();
		ft_putstr_fd("  \b\b\n\033[0;33mminishell: \033[0m", 1);
		g_sig.sigint = 1;
	}
	if (signum == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
		write(1, "  \b\b", 5);
		g_sig.sigquit = 1;
	}
}

void	minishell(t_info *info)
{
	char *line_read = (char *)NULL; // static?
	
	while (1 == 1)
	{
		signal(SIGINT, &handle_sig);
		signal(SIGQUIT, &handle_sig);
		g_sig.sigquit = 0;
		g_sig.sigint = 0;
		if (line_read)
		{
			free(line_read);
			line_read = (char *)NULL;
		}
		line_read = readline("\033[0;33mminishell: \033[0m");
		if (!line_read && !g_sig.sigint && !g_sig.sigquit)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (!line_read || !line_read[0])
			continue ;
		lexer(info, line_read);
		parser(info);
		if (info->tokens[0] == NULL)
		{
			ft_free(info);
			continue ;
		}
		info->exit_status = 0;
		check_redirect_v2(info, 0, ft_strstrlen(info->tokens, "|", 0), 0);
		dup2(info->fd_std[0], 0);
		dup2(info->fd_std[1], 1);
		if (line_read && *line_read)
    		add_history(line_read);
		ft_free(info);
		//system("leaks minishell");
	}
}
int main(int ac, char **av, char **env)
{
	t_info	info;

	g_sig.sigint = 0;
	g_sig.sigquit = 0;
	ft_init_struct(&info, av, env);
	printf("\033[1;33mWelcome! You can exit by pressing Ctrl+D at any time...\033[1;33m\n");
	//printf("\033[1;33mWelcome Milan. History gefixt, ook de newlines na een enter zijn weg cntrl-D stopt nu altijd in 1 keer! Enige verschil is de output van bash met cmd exit en cntrl-D versus onze  ..\033[1;33m\n");
	minishell(&info);
	free_info(&info);
	//printf("leaks2\n");
	//system("leaks minishell");
	rl_clear_history();
	return (0);
}

// tokens: < > << >> | $
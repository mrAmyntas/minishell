/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 13:23:35 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/09 16:48:51 by mgroen        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

static char	*minishell_readline(t_info *info, char *line_read)
{
	signal(SIGINT, &handle_sig);
	signal(SIGQUIT, &handle_sig);
	g_sig.sigquit = 0;
	g_sig.sigint = 0;
	if (line_read)
	{
		free(line_read);
		line_read = (char *) NULL;
	}
	return (readline("\033[0;33mminishell: \033[0m"));
}

void	minishell_cont(t_info *info, char *line_read)
{
	g_sig.exit_status = 0;
	check_redirect_v2(info, 0, ft_strstrlen(info->tokens, "|", 0), 0);
	dup2(info->fd_std[0], 0);
	dup2(info->fd_std[1], 1);
	ft_free(info);
}

void	minishell(t_info *info)
{
	static char	*line_read = (char *) NULL;

	while (1 == 1)
	{
		line_read = minishell_readline(info, line_read);
		if (line_read && *line_read)
			add_history(line_read);
		if ((!line_read && (!g_sig.sigint || !g_sig.sigquit))
			|| (!line_read && g_sig.sigint && g_sig.sigquit))
		{
			write(0, "\033[0;33mexit: thanks for using minishell\033[0m\n", 45);
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
		minishell_cont(info, line_read);
		//system("leaks minishell");
	}
}

int	main(int ac, char **av, char **env)
{
	t_info	info;

	g_sig.sigint = 0;
	g_sig.sigquit = 0;
	ft_init_struct(&info, av, env);
	write(1, "\033[1;33mWelcome! You can exit by", 32);
	write(1, " pressing Ctrl+D at any time...\033[1;33m\n", 40);
	minishell(&info);
	free_info(&info);
	system("leaks minishell");
	rl_clear_history();
	return (0);
}

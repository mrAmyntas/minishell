/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 13:23:35 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/10 15:04:34 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	handle_sig(int signum)
{
	if (signum == SIGINT)
	{
		rl_on_new_line();
		rl_redisplay();
		ft_putstr_fd("  \b\b\nminishell: ", 1);
		g_sig.sigint = 1;
	}
	if (signum == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
		write(2, "  \b\b", 5);
		g_sig.sigquit = 1;
	}
}

static char	*minishell_readline(char *line_read)
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
	return (readline("minishell: "));
}

void	minishell_cont(t_info *info)
{
	int	set_fd[2];

	set_fd[0] = 0;
	set_fd[1] = 0;
	g_sig.exit_status = 0;
	check_redirect_v2(info, 0, ft_strstrlen(info->tokens, "|", 0), set_fd);
	free_stuff(info);
}

void	minishell(t_info *info)
{
	static char	*line_read = (char *) NULL;

	while (1 == 1)
	{
		line_read = minishell_readline(line_read);
		if (line_read && *line_read)
			add_history(line_read);
		if ((!line_read && (!g_sig.sigint || !g_sig.sigquit))
			|| (!line_read && g_sig.sigint && g_sig.sigquit))
		{
			write(0, "exit: thanks for using minishell\n", 33);
			break ;
		}
		if (!line_read || !line_read[0])
			continue ;
		lexer(info, line_read);
		parser(info);
		if (info->tokens[0] == NULL)
		{
			free_stuff(info);
			continue ;
		}
		minishell_cont(info);
	}
}

int	main(int ac, char **av, char **env)
{
	t_info	info;

	if (ac != 1)
		return (1);
	g_sig.sigint = 0;
	g_sig.sigquit = 0;
	g_sig.exit_status = 0;
	get_env(&info, env);
	ft_init_struct(&info, av);
	write(2, "Welcome! You can exit by", 24);
	write(2, " pressing Ctrl+D at any time...\n", 32);
	minishell(&info);
	free_info(&info);
	rl_clear_history();
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 13:23:35 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/25 12:59:20 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

//_=/Users/bhoitzin/minishellmilan/minishell/./minishell in env?
void	handle_sigint(int signum)
{
	if (!g_sig.id)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		ft_putstr_fd("  \b\b\nminishell: ", 2);
		rl_replace_line("", 0);
		g_sig.exit_status = 0;
	}
	else
	{
		write(2, "\n", 1);
		if (g_sig.id == 1)
			g_sig.exit_status = signum + 128;
		if (g_sig.id == 2)
		{
			g_sig.exit_status = 1;
			close(STDIN_FILENO);
			g_sig.sig = 4;
		}
	}
	if (!g_sig.sig || g_sig.sig == 1)
		g_sig.sig += 2;
	g_sig.id = 0;
}

void	handle_sigquit(int signum)
{
	if (g_sig.id == 1)
	{
		write(2, "Quit: ", 6);
		ft_putnbr_fd(signum, 2);
		write(2, "\n", 1);
		g_sig.exit_status = SIGQUIT + 128;
		g_sig.id = 0;
	}
	else
	{
		rl_on_new_line();
		rl_redisplay();
		write(2, "  \b\b", 5);
		g_sig.exit_status = 0;
	}
	if (!g_sig.sig || g_sig.sig == 2)
		g_sig.sig += 1;
}

void	minishell_cont(t_info *info, char *line_read)
{
	int	set_fd[2];

	if (!line_read)
		return ;
	parser(info);
	if (info->tokens[0] == NULL)
	{
		free_stuff(info);
		return ;
	}
	set_fd[0] = 0;
	set_fd[1] = 0;
	g_sig.exit_status = 0;
	g_sig.id = 1;
	check_redirect_v2(info, 0, ft_strstrlen2(info, "|", 0), set_fd);
	while (waitpid(-1, NULL, 0) != -1)
	{
	}
	g_sig.id = 0;
	free_stuff(info);
}

void	minishell(t_info *info)
{
	static char	*line_read = (char *) NULL;

	while (1 == 1)
	{
		info->first_process = 0;
		g_sig.sig = 0;
		if (line_read)
		{
			free(line_read);
			line_read = (char *) NULL;
		}
		line_read = readline("minishell: ");
		if (!line_read)
		{
			write(0, "exit: thanks for using minishell\n", 33);
			break ;
		}
		if (!line_read[0])
			continue ;
		add_history(line_read);
		line_read = lexer(info, line_read);
		minishell_cont(info, line_read);
	}
}

int	main(int ac, char **av, char **env)
{
	t_info	info;

	if (ac != 1)
		return (1);
	g_sig.sig = 0;
	g_sig.exit_status = 0;
	g_sig.id = 0;
	get_env(&info, env);
	ft_init_struct(&info, av);
	g_sig.act.sa_handler = handle_sigquit;
	g_sig.act2.sa_handler = handle_sigint;
	sigaction(SIGQUIT, &g_sig.act, NULL);
	sigaction(SIGINT, &g_sig.act2, NULL);
	write(2, "Welcome! You can exit by", 24);
	write(2, " pressing Ctrl+D at any time...\n", 32);
	minishell(&info);
	free_info(&info);
	rl_clear_history();
	return (0);
}

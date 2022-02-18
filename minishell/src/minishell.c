/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 13:23:35 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/18 18:36:09 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

//to-do:
// cat << END > out | cat << END > out2 schrijft wel naar out2
// maar niet naar out (als er iets in out staat, maakt hij dat wel leeg)
//
// control - C in heredoc doet het niet
// control - backslash in heredoc print nu de Quit 
// uit en delete alles wat er in stond
//
// << END echo hoi
// in bash doet dit nog gewoon echo hoi
// denk niet dat het op eval sheet stond dus meh

// 0x28
void	handle_sigint(int signum)
{
	if (!g_sig.id && g_sig.in_heredoc == 0)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		ft_putstr_fd("  \b\b\nminishell: ", 2);
		rl_replace_line("", 0);
		g_sig.exit_status = 0;
	}
	else if (g_sig.in_heredoc == 0)
	{
		write(2, "\n", 2);
		g_sig.exit_status = signum + 128;
	}
	g_sig.sig += 1;
	g_sig.id = 0;
	if (g_sig.in_heredoc == 1)
	{
		write(2, "\n", 2);
		close(STDIN_FILENO);
		g_sig.id = 2; // zodat execve de cat niet uitvoert en een error geeft, zie execve if statement
		return ;
	}
}

/*void	handle_sigquit(int signum)
{
	dprintf(0, "hoi\n");
	if (!g_sig.id)
	{
		rl_on_new_line();
		rl_redisplay();
		write(2, "  \b\b", 5);
		g_sig.exit_status = 0;
	}
	else
	{
		write(0, "Quit: ", 7);
		ft_putnbr_fd(signum, 2);
		write(0, "\n", 1);
		g_sig.exit_status = SIGQUIT + 128;
	}
	g_sig.sig += 1;
	g_sig.id = 0;
}*/

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
	check_redirect_v2(info, 0, ft_strstrlen(info->tokens, "|", 0), set_fd);
	while (waitpid(-1, NULL, 0) != -1)
		g_sig.exit_status2 = 0;
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
		if (line_read && *line_read)
			add_history(line_read);
		if ((!line_read && (!g_sig.sig || g_sig.sig == 1 || g_sig.sig == 2))
			|| (!line_read && g_sig.sig == 3))
		{
			write(0, "exit: thanks for using minishell\n", 33);
			break ;
		}
		if (!line_read || !line_read[0])
			continue ;
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
	g_sig.in_heredoc = 0;
	get_env(&info, env);
	ft_init_struct(&info, av);
	g_sig.act.sa_handler = SIG_IGN;
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

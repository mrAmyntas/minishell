/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 13:23:35 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/11 18:31:05 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// echo -n werkt nog niet
// echo met geen arguments werkt nog niet
// PWD= trimmen bij pwd

// $? expansion fixen

// '> aa cat | echo hoi' (in bash hangt ie, bij ons schijft hij echo hoi naar aa)
// hij ging fout bij trim_command -> heb hem ook met '>' de startpositie laten aanpassen
// maakt dat nog uit?
// hij laat nog niet de echo hoi zien, alleen hangen met de cat.
// dat is waar Maarten het over had lijkt me, met wachten op alle child processes
// en niet 1 voor 1.



// '< aaa cat | echo hoi' (bash voert uit eerst no such file or dir dan echo hoi2)
//
// bij ons: '< aaa cat | echo hoi' -> blijft hangen in de dup2 in exec
// OPGELOST DOOR In PIPE NIET NAAR ft_find_command TE GAAN MAAR NAAR
// ft_find_command2 DIE ALLEEN EXEC'D ALS ERROR STATUS 0 IS.
// weet niet zeker of dit niks anders kapot maakt.


// bij enkel cat of andere hangdingen dan gaat control-C helemaal los
// de ˆC krijg ik niet meer weg. Ik heb hem aangepast zodat ie de buffer leegt
// (met rl_replace_line("", 0)) -> Nu doet ie dat wel goed maar de control C is terug.

// de exit statusen vang ik nu op met waitpids (bij de forks bij pipe / exec)
// lijkt goed te werken maar wel weer grotere functies misschien kan dat nog kleiner
#include "../inc/minishell.h"

void	handle_sig(int signum)
{
	if (signum == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		write(2, "\nminishell: ", 13);
		rl_replace_line("", 0);
		g_sig.sigint = 1;
		g_sig.exit_status = SIGINT + 128;
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

	if (info->tokens[0] == NULL)
	{
		free_stuff(info);
		return ;
	}
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
		line_read = lexer(info, line_read);
		if (!line_read)
			continue ;
		parser(info);
		minishell_cont(info);
	}
}

int	main(int ac, char **av, char **env)
{
	t_info	info;

	if (ac != 1)
		return (1);
	signal(SIGINT, &handle_sig);
	signal(SIGQUIT, &handle_sig);
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

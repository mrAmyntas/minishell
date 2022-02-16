/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 13:23:35 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/16 16:06:40 by mgroen        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// $? expansion fixen

	//// cat /usr/share/dict/words | cat -e
	//// cat > output | echo hoi | echo hoi | echo hoi | echo hoi | echo hoi | echo hoi | echo hoi | echo hoi | echo hoi | echo hoi | echo hoi | echo hoi | echo hoi
	//// > aa cat | echo hoi
	//// < aaa cat | echo hoi

// hij ging fout bij trim_command -> heb hem ook met '>' de startpositie laten aanpassen
// maakt dat nog uit?
	//// volgensmij maakt dat idd niks uit, dus kan t gewoon.

// heb parent_process voor de wait gezet, lijkt te werken
// alleen het lijkt erop alsof de recursive versie van ons er voor zorgt dat de laatste
// die wij uitvoeren de eerst is die returned (al komen de error messages wel op volgorde???)
	//// We moeten niet na elke command wait callen, heb ze nu uitgecomment, moeten we nog even 
	//// naar kijken voor de exitstatus

// '< aaa cat | echo hoi' (bash voert uit eerst no such file or dir dan echo hoi2)
// bij ons: '< aaa cat | echo hoi' -> blijft hangen in de dup2 in exec
// OPGELOST DOOR In PIPE NIET NAAR ft_find_command TE GAAN MAAR NAAR
// ft_find_command2 DIE ALLEEN EXEC'D ALS ERROR STATUS 0 IS.
// weet niet zeker of dit niks anders kapot maakt.
	//// Heb de wait in exec weggehaald, hij bleef niet hangen bij dup2
	//// , maar je kon niks uitprinten omdat de output werd geredirect


// bij enkel cat of andere hangdingen dan gaat control-C helemaal los
// de ˆC krijg ik niet meer weg. Ik heb hem aangepast zodat ie de buffer leegt
// (met rl_replace_line("", 0)) -> Nu doet ie dat wel goed maar de control C is terug.
	//// met beide signals doet ie de output nu goed, functie is nu wel te groot
	//// en misschien nog even goed checken of ie het altijd goed doet

// de exit statusen vang ik nu op met waitpids (bij de forks bij pipe / exec)
// lijkt goed te werken maar wel weer grotere functies misschien kan dat nog kleiner

// doet de \b iets?
#include "../inc/minishell.h"

void	handle_sig(int signum)
{
	if (signum == SIGINT)
	{
		if (!g_sig.id)
		{
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
			ft_putstr_fd("  \b\b\nminishell: ", 2);
			rl_replace_line("", 0);
			g_sig.exit_status = 1;
		}
		else
		{
			ft_putstr_fd("\n", 2);
			g_sig.exit_status = SIGINT + 128;
		}
		g_sig.sig += 2;
	}
	if (signum == SIGQUIT)
	{
		if (!g_sig.id)
		{
			rl_on_new_line();
			rl_redisplay();
			write(2, "  \b\b", 5);
			g_sig.exit_status = 0;
		}
		else
		{
			write(2, "Quit: ", 6);
			ft_putnbr_fd(signum, 2);
			write(2, "\n", 1);
			g_sig.exit_status = SIGINT + 128;
		}
		g_sig.sig += 1;
	}
	g_sig.id = 0;
}

static char	*minishell_readline(char *line_read)
{
	g_sig.sig = 0;
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
	g_sig.id = 1;
	check_redirect_v2(info, 0, ft_strstrlen(info->tokens, "|", 0), set_fd);
	while (waitpid(-1, NULL, 0) != -1);
	g_sig.id = 0;
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
		if ((!line_read && (!g_sig.sig || g_sig.sig == 1 || g_sig.sig == 2))
			|| (!line_read && g_sig.sig == 3))
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
	g_sig.sig = 0;
	g_sig.exit_status = 0;
	g_sig.id = 0;
	get_env(&info, env);
	ft_init_struct(&info, av);
	signal(SIGINT, &handle_sig);
	signal(SIGQUIT, &handle_sig);
	write(2, "Welcome! You can exit by", 24);
	write(2, " pressing Ctrl+D at any time...\n", 32);
	minishell(&info);
	free_info(&info);
	rl_clear_history();
	return (0);
}

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include "../libft/libft.h"

typedef	struct	s_info
{
	int			ret;
	char		*line_read;
	int			cmd;
	int			redirect;
	char		**lines;
	char		**av;
	char		**env;
}				t_info;

enum	e_directions
{
	echo,
	cd,
	pwd,
	export,
	unset,
	env,
	exit_shell,
	redirect_in,
	redirect_out,
	redirect_in_delim,
	redirect_out_append,
	pipes,
	inhibit,
	inhibit_except_dollar,
	environment,
	command_not_found
};

int		ft_find_command(t_info *info);
int		ft_find_redirect(t_info *info);
int		exec(t_info *info);
void	parseline(t_info *info);
void	ft_init_struct(t_info *info, char **av, char **env);


#endif
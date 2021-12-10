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
	char		**tokens;
	int			t_pos;
	char		**av;
	char		**env;
	int			char_type;
	int			p_pos;
}				t_info;

enum	e_tokentype
{
	CHAR_NORMAL,
	CHAR_LESSER = '<',
	CHAR_GREATER = '>',
	CHAR_PIPE = '|',
	CHAR_DOLLAR = '$',
	CHAR_SEMICOLON = ';',
	CHAR_BACKSLASH = '\\',
	CHAR_SPACE = ' ',
	CHAR_QUOTE = '\'',
	CHAR_DQUOTE = '\"',
	CHAR_NEWLINE = '\n',
	CHAR_EOF = '\0',
	CHAR_DLESSER = 129,
	CHAR_DGREATER = 130
};

int		ft_find_command(t_info *info);
int		ft_find_redirect(t_info *info);
int		exec(t_info *info);
void	parseline(t_info *info);
void	ft_init_struct(t_info *info, char **av, char **env);
void	ft_error(int error_type);
int		ft_lexer(t_info *info);
int		check_char(t_info *info, int i);



#endif
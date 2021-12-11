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
	char		**export;
}				t_info;

enum	e_tokentype
{
	C_NORMAL,
	C_LESSER = '<',
	C_GREATER = '>',
	C_PIPE = '|',
	C_DOLLAR = '$',
//	C_SEMICOLON = ';',
//	C_BACKSLASH = '\\',
	C_SPACE = ' ',
	C_QUOTE = '\'',
	C_DQUOTE = '\"',
	C_NEWLINE = '\n',
	C_EOF = '\0',
	C_DLESSER = 129,
	C_DGREATER = 130
};

int		ft_find_command(t_info *info);
int		ft_find_redirect(t_info *info);
int		exec(t_info *info);
void	lexer(t_info *info);
void	ft_init_struct(t_info *info, char **av, char **env);
void	ft_error(int error_type);
int		store_input(t_info *info);
int		check_char(t_info *info, int i);
void    add_export(t_info *info, char *new_var);
void    sort_export(t_info *info);
int		parser(t_info *info);

#endif

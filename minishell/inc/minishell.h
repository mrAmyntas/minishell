#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
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
	char		*home;
	char		*pwd;
	int			fd_std[2];
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
void    add_env(t_info *info, char *new_var);
void    sort_export(t_info *info);
int		parser(t_info *info);
int		check_char_token(t_info *info, int i);
void    get_env(t_info *info, char **env);
int		exec_cd(t_info *info);
int		exec_unset(t_info *info);
int		exec_export(t_info *info);
void    unset_var(t_info *info, char *var);
int		exec_pwd(t_info *info);
void    make_dir(t_info *info, char **command);
char	*get_path(char *cmd, char **env);
char    *get_val(t_info *info, char *var);
int     ft_strstrlen(char **str);
int     ft_len_to_char(char *str, char c);
void    put_str(char *env, char **export, int j);
void	realloc_copy(t_info *info, int start, int incr);
int		exec_env(t_info *info);
int		check_before_after(t_info *info, int first_q, int last_q);
int		check_empty_quotes(t_info *info, int first_q, int last_q);


#endif

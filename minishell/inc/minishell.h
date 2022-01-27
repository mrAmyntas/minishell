#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <signal.h>
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
	int			*token_state;
	int			exit_status;
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
	C_DLESSER = 129,
	C_DGREATER = 130
};

typedef struct	s_sig
{
	int				sigint;
	int				sigquit;
}				t_sig;

t_sig	g_sig;
int		ft_find_command(t_info *info, char **command);
int		ft_find_redirect(t_info *info);
int		exec(t_info *info, char **command);
void	lexer(t_info *info);
void	ft_init_struct(t_info *info, char **av, char **env);
void	ft_error(t_info *info, int error_type);
int		store_input(t_info *info);
int		check_char(t_info *info, int i);
void    add_env(t_info *info, char *new_var);
void    sort_export(t_info *info);
int		parser(t_info *info);
int		check_char_token(t_info *info, int i, int j);
void    get_env(t_info *info, char **env);
int		exec_cd(t_info *info, char **command);
int		exec_unset(t_info *info, char **command);
int		exec_export(t_info *info, char **command);
void    unset_var(t_info *info, char *var);
int		exec_pwd(t_info *info);
void    make_dir(t_info *info, char **command);
char	*get_path(char *cmd, char **env);
char    *get_val(t_info *info, char *var);
int     ft_strstrlen(char **str, char *c, int i);
int     ft_len_to_char(char *str, char c);
void    put_str(char *env, char **export, int j);
void	realloc_copy(t_info *info, int start, int incr);
int		exec_env(t_info *info);
int		check_before_after(t_info *info, int first_q, int last_q);
int		check_empty_quotes(t_info *info, int first_q, int last_q);
int		parse_quotes(t_info *info, int i);
void	remove_spaces(t_info *info);
char	*ft_strjoinbas(t_info *info, char *s1, char const *s2);
int		check_unclosed(t_info *info, int i, int j);
int		cut_quotes(t_info *info, int pos, char c, int first);
void	check_dollar_token(t_info *info);
void	expand_dollar(t_info *info, int i);
void	merge_quotes(t_info *info, int first_q, int last_q, int n);
void	joinwithnormalbefore(t_info *info, int first_q);
void	set_token_state(t_info *info);
void	expand_exitstatus(t_info *info, int i);
void	expand_token_dollar(t_info *info, int i);
void	expand_str_dollar(t_info *info, int i, int pos);
int		join_tokens(t_info *info, int pos);
void	joinwithbefore(t_info *info, int first_q);
char	*get_name(t_info *info, int i, int j);
char	*get_val(t_info *info, char *var);
void	cut_dollar(t_info *info, int i, int start, int end);
void	expand_str_dollar3(t_info *info, int i, char *name, int end);
void	remove_quotes(t_info *info);
void	join_quoted_tokens(t_info *info);
void	join_quoted_tokens2(t_info *info, int i);
int		check_name(t_info *info, int i, int j);
void	expansion(t_info *info);
int		minishell(t_info *info);
void	ft_free(t_info *info);
int		check_redirect(t_info *info);
int		check_redirect_v2(t_info *info, int start, int end, int inputfd); // weet nog niet welke t wordt
char	*expand_buf(t_info *info, char *buf, int i);
void	check_dollar_in_quotes(t_info *info, int i);



#endif

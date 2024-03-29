/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/02 18:54:43 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/27 14:52:44 by mgroen        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <signal.h>
# include <dirent.h>
# include <errno.h>
# include <sys/wait.h>

typedef struct s_info
{
	int			ret;
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
	int			fd_std[3];
	int			*token_state;
	char		*exit_msg;
	int			first_process;
	int			error_done;
}				t_info;

typedef struct s_cmd
{
	int		j;
	int		start;
	int		end;
	char	**command;
	int		*arr;
}				t_cmd;

enum	e_tokentype
{
	C_NORMAL,
	C_LESSER = '<',
	C_GREATER = '>',
	C_PIPE = '|',
	C_DOLLAR = '$',
	C_SPACE = ' ',
	C_QUOTE = '\'',
	C_DQUOTE = '\"',
	C_NEWLINE = '\n',
	C_DLESSER = 129,
	C_DGREATER = 130
};

typedef struct s_sig
{
	int					exit_status;
	int					exit_status2;
	int					sig;
	int					id;
	struct sigaction	act;
	struct sigaction	act2;
}				t_sig;

t_sig	g_sig;
void	ft_find_command(t_info *info, char **command, int oldfd);
int		ft_find_redirect(t_info *info);
char	*lexer(t_info *info, char *line_read);
void	ft_init_struct(t_info *info, char **av);
void	ft_error(t_info *info, int i);
void	add_env(t_info *info, char *new_var);
void	sort_export(t_info *info);
void	parser(t_info *info);
int		check_char_token(t_info *info, int i, int j);
void	get_env(t_info *info, char **env);
void	exec_cd(t_info *info, char **command, int x);
int		exec_unset(t_info *info, char **command);
int		exec_export(t_info *info, char **command);
char	*make_dir(t_info *info, char *command);
int		ft_strstrlen(char **str, char *c, int i);
int		ft_len_to_char(char *str, char c);
void	put_str(char *env, char **export, int j);
void	realloc_copy(t_info *info, int start, int incr);
int		parse_quotes(t_info *info, int i);
char	*ft_strjoinbas(char *s1, char const *s2);
int		cut_quotes(t_info *info, int pos, char c, int first);
void	check_dollar_token(t_info *info);
void	expand_dollar(t_info *info, int i);
void	joinwithnormalbefore(t_info *info, int first_q);
void	expand_token_dollar(t_info *info, int i);
int		join_tokens(t_info *info, int pos);
void	joinwithbefore(t_info *info, int first_q);
char	*get_name(t_info *info, int i, int j);
char	*get_val(t_info *info, char *var);
void	cut_dollar(t_info *info, int i, int start, int end);
void	expand_str_dollar(t_info *info, int i, int pos);
void	join_quoted_tokens(t_info *info, int i);
int		check_name(t_info *info, int i, int j);
void	expand(t_info *info);
void	minishell(t_info *info);
void	free_stuff(t_info *info);
void	check_redirect_v2(t_info *info, int start, int end, int oldfd[2]);
char	*expand_buf(t_info *info, char *buf, int i);
void	check_dollar_in_quotes(t_info *info, int i);
void	set_error(t_info *info, int error_type, char *str, int token);
int		check_nosuchdir(t_info *info, char **command);
void	free_info(t_info *info);
void	free_strstr(char **str);
void	find_dgreater_dlesser(t_info *info);
int		check_char(int i, char *line_read);
void	ft_heredoc(t_info *info, int i);
char	*realloc_token(t_info *info, int i, int len);
size_t	ft_strlcpy2(char *dest, const char *src, size_t dstsize, int start);
void	find_syntax_error(t_info *info);
void	update_expand_exit_status(t_info *info);
char	*check_path(t_info *info, char *command);
void	change_pwd(t_info *info, char *command);
void	parent_process(t_info *info, int pipefd[2], int loc_pipe);
int		exec_exit(char **command);
void	exec_echo(t_info *info, char **command);
int		ft_strstrlen2(t_info *info, char *c, int i);
char	**trim_command2(t_info *info, int start, int end);
char	**exec_cd_noarg(char **command);
void	print_export(t_info *info, char *command);
char	*check_errors(t_info *info, DIR *ret, char *command, int i);
char	*find_path(char **dirs, char *cmd);

#endif

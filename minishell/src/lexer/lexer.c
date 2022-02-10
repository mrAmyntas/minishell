/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 19:03:32 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/10 19:41:42 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*ft_remove_spaces(t_info *info, char *line_read, int i)
{
	int		j;
	char	*buff;

	while (line_read[i] == ' ')
		i++;
	if (i == 0)
		return (line_read);
	j = ft_strlen(line_read);
	if (j == i)
		return (NULL);
	buff = (char *)malloc((sizeof(char) * j) - i + 1);
	if (buff == NULL)
		ft_error(info, -1);
	j = 0;
	while (line_read[i] != '\0')
	{
		buff[j] = line_read[i];
		i++;
		j++;
	}
	buff[j] = '\0';
	free(line_read);
	line_read = NULL;
	return (buff);
}

static void	store_char(t_info *info, int i, char *line_read)
{
	info->tokens[info->t_pos] = (char *)malloc(sizeof(char) * 2);
	if (info->tokens[info->t_pos] == NULL)
		ft_error(info, -1);
	info->tokens[info->t_pos][0] = line_read[i];
	info->tokens[info->t_pos][1] = '\0';
	info->t_pos++;
	info->p_pos++;
}

static void	store_string(t_info *info, int i, char *line_read)
{
	int	j;

	if (i == 0)
		return (store_char(info, i, line_read));
	if (check_char(i - 1, line_read) == 0 || info->p_pos == 0)
	{
		info->tokens[info->t_pos] = (char *)malloc(sizeof(char)
				* (i - info->p_pos + 1));
		if (info->tokens[info->t_pos] == NULL)
			ft_error(info, -1);
		j = 0;
		while (info->p_pos < i)
		{
			info->tokens[info->t_pos][j] = line_read[info->p_pos];
			info->p_pos++;
			j++;
		}
		info->tokens[info->t_pos][j] = '\0';
		info->t_pos++;
	}
	if (line_read[i] != '\0' )
		store_char(info, i, line_read);
}

static int	store_input(t_info *info, char *line_read)
{
	int	i;

	i = 0;
	while (line_read[i] != '\0')
	{
		info->char_type = check_char(i, line_read);
		if (info->char_type != 0)
			store_string(info, i, line_read);
		i++;
	}
	store_string(info, i, line_read);
	return (0);
}

char	*lexer(t_info *info, char *line_read)
{
	int	i;

	line_read = ft_remove_spaces(info, line_read, 0);
	if (!line_read)
	{
		info->tokens = (char **)malloc(sizeof(char *) * 1);
		info->tokens[0] = NULL;
		return (line_read);
	}
	i = ft_strlen(line_read);
	info->tokens = (char **)malloc(sizeof(char *) * (i + 2));
	if (info->tokens == NULL)
		ft_error(info, -1);
	info->token_state = (int *)malloc(sizeof(int) * (i + 2));
	if (info->token_state == NULL)
		ft_error(info, -1);
	while (i + 1 >= 0)
	{
		info->tokens[i + 1] = NULL;
		info->token_state[i + 1] = -1;
		i--;
	}
	store_input(info, line_read);
	return (line_read);
}

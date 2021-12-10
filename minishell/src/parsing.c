/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 11:34:31 by bhoitzin      #+#    #+#                 */
/*   Updated: 2021/12/10 12:14:39 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_redirect_in(t_info *info, int i)
{
	return (i);
}

int	ft_find_redirect(t_info *info)
{
	return (15);
}

int	ft_find_command(t_info *info)
{
	if (ft_strncmp(info->line_read, "echo", 4) == 0)
		return (exec(info));
	if (ft_strncmp(info->line_read, "cd", 2) == 0)
		return (exec(info));
	if (ft_strncmp(info->line_read, "pwd", 3) == 0)
		return (exec(info));
	if (ft_strncmp(info->line_read, "export", 6) == 0)
		return (exec(info));
	if (ft_strncmp(info->line_read, "unset", 5) == 0)
		return (exec(info));
	if (ft_strncmp(info->line_read, "env", 3) == 0)
		return (exec(info));
	if (ft_strncmp(info->line_read, "exit", 4) == 0)
		exit(0);
	return (15);
}

char	*ft_remove_spaces(t_info *info)
{
	int		i;
	int		j;
	char	*buff;

	i = 0;
	while (info->line_read[i] == ' ')
		i++;
	if (i == 0)
		return (info->line_read);
	j = ft_strlen(info->line_read);
	buff = (char *)malloc((sizeof(char) * j) - i + 1);
	j = 0;
	while (info->line_read[i] != '\0')
	{
		buff[j] = info->line_read[i];
		i++;
		j++;
	}
	buff[j] = '\0';
	free(info->line_read);
	info->line_read = NULL;
	return (buff);
}

void	parseline(t_info *info)
{
	info->line_read = ft_remove_spaces(info);
	info->redirect = ft_find_redirect(info);
	info->cmd = ft_find_command(info);
	if (info->cmd == 15)
	{
		printf("minishell: command not found: %s\n", info->line_read);
		return ;
	}
}
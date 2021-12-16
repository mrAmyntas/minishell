/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 15:05:11 by bhoitzin      #+#    #+#                 */
/*   Updated: 2021/12/16 15:55:13 by mgroen        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_error(int error_type)
{
	if (error_type == 1)
		perror("malloc error:");
	if (error_type == 2)
		perror("unclosed quote:");
	if (error_type == 3)
		perror("no such file or directory.");
	if (error_type == 4)
		perror("fork error.");
	exit(1);
}

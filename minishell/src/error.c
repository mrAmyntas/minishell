/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 15:05:11 by bhoitzin      #+#    #+#                 */
/*   Updated: 2021/12/15 19:04:55 by mgroen        ########   odam.nl         */
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
		perror("No such file or directory.");
	exit(1);
}

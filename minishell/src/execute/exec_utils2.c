/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_utils2.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/11 15:06:41 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/16 18:04:08 by bhoitzin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	ft_atoi_llong_check(char *str)
{
	long long unsigned int	i;
	long long unsigned int	n;

	i = 0;
	n = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] != '\0')
	{
		n = n * 10 + str[i] - 48;
		i++;
	}
	i = LLONG_MAX;
	if (n > i)
		return (-2);
	return (n);
}

static int	ft_check_isdigit(char *command)
{
	int	i;

	i = 0;
	if (command[i] == '-' || command[i] == '+')
		i++;
	while (command[i] != '\0')
	{
		if (ft_isdigit(command[i]) == 0)
			return (-1);
		i++;
	}
	return (0);
}

int	exec_exit(char **command)
{
	long long unsigned int	x;

	x = 0;
	if (command[1])
	{
		x = ft_atoi(command[1]);
		if (ft_check_isdigit(command[1]) == -1
			|| (ft_atoi(command[1]) != -2 && ft_atoi_llong_check(command[1]) == -2))
		{
			write(2, "exit: ", 7);
			write(2, command[1], ft_strlen(command[1]));
			write(2, ": numeric argument required\n", 29);
			exit(255);
		}
	}
	if (command[1] && command[2])
	{
		write(2, "exit: too many arguments\n", 25);
		g_sig.exit_status = 1;
		return (1);
	}
	write(2, "exit\n", 6);
	exit(x);
}

void	exec_echo(char **command)
{
	int	i;

	i = 1;
	while (command[i])
	{
		if (i != 1 || (i == 1 && ft_strncmp(command[1], "-n", 3)))
		{
			write(2, command[i], ft_strlen(command[i]));
			if (command[i + 1])
				write(2, " ", 1);
		}
		i++;
	}
	if (ft_strncmp(command[1], "-n", 3))
		write(2, "\n", 1);
}

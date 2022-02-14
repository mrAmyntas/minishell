/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_utils2.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/11 15:06:41 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/14 16:24:21 by mgroen        ########   odam.nl         */
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
	if (command[1] && command[2])
	{
		write(2, "exit: too many arguments\n", 25);
		g_sig.exit_status = 1;
		return (1);
	}
	if (command[1])
	{
		x = ft_atoi(command[1]);
		if (ft_check_isdigit(command[1]) == -1
			|| (x != (long long unsigned int)-2 && ft_atoi_llong_check(command[1]) == -2))
		{
			write(2, "exit: ", 7);
			write(2, command[1], ft_strlen(command[1]));
			write(2, ": numeric argument required\n", 29);
			exit(255);
		}
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

/*
void	check_redirect_v3(t_info *info, int start, int end, int oldfd[2])
{
	int		fd[3];
	char	**command;

	fd[0] = 0;
	fd[1] = 0;
	fd[2] = find_redirect(info, start, fd, end);
	if (fd[2] >= 0)
		return (ft_pipe(info, start, fd, oldfd));
	if (fd[0] < 0 || fd[1] < 0)
		return ;
	command = trim_command(info, start, end);
	command[0] = check_path(info, command[0]);
	return (ft_find_command(info, command, oldfd[0]));
}*/
/*
void	ft_find_command2(t_info *info, char **command, int oldfd)
{
	if (!ft_strncmp(command[0], "echo", 5))
		exec_echo(command);
	else if (!ft_strncmp(command[0], "cd", 3))
		exec_cd(info, command, 0);
	else if (!ft_strncmp(command[0], "pwd", 4))
		exec_pwd(info);
	else if (!ft_strncmp(command[0], "export", 7))
		exec_export(info, command);
	else if (!ft_strncmp(command[0], "unset", 6))
		exec_unset(info, command);
	else if (!ft_strncmp(command[0], "env", 4))
		exec_env(info);
	else if (!ft_strncmp(command[0], "exit", 5))
		exec_exit(info, command);
	else if (command[0] && g_sig.exit_status == 0)
		exec(info, command);
	if (oldfd)
		close(oldfd);
	free_strstr(command);
}*/

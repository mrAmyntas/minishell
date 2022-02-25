/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   trim_command.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: bhoitzin <bhoitzin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/24 15:11:14 by bhoitzin      #+#    #+#                 */
/*   Updated: 2022/02/25 13:48:50 by mgroen        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	set_start(t_info *info, t_cmd *cmd)
{
	if ((!ft_strncmp(info->tokens[cmd->start], "<", 2)
			|| !ft_strncmp(info->tokens[cmd->start], "<<", 3)
			|| !ft_strncmp(info->tokens[cmd->start], ">>", 3)
			|| !ft_strncmp(info->tokens[cmd->start], ">", 2)
			|| !ft_strncmp(info->tokens[cmd->start], "|", 2))
		&& info->token_state[cmd->start] == 1)
		cmd->start += 2;
	else if (!ft_strncmp(info->tokens[cmd->start + 1], "<<", 3)
		&& info->token_state[cmd->start + 1])
		cmd->start += 3;
}

static void	set_arr(t_info *info, t_cmd *cmd, int i)
{
	cmd->arr = (int *)malloc(sizeof(int) * cmd->end);
	if (cmd->arr == NULL)
		ft_error(NULL, -1);
	while (i < cmd->end)
	{
		cmd->arr[i] = 0;
		i++;
	}
	i = 0;
	while ((cmd->start + i) < cmd->end)
	{
		if ((!ft_strncmp(info->tokens[cmd->start + i], "<", 2)
				|| !ft_strncmp(info->tokens[cmd->start + i], "<<", 3)
				|| !ft_strncmp(info->tokens[cmd->start + i], ">>", 3)
				|| !ft_strncmp(info->tokens[cmd->start + i], ">", 2)
				|| !ft_strncmp(info->tokens[cmd->start + i], "|", 2))
			&& info->token_state[cmd->start + i] == 1)
		{
			cmd->arr[cmd->start + i] = -1;
			cmd->arr[cmd->start + i + 1] = -1;
		}
		else if (info->token_state[cmd->start + i] == 1)
			break ;
		i++;
	}
}

static	void	set_command(t_info *info, t_cmd *cmd, int start)
{
	int	i;

	cmd->command = malloc(sizeof(char **) * (cmd->end + 1));
	if (cmd->command == NULL)
		ft_error(info, -1);
	if (start + 3 == cmd->start)
		cmd->command[0] = ft_strdup(info->tokens[0]);
	i = cmd->j;
	while (cmd->start < cmd->end)
	{
		if (cmd->arr[cmd->start] == 0)
		{
			cmd->command[i] = ft_strdup(info->tokens[cmd->start]);
			i++;
		}
		cmd->start++;
	}
	cmd->command[i] = NULL;
}

char	**trim_command2(t_info *info, int start, int end)
{
	t_cmd	cmd;

	cmd.j = 0;
	cmd.start = start;
	cmd.end = end;
	set_start(info, &cmd);
	if (start + 3 == cmd.start)
		cmd.j++;
	set_arr(info, &cmd, 0);
	set_command(info, &cmd, start);
	free(cmd.arr);
	cmd.arr = NULL;
	return (cmd.command);
}

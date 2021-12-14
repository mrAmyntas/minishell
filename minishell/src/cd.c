#include "../inc/minishell.h"

void    get_oldpwd(t_info *info)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (info->env[i] && ft_strncmp(info->env[i], "PWD", 3))
        i++;
    if (!info->env[i])
        add_env(info, info->pwd);
    while (info->env[j] && ft_strncmp(info->env[j], "OLDPWD", 6))
        j++;
    if (!info->env[j])
        add_env(info, ft_strjoin("OLD", info->env[i]));
    else
    {    
        free (info->env[j]);
        info->env[j] = ft_strjoin("OLD", info->env[i]);
    }
}

void    pwd_is_home(t_info *info)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (info->env[i] && ft_strncmp(info->env[i], "PWD", 3))
        i++;
    if (!info->env[i])
        add_env(info, info->pwd);
    while (info->env[j] && ft_strncmp(info->env[j], "HOME", 3))
        j++;
    if (!info->env[j])
        printf("No HOME set\n");
    free (info->env[i]);
    info->env[i] = ft_strjoin("PWD=", (info->env[j] + 5));
    sort_export(info);
}

void    trim_last_dir(t_info *info)
{
    int i;
    int len;

    i = 0;
    get_oldpwd(info);
    while (info->env[i] && ft_strncmp(info->env[i], "PWD", 3))
        i++;
    if (!info->env[i])
        add_env(info, info->pwd); 
    len = ft_strlen(info->env[i]);
    while (info->env[i][len] != '/' && len != 5)
        len--;
    info->env[i][len] = '\0';
    sort_export(info);
}

void    change_pwd(t_info *info, char **command)
{
    int i;

    i = 0;
    get_oldpwd(info);
    while (info->env[i] && ft_strncmp(info->env[i], "PWD", 3))
        i++;
    if (!info->env[i])
        add_env(info, command[1]);
    free (info->env[i]);
    info->env[i] = ft_strjoin("PWD=", command[1]);
    i = 0;
    while (command[i])
    {
        free (command[i]);
        i++;
    }
    free (command);
    sort_export(info);
}

int	exec_cd(t_info *info)
{
	int		id;
	int		status;
	char	**command;
	char	*path;

	command = ft_split(info->line_read, ' ');
	if (!ft_strncmp(command[1], "..", 2))
		trim_last_dir(info);
	if (!ft_strncmp(command[1], "~", 1))
		pwd_is_home(info);
	if (!ft_strncmp(command[1], "..", 2) || !ft_strncmp(command[1], "~", 1))
		return (0);
	id = fork();
	if (id == -1)
		perror("fork error");
	if (command[1][0] != '/')
		make_dir(info, &command[1]);
	if (id)
	{
		waitpid(id, &status, 0);
		if (status)
			return (0);
		change_pwd(info, command);
		return (0);
	}
    path = get_path(command[0], info->env);
	execve(path, command, info->env);
    perror("No such file or directory");
	exit (1);
}

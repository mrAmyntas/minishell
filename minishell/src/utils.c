#include "../inc/minishell.h"

int     ft_len_to_char(char *str, char c)
{
    int i;

    i = 0;
    while (str[i] != c && str[i])
        i++;
    if (!str[i])
        return (-1);
    return (i);
}

int     ft_strstrlen(char **str)
{
    int i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

void    make_dir(t_info *info, char **command)
{
    int     i;
    char    *temp;

    i = 0;
    while (info->env[i] && strncmp(info->env[i], "PWD", 3))
        i++;
    if (!info->env[i])
        add_env(info, info->pwd);
    temp = ft_strjoin("/", *command);
    free (*command);
    *command = ft_strjoin((info->env[i] + 4), temp);
    free (temp);
}

void    put_str(char *env, char **export, int j)
{
    int i;

    i = ft_strstrlen(export);
    while (i > j)
    {
        export[i] = ft_strdup(export[i - 1]);
        i--;
    }
    export[j] = ft_strdup(env);
}

void    copy_to_env(t_info *info, char **temp, char *new_var)
{
    int i;

    i = 0;
    info->env = malloc(sizeof(char **) * (ft_strstrlen(temp) + 1));
    while (i < ft_strstrlen(temp))
    {
        info->env[i] = malloc(sizeof(char *) * ft_strlen(temp[i]));
        info->env[i] = ft_strdup(temp[i]);
        free(temp[i]);
        i++; 
    }
    info->env[i] = NULL;
    free (temp);
    i = ft_strstrlen(info->env);
    info->env[i] = ft_strdup(new_var);
}

void    unset_var(t_info *info, char *var)
{
    int loc;

	loc = 0;
	while (info->env[loc] && strncmp(info->env[loc], var, ft_strlen(var)))
		loc++;
    if (!info->env[loc])
		return ;
    if (!strncmp(var, "PWD", 3))
        info->pwd = ft_strdup(info->env[loc]);
	while (info->env[loc + 1])
	{
		free(info->env[loc]);
		info->env[loc] = ft_strdup(info->env[loc + 1]);
		loc++;
	}
    info->env[loc] = NULL;
}

void    add_env(t_info *info, char *new_var)
{
    int     i;
    char    **temp;
    i = 0;

    temp = malloc(sizeof(char **) * (ft_strstrlen(info->env) + 1));
    while (i < ft_strstrlen(info->env))
    {
        temp[i] = ft_strdup(info->env[i]);
        free(info->env[i]);
        i++;
    }
    temp[i] = NULL;
    free (info->env);
    copy_to_env(info, temp, new_var);
    sort_export(info);
}

void    sort_export(t_info *info)
{
    int     i;
    int     j;

    i = 1;
    info->export = malloc(sizeof(char **) * (ft_strstrlen(info->env) + 1));
    info->export[0] = ft_strdup(info->env[0]);
    while (info->env[i])
    {
        info->export[i] = NULL;
        j = ft_strstrlen(info->export) - 1;
        while (j >= 0)
        {
            if (ft_strncmp(info->env[i], info->export[j], ft_len_to_char(info->env[i], '=')) > 0)
            {
                put_str(info->env[i], info->export, j + 1);
                break ;
            }
            else if (!j)
                put_str(info->env[i], info->export, 0);  
            j--;
        }
        i++;
    }
    info->export[i] = NULL;
}

void    get_env(t_info *info, char **env)
{
    int i;
    int env_len;

    i = 0;
    env_len = ft_strstrlen(env) + 1;
    info->env = malloc(sizeof(char **) * env_len);
    while (env[i])
    {
        info->env[i] = ft_strdup(env[i]);
        i++;
    }
    info->env[i] = NULL;
}

int	ft_find_command(t_info *info)
{
	if (ft_strncmp(info->line_read, "echo", 4) == 0)
		return (exec(info));
	if (ft_strncmp(info->line_read, "cd", 2) == 0)
		return (exec_cd(info));
	if (ft_strncmp(info->line_read, "pwd", 3) == 0)
		return (exec_pwd(info));
	if (ft_strncmp(info->line_read, "export", 6) == 0)
		return (exec_export(info));
	if (ft_strncmp(info->line_read, "unset", 5) == 0)
		return (exec_unset(info));
	if (ft_strncmp(info->line_read, "env", 3) == 0)
		return (exec(info));
	if (ft_strncmp(info->line_read, "exit", 4) == 0)
		exit(0);
	return (15);
}

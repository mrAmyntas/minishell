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

void    copy_to_export(t_info *info, char **temp, char *new_var)
{
    int i;

    i = 0;
    info->export = malloc(sizeof(char **) * (ft_strstrlen(temp) + 1));
    while (i < ft_strstrlen(temp))
    {
        info->export[i] = malloc(sizeof(char *) * ft_strlen(temp[i]));
        info->export[i] = ft_strdup(temp[i]);
        free(temp[i]);
        i++; 
    }
    info->export[i] = NULL;
    free (temp);
    i = ft_strstrlen(info->export) - 1;
    while (i >= 0)
    {
        if (ft_strncmp(new_var, info->export[i], ft_len_to_char(new_var, '=')) > 0)
        {
            put_str(new_var, info->export, i + 1);
            break ;
        }
        else if (!i)
            put_str(new_var, info->export, 0);  
        i--;
    }
}

void    add_export(t_info *info, char *new_var)
{
    int     i;
    char    **temp;
    i = 0;
    
    temp = malloc(sizeof(char **) * (ft_strstrlen(info->export) + 1));
    while (i < ft_strstrlen(info->export))
    {
        temp[i] = malloc(sizeof(char *) * ft_strlen(info->export[i]));
        temp[i] = ft_strdup(info->export[i]);
        free(info->export[i]);
        i++;
    }
    temp[i] = NULL;
    free (info->export);
    copy_to_export(info, temp, new_var);
}

void    sort_export(t_info *info)
{
    int     i;
    int     j;

    i = 1;
    info->export = malloc(sizeof(char **) * 25);
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
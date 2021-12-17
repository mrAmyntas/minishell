#include "../inc/minishell.h"

char	*get_val(t_info *info, char *var)
{
	int	i;
	int	len[2];

	i = 0;
	len[0] = ft_len_to_char(var, '=');
	if (len[0] < 0)
		len[0] = ft_strlen(var);
	while (info->env[i])
	{
		len[1] = ft_len_to_char(info->env[i], '=');
		if (len[1] == -1)
			len[1] = ft_strlen(info->env[i]);
		if (!info->env[i] || (!ft_strncmp(info->env[i], var, len[0]) && len[0] == len[1]))
			break ;
		i++;
	}
	if (!info->env[i])
		return (NULL);
	return (info->env[i] + ft_strlen(var) + 1);
}

void	make_dir(t_info *info, char **command)
{
	int		i;
	char	*temp;

	i = 0;
	while (info->env[i] && strncmp(info->env[i], "PWD=", 4))
		i++;
	if (!info->env[i])
		add_env(info, info->pwd);
	temp = ft_strjoin("/", *command);
	free (*command);
	*command = ft_strjoin((info->env[i] + 4), temp);
	free (temp);
}

void	put_str(char *env, char **export, int j)
{
	int	i;

	i = ft_strstrlen(export);
	while (i > j)
	{
		free (export[i]);
		export[i] = ft_strdup(export[i - 1]);
		i--;
	}
	free (export[j]);
	export[j] = ft_strdup(env);
}

void	copy_to_env(t_info *info, char **temp, char *new_var)
{
	int	i;

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

void	add_env(t_info *info, char *new_var)
{
	int		i;
	char	**temp;

	i = 0;
	if (get_val(info, new_var))
	{
		if (ft_len_to_char(new_var, '=') == -1)
			return ;
		while (ft_strncmp(info->env[i], new_var, ft_len_to_char(new_var, '=')))
			i++;
		free (info->env[i]);
		info->env[i] = ft_strdup(new_var);
		sort_export(info);
		return ;
	}
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

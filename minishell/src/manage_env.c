#include "../inc/minishell.h"

char	*get_val(t_info *info, char *var)
{
	int	i;
	int	len[2];

	if (var == NULL)
		return (NULL);
	i = 0;
	len[0] = ft_len_to_char(var, '=');
	if (len[0] < 0)
	{
		len[0] = ft_strlen(var);
	}
	while (info->env[i])
	{
		len[1] = ft_len_to_char(info->env[i], '=');
		if (len[1] == -1)
		{
			len[1] = ft_strlen(info->env[i]);
		}
		if (!info->env[i] || (!ft_strncmp(info->env[i], var, len[0]) && len[0] == len[1]))
		{
			break ;
		}
		i++;
	}
	if (!info->env[i])
	{
		return (NULL);
	}
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

	i = ft_strstrlen(export, NULL, 0);
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
	int	temp_len;

	i = 0;
	temp_len = ft_strstrlen(temp, NULL, 0);
	info->env = malloc(sizeof(char **) * (temp_len + 1));
	while (i < temp_len)
	{
		info->env[i] = malloc(sizeof(char *) * ft_strlen(temp[i]));
		info->env[i] = ft_strdup(temp[i]);
		free(temp[i]);
		i++;
	}
	info->env[i] = NULL;
	free (temp);
	i = ft_strstrlen(info->env, NULL, 0);
	info->env[i] = ft_strdup(new_var);
}

void	add_env(t_info *info, char *new_var)
{
	int		i;
	char	**temp;
	int		env_len;
	//int t = 0;

	i = 0;
	if (get_val(info, new_var))
	{
		if (ft_len_to_char(new_var, '=') == -1)
		{
			return ;
		}
		while (ft_strncmp(info->env[i], new_var, ft_len_to_char(new_var, '=')))
		{
			i++;
		}
		free (info->env[i]);
		info->env[i] = ft_strdup(new_var);
		sort_export(info);
		return ;
	}
	env_len = ft_strstrlen(info->env, NULL, 0);
	temp = malloc(sizeof(char **) * env_len + 1);
	while (i < env_len)
	{
		temp[i] = ft_strdup(info->env[i]);
	//	printf("temp: %s\nenv: %s\ni: %i\n", temp[i], info->env[i], i);
		free(info->env[i]);
		i++;
	}
	temp[i] = NULL;
	free (info->env);
	//while (temp[t])
	//{
	//	printf("%s\n", temp[t]);
	//	t++;
	//}
	copy_to_env(info, temp, new_var);
	//t = 0;
	//while (info->env[t])
	//{
	//	printf("%s\n", info->env[t]);
	//	t++;
	//}
	sort_export(info);
}

#include "../inc/minishell.h"

char	*get_val(t_info *info, char *var)
{
	int	i;
	int	len[2];

	if (var == NULL)
		return (NULL);
	i = 0;
	printf("test28\n");
	len[0] = ft_len_to_char(var, '=');
	printf("test29\n");
	if (len[0] < 0)
	{
		printf("test30\n");
		len[0] = ft_strlen(var);
	}
	printf("test31\n");
	while (info->env[i])
	{
		printf("test32\n");
		len[1] = ft_len_to_char(info->env[i], '=');
		printf("test33\n");
		if (len[1] == -1)
		{
			printf("test34\n");
			len[1] = ft_strlen(info->env[i]);
		}
		printf("test35\n");
		if (!info->env[i] || (!ft_strncmp(info->env[i], var, len[0]) && len[0] == len[1]))
		{
			printf("test36\n");
			break ;
		}
		printf("test37\n");
		i++;
	}
	printf("test38\n");
	if (!info->env[i])
	{
		printf("test39\n");
		return (NULL);
	}
	printf("test40\n");
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

	i = 0;
	info->env = malloc(sizeof(char **) * (ft_strstrlen(temp, NULL, 0) + 1));
	while (i < ft_strstrlen(temp, NULL, 0))
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
	int t = 0;

	i = 0;
	printf("test20\n");
	if (get_val(info, new_var))
	{
		printf("test21\n");
		if (ft_len_to_char(new_var, '=') == -1)
		{
			printf("test22\n");
			return ;
		}
		while (ft_strncmp(info->env[i], new_var, ft_len_to_char(new_var, '=')))
		{
			printf("test23\n");
			i++;
		}
		printf("test24\n");
		free (info->env[i]);
		printf("test25\n");
		info->env[i] = ft_strdup(new_var);
		printf("test26\n");
		sort_export(info);
		printf("test27\n");
		return ;
	}
	printf("test40env:\n");
	while (info->env[t])
	{
		printf("%s\n", info->env[t]);
		t++;
	}
	temp = malloc(sizeof(char **) * (ft_strstrlen(info->env, NULL, 0) + 1));
	while (i < ft_strstrlen(info->env, NULL, 0))
	{
		printf("test41\n");
		temp[i] = ft_strdup(info->env[i]);
		free(info->env[i]);
		i++;
	}
	t = 0;
	printf("test42env:\n");
	while (info->env[t])
	{
		printf("%s\n", info->env[t]);
		t++;
	}
	temp[i] = NULL;
	free (info->env);
	t = 0;
	printf("test43env:\n");
	while (info->env[t])
	{
		printf("%s\n", info->env[t]);
		t++;
	}
	copy_to_env(info, temp, new_var);
	t = 0;
	printf("test44env:\n");
	while (info->env[t])
	{
		printf("%s\n", info->env[t]);
		t++;
	}
	sort_export(info);
	printf("test45\n");
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_set_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:54:35 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/10 09:40:08 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_env	*get_env_if_exist(t_env *copied_env, const char *new_name)
{
	t_env	*env;

	env = copied_env;
	while (env)
	{
		if (ft_strcmp(env->name, new_name) == 0)
		{
			return (env);
		}
		env = env->next;
	}
	return (NULL);
}

int	update_env_value(t_env *env, const char *new_value, bool append)
{
	char	*tmp_value;

	if (append && env->value)
		tmp_value = ft_strjoin(env->value, new_value);
	else
		tmp_value = ft_strdup(new_value);
	if (!tmp_value)
		return (EXIT_FAILURE);
	free(env->value);
	env->value = tmp_value;
	return (EXIT_SUCCESS);
}

int	set_env_value(t_env *copied_env, const char *new_name,
		const char *new_value, bool append)
{
	t_env	*new_env;

	new_env = get_env_if_exist(copied_env, new_name);
	if (new_env)
	{
		return (update_env_value(new_env, new_value, append));
	}
	else
	{
		new_env = create_new_env_util(new_name, new_value, NULL);
		if (!new_env)
			return (EXIT_FAILURE);
		env_add_back_util(&copied_env, new_env);
		return (EXIT_SUCCESS);
	}
}

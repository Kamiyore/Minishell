/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:44:32 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/02 22:08:33 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	remove_target_env(t_env **copied_env, t_env *prev_env,
		t_env *target_env)
{
	if (prev_env)
		prev_env->next = target_env->next;
	else
		*copied_env = target_env->next;
	if (target_env->name)
		free(target_env->name);
	if (target_env->value)
		free(target_env->value);
	if (target_env)
		free(target_env);
}

int	ft_unset(t_minishell *shell)
{
	size_t	i;
	char	*var_name;
	t_env	*prev_env;
	t_env	*tmp_env;

	i = 1;
	while (shell->commands->args[i])
	{
		var_name = shell->commands->args[i];
		prev_env = NULL;
		tmp_env = shell->env;
		while (tmp_env)
		{
			if (ft_strcmp(tmp_env->name, var_name) == 0)
			{
				remove_target_env(&shell->env, prev_env, tmp_env);
				break ;
			}
			prev_env = tmp_env;
			tmp_env = tmp_env->next;
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

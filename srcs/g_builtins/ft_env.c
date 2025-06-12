/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:42:21 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/30 20:38:41 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_env(t_minishell *shell)
{
	t_env	*tmp_env;

	tmp_env = shell->env;
	while (tmp_env)
	{
		if (tmp_env->value)
		{
			if (printf("%s=%s\n", tmp_env->name, tmp_env->value) < 0)
				return (EXIT_FAILURE);
		}
		tmp_env = tmp_env->next;
	}
	return (EXIT_SUCCESS);
}

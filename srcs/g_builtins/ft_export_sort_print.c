/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_sort_print.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 14:30:45 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/03 14:30:52 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	bubble_sort_env(t_env **sorted_env, size_t count)
{
	t_env	*temp;
	size_t	i;
	size_t	j;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - 1 - i)
		{
			if (ft_strcmp(sorted_env[j]->name, sorted_env[j + 1]->name) > 0)
			{
				temp = sorted_env[j];
				sorted_env[j] = sorted_env[j + 1];
				sorted_env[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

static void	copy_env_pointer(t_env **des_env, t_env *src_env)
{
	size_t	i;

	i = 0;
	while (src_env)
	{
		des_env[i] = src_env;
		i++;
		src_env = src_env->next;
	}
}

static void	print_sorted_env(t_env **sorted_env, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		if (sorted_env[i]->value != NULL)
			printf("declare -x %s=\"%s\"\n", sorted_env[i]->name,
				sorted_env[i]->value);
		else
			printf("declare -x %s\n", sorted_env[i]->name);
		i++;
	}
}

int	sort_and_print_env(t_env **copied_env)
{
	size_t	count;
	t_env	**sorted_env;

	count = count_env_util(*copied_env);
	sorted_env = (t_env **)malloc(sizeof(t_env *) * count);
	if (!sorted_env)
	{
		return (EXIT_FAILURE);
	}
	copy_env_pointer(sorted_env, *copied_env);
	bubble_sort_env(sorted_env, count);
	print_sorted_env(sorted_env, count);
	free(sorted_env);
	return (EXIT_SUCCESS);
}

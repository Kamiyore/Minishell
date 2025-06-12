/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:09:19 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/11 19:45:02 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**expander(t_minishell *shell, char *arg)
{
	char	*expanded_str;
	char	**result;

	expanded_str = get_expanded_str(shell, arg);
	if (!expanded_str)
		return (NULL);
	result = word_splitting(shell, expanded_str);
	free(expanded_str);
	if (!result)
		return (NULL);
	if (!quote_removal_args(shell, result))
	{
		ft_array_free(result);
		return (NULL);
	}
	return (result);
}

char	**create_expanded_args(t_minishell *shell, t_command *cmd)
{
	size_t	i;
	char	**result;
	char	**expanded;
	char	**joined_array;

	result = NULL;
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		expanded = expander(shell, cmd->args[i]);
		if (!expanded)
			return (NULL);
		joined_array = ft_array_join(result, expanded);
		ft_array_free(expanded);
		ft_array_free(result);
		result = joined_array;
		if (!result)
			return (NULL);
		i++;
	}
	return (result);
}

bool	expand_all_cmd_args(t_minishell *shell)
{
	t_command	*current;
	char		**new_args;

	current = shell->commands;
	while (current)
	{
		if (current->args && current->args[0])
		{
			new_args = create_expanded_args(shell, current);
			if (!new_args)
				return (false);
			ft_array_free(current->args);
			current->args = new_args;
		}
		current = current->next;
	}
	return (true);
}

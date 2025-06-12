/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_and_quotes_removal.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 16:16:01 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/11 20:05:09 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	quote_removal_args(t_minishell *shell, char **args)
{
	char	*tmp;
	size_t	i;

	i = 0;
	while (args && args[i])
	{
		tmp = remove_quotes_util(args[i]);
		if (tmp)
		{
			free(args[i]);
			args[i] = tmp;
		}
		else
			return (set_exit_failure_util(shell), false);
		i++;
	}
	return (true);
}

char	**word_splitting(t_minishell *shell, char *str)
{
	char	**splited_args;

	splited_args = split_quoted_words_util(str, DELIMITERS);
	if (!splited_args)
		return (set_exit_failure_util(shell), NULL);
	return (splited_args);
}

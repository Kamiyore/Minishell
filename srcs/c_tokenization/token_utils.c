/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 14:28:42 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/30 14:29:24 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*free_token_and_fail(t_minishell *shell, t_token *token)
{
	if (token)
		free(token);
	shell->exit_status = EXIT_FAILURE;
	return (NULL);
}

t_token	*create_token(t_minishell *shell, t_token_type type, const char *value)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (free_token_and_fail(shell, token), NULL);
	if (value)
	{
		token->value = ft_strdup(value);
		if (!token->value)
			return (free_token_and_fail(shell, token), NULL);
	}
	else
		token->value = NULL;
	token->type = type;
	token->next = NULL;
	return (token);
}

bool	add_token(t_token **head, t_token *new_node)
{
	t_token	*temp;

	if (!new_node)
		return (EXIT_FAILURE);
	if (!*head)
	{
		*head = new_node;
	}
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new_node;
	}
	return (EXIT_SUCCESS);
}

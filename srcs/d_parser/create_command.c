/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:07:23 by oyuhi             #+#    #+#             */
/*   Updated: 2025/04/11 20:05:09 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_command	*create_cmd_node(t_minishell *shell)
{
	t_command	*new_cmd;

	new_cmd = (t_command *)malloc(sizeof(t_command));
	if (!new_cmd)
		return (set_exit_failure_util(shell), NULL);
	new_cmd->args = NULL;
	new_cmd->infiles = NULL;
	new_cmd->infile_count = 0;
	new_cmd->outfiles = NULL;
	new_cmd->outfile_count = 0;
	new_cmd->next = NULL;
	return (new_cmd);
}

bool	add_token_content_to_cmd(t_minishell *shell, t_token *tokens,
		t_command *cmd)
{
	if (tokens->type == TOKEN_WORD)
	{
		if (!add_argument(shell, cmd, tokens->value))
			return (false);
	}
	else if (is_redirection_type(tokens->type))
	{
		if (!set_redirection(shell, cmd, tokens))
			return (false);
	}
	return (true);
}

t_token	*move_to_next_token(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	if (is_redirection_type(tokens->type))
	{
		tokens = tokens->next;
		if (tokens)
			tokens = tokens->next;
		return (tokens);
	}
	return (tokens->next);
}

t_command	*convert_token_into_cmd(t_minishell *shell, t_token *tokens)
{
	t_command	*head_cmd;
	t_command	*current_cmd;

	head_cmd = create_cmd_node(shell);
	if (!head_cmd)
		return (NULL);
	current_cmd = head_cmd;
	while (tokens && tokens->type != TOKEN_EOF)
	{
		if (tokens->type == TOKEN_PIPE)
		{
			current_cmd->next = create_cmd_node(shell);
			if (!current_cmd->next)
				return (free_all_cmds(head_cmd), NULL);
			current_cmd = current_cmd->next;
		}
		else
		{
			if (!add_token_content_to_cmd(shell, tokens, current_cmd))
				return (free_all_cmds(head_cmd), NULL);
		}
		tokens = move_to_next_token(tokens);
	}
	return (head_cmd);
}

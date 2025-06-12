/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 17:42:43 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/11 19:17:51 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	is_redirection_syntax_error(t_minishell *shell, t_token *current_token)
{
	t_token			*next_token;
	t_token_type	current_type;

	current_type = current_token->type;
	next_token = current_token->next;
	if (is_redirection_type(current_type))
	{
		if (next_token == NULL || next_token->type != TOKEN_WORD)
		{
			shell->exit_status = 2;
			if (next_token && next_token->value)
				ft_fprintf(STDERR_FILENO,
					"MINISHELL: syntax error near unexpected token `%s'\n",
					next_token->value);
			ft_fprintf(STDERR_FILENO,
				"MINISHELL: syntax error near unexpected token `newline'\n");
			return (true);
		}
	}
	return (false);
}

bool	is_pipe_syntax_error(t_minishell *shell, t_token *current_token,
		t_token *prev_token)
{
	if (current_token->type == TOKEN_PIPE)
	{
		if (prev_token == NULL || current_token->next->type == TOKEN_PIPE)
		{
			shell->exit_status = 2;
			ft_fprintf(STDERR_FILENO,
				"MINISHELL: syntax error near unexpected token `|'\n");
			return (true);
		}
		if (current_token->next == NULL
			|| current_token->next->type == TOKEN_EOF)
		{
			shell->exit_status = 2;
			ft_fprintf(STDERR_FILENO,
				"MINISHELL: syntax error near unexpected token end of file\n");
			return (true);
		}
	}
	return (false);
}

bool	detect_unclosed_quotes(t_token *tokens, char *unclosed_quote)
{
	char	quote;
	int		i;

	quote = 0;
	while (tokens && tokens->type != TOKEN_EOF)
	{
		i = 0;
		while (tokens->value && tokens->value[i])
		{
			if ((tokens->value[i] == '\'' || tokens->value[i] == '"')
				&& quote == 0)
				quote = tokens->value[i];
			else if (tokens->value[i] == quote)
				quote = 0;
			i++;
		}
		tokens = tokens->next;
	}
	if (quote != 0)
	{
		*unclosed_quote = quote;
		return (true);
	}
	return (false);
}

bool	is_quotes_syntax_error(t_minishell *shell, t_token *tokens)
{
	char	unclosed_quote;

	if (detect_unclosed_quotes(tokens, &unclosed_quote))
	{
		ft_fprintf(STDERR_FILENO,
			"MINISHELL:  unexpected EOF while looking for matching `%c'\n",
			unclosed_quote);
		ft_fprintf(STDERR_FILENO,
			"MINISHELL: syntax error: unexpected end of file\n");
		shell->exit_status = 2;
		return (true);
	}
	return (false);
}

bool	is_syntax_error(t_minishell *shell, t_token *tokens)
{
	t_token	*prev_token;
	t_token	*current_token;

	prev_token = NULL;
	current_token = tokens;
	if (is_quotes_syntax_error(shell, tokens))
		return (true);
	while (current_token->value && current_token->type != TOKEN_EOF)
	{
		if (is_pipe_syntax_error(shell, current_token, prev_token))
			return (true);
		if (is_redirection_syntax_error(shell, current_token))
			return (true);
		prev_token = current_token;
		current_token = current_token->next;
	}
	return (false);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 14:32:39 by oyuhi             #+#    #+#             */
/*   Updated: 2025/04/01 17:46:19 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	add_redirection_token(t_minishell *shell, size_t *i, t_token **tokens)
{
	char	*input;

	input = shell->input;
	if (input[*i] == '<')
	{
		(*i)++;
		if (input[*i] == '<')
		{
			(*i)++;
			return (add_token(tokens, create_token(shell, TOKEN_HEREDOC,
						"<<")));
		}
		return (add_token(tokens, create_token(shell, TOKEN_REDIR_IN, "<")));
	}
	else if (input[*i] == '>')
	{
		(*i)++;
		if (input[*i] == '>')
		{
			(*i)++;
			return (add_token(tokens, create_token(shell, TOKEN_APPEND, ">>")));
		}
		return (add_token(tokens, create_token(shell, TOKEN_REDIR_OUT, ">")));
	}
	return (EXIT_FAILURE);
}

bool	add_pipe_token(t_minishell *shell, size_t *i, t_token **tokens)
{
	(*i)++;
	return (add_token(tokens, create_token(shell, TOKEN_PIPE, "|")));
}

bool	add_eof_token(t_minishell *shell, t_token **tokens)
{
	return (add_token(tokens, create_token(shell, TOKEN_EOF, NULL)));
}

void	skip_whitespace(const char *input, size_t *i)
{
	while (input[*i] && isspace(input[*i]))
		(*i)++;
}

t_token	*tokenizer(t_minishell *shell)
{
	size_t	i;
	t_token	*tokens;
	char	*input;
	bool	malloc_fail;

	malloc_fail = false;
	input = shell->input;
	i = 0;
	tokens = NULL;
	while (input[i] && !malloc_fail)
	{
		skip_whitespace(input, &i);
		if (input[i] && input[i] == '|')
			malloc_fail = add_pipe_token(shell, &i, &tokens);
		else if (input[i] && (input[i] == '<' || input[i] == '>'))
			malloc_fail = add_redirection_token(shell, &i, &tokens);
		else if (input[i])
			malloc_fail = add_word_token(shell, &i, &tokens);
	}
	if (!malloc_fail)
		malloc_fail = add_eof_token(shell, &tokens);
	if (malloc_fail)
		return (free_tokens(tokens), NULL);
	return (tokens);
}

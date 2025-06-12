/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 16:34:11 by oyuhi             #+#    #+#             */
/*   Updated: 2025/04/11 20:05:09 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	is_in_quote(char c, char *quote)
{
	if (c == '\'' || c == '"')
	{
		if (!(*quote))
		{
			return (c);
		}
		else if (c == *quote)
		{
			return (0);
		}
	}
	return (*quote);
}

static bool	is_unquoted_delimiter_or_operator(char current_c, char quote)
{
	if (!quote && ft_strchr(DELIMITERS, current_c))
		return (true);
	else if (!quote && ft_strchr("|><", current_c))
		return (true);
	else
		return (false);
}

size_t	calculate_word_length(const char *input, size_t *i)
{
	size_t	len;
	char	quote;

	quote = 0;
	len = 0;
	while (input[*i] && !is_unquoted_delimiter_or_operator(input[*i], quote))
	{
		quote = is_in_quote(input[*i], &quote);
		len++;
		(*i)++;
	}
	return (len);
}

static char	*extract_word(t_minishell *shell, size_t *i)
{
	char	*word;
	size_t	len;
	size_t	start;
	char	*input;

	start = *i;
	input = shell->input;
	len = calculate_word_length(input, i);
	word = (char *)ft_calloc(len + 1, sizeof(char));
	if (!word)
		return (set_exit_failure_util(shell), NULL);
	ft_memcpy(word, input + start, len);
	word[len] = '\0';
	return (word);
}

bool	add_word_token(t_minishell *shell, size_t *i, t_token **tokens)
{
	char	*word;
	bool	malloc_fail;

	malloc_fail = false;
	word = extract_word(shell, i);
	if (!word)
		return (EXIT_FAILURE);
	if (*word != '\0')
	{
		malloc_fail = add_token(tokens, create_token(shell, TOKEN_WORD, word));
		free(word);
	}
	else if (*word == '\0')
		free(word);
	return (malloc_fail);
}

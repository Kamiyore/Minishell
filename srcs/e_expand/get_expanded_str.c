/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_expanded_str.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:00:40 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/11 20:05:09 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	append_to_buffer(t_minishell *shell, t_expanded_str *expanded_str,
		const char *src, size_t src_len)
{
	char	*new_buffer;
	size_t	old_size;
	size_t	new_size;

	if (expanded_str->index + src_len >= expanded_str->size)
	{
		old_size = expanded_str->size;
		new_size = expanded_str->index + src_len + 1;
		new_buffer = ft_realloc(expanded_str->buffer, old_size, new_size);
		if (!new_buffer)
			return (set_exit_failure_util(shell), false);
		expanded_str->buffer = new_buffer;
		expanded_str->size = new_size;
	}
	ft_memcpy(expanded_str->buffer + expanded_str->index, src, src_len);
	expanded_str->index += src_len;
	expanded_str->buffer[expanded_str->index] = '\0';
	return (true);
}

bool	append_one_char(t_minishell *shell, t_expanded_str *s_expanded_str,
		const char *src_input, size_t *i)
{
	if (src_input[*i] == '\'' && !s_expanded_str->in_double_quote)
	{
		s_expanded_str->in_single_quote = !s_expanded_str->in_single_quote;
	}
	else if (src_input[*i] == '"' && !s_expanded_str->in_single_quote)
	{
		s_expanded_str->in_double_quote = !s_expanded_str->in_double_quote;
	}
	if (!append_to_buffer(shell, s_expanded_str, &src_input[*i], 1))
		return (false);
	(*i)++;
	return (true);
}

bool	init_expanded_str(t_minishell *shell, t_expanded_str *expanded_str,
		const char *src_input)
{
	expanded_str->size = ft_strlen(src_input) + 1;
	expanded_str->index = 0;
	expanded_str->in_single_quote = false;
	expanded_str->in_double_quote = false;
	expanded_str->buffer = ft_calloc(sizeof(char), expanded_str->size);
	if (!expanded_str->buffer)
		return (set_exit_failure_util(shell), false);
	return (true);
}

char	*get_expanded_str(t_minishell *shell, const char *src_input)
{
	t_expanded_str	expanded_str;
	size_t			i;

	if (!src_input)
		return (NULL);
	if (!init_expanded_str(shell, &expanded_str, src_input))
		return (NULL);
	i = 0;
	while (src_input[i])
	{
		if (src_input[i] == '$' && !expanded_str.in_single_quote)
		{
			i++;
			if (!handle_dollar(shell, &expanded_str, src_input, &i))
				return (free(expanded_str.buffer), NULL);
		}
		else
		{
			if (!append_one_char(shell, &expanded_str, src_input, &i))
				return (free(expanded_str.buffer), NULL);
		}
	}
	return (expanded_str.buffer);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 16:37:29 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/11 20:05:09 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	append_exit_status(t_minishell *shell, t_expanded_str *expanded_str)
{
	char	*exit_status;

	exit_status = ft_itoa(shell->exit_status);
	if (!exit_status)
		return (set_exit_failure_util(shell), false);
	if (!append_to_buffer(shell, expanded_str, exit_status,
			ft_strlen(exit_status)))
	{
		free(exit_status);
		return (false);
	}
	free(exit_status);
	return (true);
}

static char	*get_env_name(t_minishell *shell, const char *input)
{
	size_t	len;
	char	*name;

	if (!input)
		return (NULL);
	if (ft_isalpha(input[0]) || input[0] == '_')
		len = 1;
	else
		return (NULL);
	while (ft_isalnum(input[len]) || input[len] == '_')
		len++;
	name = (char *)ft_calloc((len + 1), sizeof(char));
	if (!name)
		return (set_exit_failure_util(shell), NULL);
	if (len > 0)
		ft_strncpy(name, input, len);
	name[len] = '\0';
	return (name);
}

bool	append_env_value(t_minishell *shell, t_expanded_str *expanded_str,
		const char *src_input, size_t *i)
{
	size_t	name_len;
	char	*name;
	char	*value;

	name = get_env_name(shell, src_input + *i);
	if (!name)
		return (false);
	name_len = ft_strlen(name);
	*i += name_len;
	if (!get_env_value_util(shell, name, &value))
	{
		free(name);
		return (false);
	}
	free(name);
	if (value)
	{
		if (!append_to_buffer(shell, expanded_str, value, ft_strlen(value)))
		{
			free(value);
			return (false);
		}
		free(value);
	}
	return (true);
}

bool	is_printable_dollar(t_expanded_str *expanded_str, const char *src_input,
		size_t i)
{
	if (src_input[i] == '\0')
		return (true);
	else if (expanded_str->in_single_quote)
		return (true);
	else if (!expanded_str->in_double_quote && src_input[i] != '\0')
		return (false);
	else if (expanded_str->in_double_quote && (ft_isalnum(src_input[i])
			|| src_input[i] == '_' || src_input[i] == '?'))
		return (false);
	else
		return (true);
}

bool	handle_dollar(t_minishell *shell, t_expanded_str *expanded_str,
		const char *src_input, size_t *i)
{
	if (is_printable_dollar(expanded_str, src_input, *i))
		return (append_to_buffer(shell, expanded_str, "$", 1));
	else if (src_input[*i] == '?')
	{
		(*i)++;
		return (append_exit_status(shell, expanded_str));
	}
	else if (ft_isalpha(src_input[*i]) || src_input[*i] == '_')
	{
		return (append_env_value(shell, expanded_str, src_input, i));
	}
	else if (src_input[*i] == '\'' || src_input[*i] == '"'
		|| ft_strchr(DELIMITERS, src_input[*i]))
		return (true);
	(*i)++;
	return (true);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_filename.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:47:32 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/11 20:05:09 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	error_ambiguos_redirect(t_minishell *shell, char **tmp_array,
		char *tmp, const char *filename)
{
	if (tmp_array)
		ft_array_free(tmp_array);
	if (tmp)
		free(tmp);
	set_exit_failure_util(shell);
	ft_fprintf(STDERR_FILENO, "MINISHELL: %s: ambiguous redirect\n", filename);
}

char	*expander_filename(t_minishell *shell, char *filename)
{
	char	*result;
	char	**tmp_array;
	char	*tmp;

	tmp = get_expanded_str(shell, filename);
	if (!tmp)
		return (set_exit_failure_util(shell), NULL);
	if (tmp[0] == '\0')
		return (error_ambiguos_redirect(shell, NULL, tmp, filename), NULL);
	tmp_array = split_quoted_words_util(tmp, DELIMITERS);
	if (!tmp_array)
		return (set_exit_failure_util(shell), free(tmp), NULL);
	else if (tmp_array[1])
		return (error_ambiguos_redirect(shell, tmp_array, tmp, filename), NULL);
	result = remove_quotes_util(tmp);
	free(tmp);
	ft_array_free(tmp_array);
	if (!result)
		return (set_exit_failure_util(shell), NULL);
	return (result);
}

char	*expand_filename(t_minishell *shell, t_redirection file)
{
	char			*result;
	char			*filename;
	t_token_type	type;

	filename = file.filename;
	type = file.type;
	if (type != TOKEN_HEREDOC && ft_strchr(filename, '$'))
	{
		result = expander_filename(shell, filename);
		if (!result)
			return (NULL);
		return (result);
	}
	else
	{
		result = ft_strdup(filename);
		if (!result)
			return (set_exit_failure_util(shell), NULL);
		return (result);
	}
}

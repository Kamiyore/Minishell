/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_command_data.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 14:40:23 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/11 20:05:09 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	is_redirection_type(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_APPEND
		|| type == TOKEN_REDIR_OUT || type == TOKEN_HEREDOC);
}

t_redirection	*add_redirection_file(t_minishell *shell, t_token *token,
		t_redirection *old_files, size_t old_count)
{
	size_t			old_size;
	size_t			new_size;
	t_redirection	*new_files;
	char			*filename;

	filename = token->next->value;
	old_size = (old_count + 1) * sizeof(t_redirection);
	new_size = (old_count + 2) * sizeof(t_redirection);
	new_files = (t_redirection *)ft_realloc(old_files, old_size, new_size);
	if (!new_files)
		return (set_exit_failure_util(shell), NULL);
	new_files[old_count].filename = ft_strdup(filename);
	if (!new_files[old_count].filename)
		return (free(new_files), set_exit_failure_util(shell), NULL);
	new_files[old_count].type = token->type;
	new_files[old_count + 1].filename = NULL;
	return (new_files);
}

bool	set_redirection(t_minishell *shell, t_command *cmd, t_token *tokens)
{
	if (tokens->type == TOKEN_REDIR_IN || tokens->type == TOKEN_HEREDOC)
	{
		cmd->infiles = add_redirection_file(shell, tokens, cmd->infiles,
				cmd->infile_count);
		if (!cmd->infiles)
			return (false);
		cmd->infile_count++;
	}
	else if (tokens->type == TOKEN_REDIR_OUT || tokens->type == TOKEN_APPEND)
	{
		cmd->outfiles = add_redirection_file(shell, tokens, cmd->outfiles,
				cmd->outfile_count);
		if (!cmd->outfiles)
			return (false);
		cmd->outfile_count++;
	}
	return (true);
}

bool	add_argument(t_minishell *shell, t_command *cmd, char *new_arg)
{
	size_t	count;
	char	**result;
	size_t	old_size;
	size_t	new_size;

	count = ft_array_count_str(cmd->args);
	old_size = (count + 1) * sizeof(char *);
	new_size = (count + 2) * sizeof(char *);
	result = (char **)ft_realloc(cmd->args, old_size, new_size);
	if (!result)
		return (set_exit_failure_util(shell), false);
	result[count] = ft_strdup(new_arg);
	if (!result[count])
		return (free(result), set_exit_failure_util(shell), false);
	result[count + 1] = NULL;
	cmd->args = result;
	return (true);
}

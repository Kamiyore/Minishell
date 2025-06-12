/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:28:50 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/12 12:18:30 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*create_tmpfile_path(t_minishell *shell)
{
	char		*tmpfile_path;
	static int	i = 0;
	char		*tmpfile_index;
	char		*result;

	tmpfile_path = "/tmp/minishell_heredoc_";
	tmpfile_index = ft_itoa(i);
	if (i == INT_MAX)
		i = 0;
	else
		i++;
	if (!tmpfile_index)
		return (set_exit_failure_util(shell), NULL);
	result = ft_strjoin(tmpfile_path, tmpfile_index);
	free(tmpfile_index);
	if (!result)
		set_exit_failure_util(shell);
	return (result);
}

// WEXITSTATUS(status)
// Extracts the exit status (the low 8 bits) of the child process
// from the encoded status value.
static bool	handle_heredoc_status(t_minishell *shell)
{
	int	exit_code;

	exit_code = WEXITSTATUS(shell->exit_status);
	if (exit_code == 130)
	{
		shell->exit_status = 130;
		return (false);
	}
	else if (exit_code != EXIT_SUCCESS)
	{
		shell->exit_status = 1;
		return (false);
	}
	return (true);
}

bool	start_heredoc_process(t_minishell *shell, t_command *cmd, size_t i)
{
	pid_t				pid;
	struct sigaction	sa_original;
	char				*eof_name;

	eof_name = cmd->infiles[i].filename;
	cmd->infiles[i].filename = create_tmpfile_path(shell);
	if (!cmd->infiles[i].filename)
		return (free(eof_name), false);
	ignore_sigint(&sa_original);
	pid = fork();
	if (pid == -1)
		return (restore_sigint(&sa_original), free(eof_name),
			set_exit_failure_util(shell), false);
	else if (pid == 0)
		child_heredoc(shell, cmd->infiles[i].filename, eof_name);
	waitpid(pid, &shell->exit_status, 0);
	restore_sigint(&sa_original);
	free(eof_name);
	return (handle_heredoc_status(shell));
}

bool	handle_heredoc(t_minishell *shell)
{
	t_command	*current_cmd;
	size_t		i;

	current_cmd = shell->commands;
	while (current_cmd)
	{
		i = 0;
		while (i < current_cmd->infile_count)
		{
			if (current_cmd->infiles[i].type == TOKEN_HEREDOC)
			{
				if (!start_heredoc_process(shell, current_cmd, i))
					return (false);
			}
			i++;
		}
		current_cmd = current_cmd->next;
	}
	return (true);
}

void	clean_heredoc_tmpfile(t_minishell *shell)
{
	size_t		i;
	t_command	*cmd;

	i = 0;
	cmd = shell->commands;
	while (cmd)
	{
		while (i < cmd->infile_count)
		{
			if (cmd->infiles[i].type == TOKEN_HEREDOC)
			{
				unlink(cmd->infiles[i].filename);
			}
			i++;
		}
		cmd = cmd->next;
	}
}

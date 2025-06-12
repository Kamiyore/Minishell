/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_setup.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:11:54 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/11 14:49:39 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	cleanup_and_exit_child(t_minishell *shell, t_exec *exec_info,
		int exit_status)
{
	if (exec_info)
	{
		if (exec_info->input_fd > -1 && exec_info->input_fd != STDERR_FILENO)
			close(exec_info->input_fd);
		if (exec_info->pipe_fds[0] > -1)
			close(exec_info->pipe_fds[0]);
		if (exec_info->pipe_fds[1] > -1)
			close(exec_info->pipe_fds[1]);
	}
	if (exec_info->pid_array)
		free(exec_info->pid_array);
	free_shell(shell);
	exit(exit_status);
}

bool	setup_infile(t_minishell *shell, t_exec *exec_info,
		t_command *current_cmd)
{
	if (exec_info->input_fd != STDIN_FILENO)
	{
		if (current_cmd->infile_count == 0)
		{
			if (dup2(exec_info->input_fd, STDIN_FILENO) == -1)
			{
				cleanup_and_exit_child(shell, exec_info, EXIT_FAILURE);
			}
		}
		close(exec_info->input_fd);
		exec_info->input_fd = -1;
	}
	if (!input_redirection(shell, current_cmd))
		return (false);
	return (true);
}

bool	setup_outfile(t_minishell *shell, t_exec *exec_info,
		t_command *current_cmd)
{
	if (current_cmd->next)
	{
		if (current_cmd->outfile_count == 0)
		{
			if (dup2(exec_info->pipe_fds[1], STDOUT_FILENO) == -1)
			{
				cleanup_and_exit_child(shell, exec_info, EXIT_FAILURE);
			}
		}
		close(exec_info->pipe_fds[1]);
		exec_info->pipe_fds[1] = -1;
	}
	if (!output_redirection(shell, current_cmd))
		return (false);
	return (true);
}

void	close_unsed_fds(t_minishell *shell, t_exec *exec_info,
		t_command *current_cmd)
{
	close(shell->original_stdin);
	shell->original_stdin = -1;
	close(shell->original_stdout);
	shell->original_stdout = -1;
	if (current_cmd->next && exec_info->pipe_fds[0] > -1)
	{
		close(exec_info->pipe_fds[0]);
		exec_info->pipe_fds[0] = -1;
	}
}

void	execute_child_process(t_minishell *shell, t_exec *exec_info,
		t_command *current_cmd)
{
	int	exit_status;

	exit_status = 0;
	setup_signals_child();
	close_unsed_fds(shell, exec_info, current_cmd);
	if (!setup_infile(shell, exec_info, current_cmd) || !setup_outfile(shell,
			exec_info, current_cmd))
		cleanup_and_exit_child(shell, exec_info, EXIT_FAILURE);
	if (!current_cmd->args || !current_cmd->args[0][0])
		cleanup_and_exit_child(shell, exec_info, EXIT_SUCCESS);
	if (current_cmd->args[0])
		exec_info->builtin_id = is_builtin(current_cmd->args[0]);
	if (exec_info->builtin_id != NOT_BUILTIN)
	{
		if (exec_info->builtin_id == FT_EXIT)
			free(exec_info->pid_array);
		exit_status = exec_info->builtins[exec_info->builtin_id](shell);
		cleanup_and_exit_child(shell, exec_info, exit_status);
	}
	else
	{
		execute_external_command(shell, exec_info, current_cmd);
	}
}

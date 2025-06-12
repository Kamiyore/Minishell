/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_commands_in_child.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:10:44 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/11 20:05:09 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	wait_for_all_children(pid_t *pids, int count, t_minishell *shell)
{
	int	j;

	j = 0;
	while (j < count)
	{
		waitpid(pids[j], &shell->exit_status, 0);
		if (WIFEXITED(shell->exit_status))
			shell->exit_status = WEXITSTATUS(shell->exit_status);
		else if (WIFSIGNALED(shell->exit_status))
		{
			printf("\n");
			shell->exit_status = 128 + WTERMSIG(shell->exit_status);
		}
		else
			shell->exit_status = 1;
		j++;
	}
}

// // WEFEXITED turns true if the code was ended by exit return
// // WEXITSTATUS holds the number exit give
// // WIFSIGNALED turns true if the code was ended by a signl
// // WTERMSIG holds the number of exit of the singal

size_t	count_commands(t_minishell *shell)
{
	t_command	*cmd;
	size_t		i;

	i = 0;
	cmd = shell->commands;
	while (cmd)
	{
		i++;
		cmd = cmd->next;
	}
	return (i);
}

bool	setup_child_process(t_minishell *shell, t_exec *exec_info,
		t_command *cmd, pid_t *pid)
{
	if (cmd->next)
	{
		if (pipe(exec_info->pipe_fds) < 0)
			return (set_exit_failure_util(shell), false);
	}
	*pid = fork();
	if (*pid < 0)
		return (set_exit_failure_util(shell), false);
	return (true);
}

static void	close_and_update_input(t_exec *exec_info, t_command *cmd)
{
	if (exec_info->input_fd != STDIN_FILENO)
		close(exec_info->input_fd);
	if (cmd->next)
	{
		close(exec_info->pipe_fds[1]);
		exec_info->input_fd = exec_info->pipe_fds[0];
	}
}

void	run_commands_in_child(t_minishell *shell, t_exec *exec_info)
{
	int					i;
	t_command			*current_cmd;
	struct sigaction	previous_int;

	i = 0;
	current_cmd = shell->commands;
	exec_info->pid_array = malloc(sizeof(pid_t) * count_commands(shell));
	if (!exec_info->pid_array)
		return (set_exit_failure_util(shell));
	while (current_cmd)
	{
		if (!setup_child_process(shell, exec_info, current_cmd,
				&exec_info->pid_array[i]))
			break ;
		if (exec_info->pid_array[i] == 0)
			execute_child_process(shell, exec_info, current_cmd);
		close_and_update_input(exec_info, current_cmd);
		current_cmd = current_cmd->next;
		i++;
	}
	sigaction(SIGINT, NULL, &previous_int);
	signal(SIGINT, SIG_IGN);
	wait_for_all_children(exec_info->pid_array, i, shell);
	free(exec_info->pid_array);
	sigaction(SIGINT, &previous_int, NULL);
}

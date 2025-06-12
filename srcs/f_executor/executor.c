/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:16:13 by oyuhi             #+#    #+#             */
/*   Updated: 2025/04/11 20:05:09 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	run_single_builtin_in_parent(t_minishell *shell, t_exec *exec_info)
{
	if (shell->commands->infile_count > 0)
	{
		if (!input_redirection(shell, shell->commands))
			return (set_exit_failure_util(shell));
	}
	if (shell->commands->outfile_count > 0)
	{
		if (!output_redirection(shell, shell->commands))
			return (set_exit_failure_util(shell));
	}
	if (exec_info->builtin_id != NOT_BUILTIN)
		shell->exit_status = exec_info->builtins[exec_info->builtin_id](shell);
	return ;
}

t_builtin_id	is_builtin(char *cmd_str)
{
	if (ft_strcmp(cmd_str, "echo") == 0)
		return (FT_ECHO);
	else if (ft_strcmp(cmd_str, "cd") == 0)
		return (FT_CD);
	else if (ft_strcmp(cmd_str, "pwd") == 0)
		return (FT_PWD);
	else if (ft_strcmp(cmd_str, "export") == 0)
		return (FT_EXPORT);
	else if (ft_strcmp(cmd_str, "unset") == 0)
		return (FT_UNSET);
	else if (ft_strcmp(cmd_str, "env") == 0)
		return (FT_ENV);
	else if (ft_strcmp(cmd_str, "exit") == 0)
		return (FT_EXIT);
	else
		return (NOT_BUILTIN);
}

static void	init_exec_info(t_exec *exec_info)
{
	exec_info->input_fd = STDIN_FILENO;
	exec_info->pipe_fds[0] = -1;
	exec_info->pipe_fds[1] = -1;
	exec_info->builtin_id = NOT_BUILTIN;
	exec_info->builtins[FT_ECHO] = ft_echo;
	exec_info->builtins[FT_CD] = ft_cd;
	exec_info->builtins[FT_PWD] = ft_pwd;
	exec_info->builtins[FT_EXPORT] = ft_export;
	exec_info->builtins[FT_UNSET] = ft_unset;
	exec_info->builtins[FT_ENV] = ft_env;
	exec_info->builtins[FT_EXIT] = ft_exit;
	exec_info->pid_array = NULL;
}

bool	is_executed_in_parent(t_minishell *shell, t_exec *exec_info)
{
	if (!shell->commands->next)
	{
		if (!shell->commands->args || !shell->commands->args[0]
			|| !shell->commands->args[0][0])
			return (true);
		else
			exec_info->builtin_id = is_builtin(shell->commands->args[0]);
		if (exec_info->builtin_id != NOT_BUILTIN)
			return (true);
	}
	return (false);
}

void	cmd_executor(t_minishell *shell)
{
	t_exec	exec_info;

	init_exec_info(&exec_info);
	if (!handle_heredoc(shell))
		return (clean_heredoc_tmpfile(shell));
	if (is_executed_in_parent(shell, &exec_info))
		run_single_builtin_in_parent(shell, &exec_info);
	else
		run_commands_in_child(shell, &exec_info);
	clean_heredoc_tmpfile(shell);
	return ;
}

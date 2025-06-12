/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:33:48 by oyuhi             #+#    #+#             */
/*   Updated: 2025/04/11 19:44:06 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	get_exit_status(int err)
{
	if (err == ENOENT)
		return (127);
	else if (err == EACCES)
		return (126);
	else
		return (1);
}

// Fix: Use a proper function parameter

void	reset_shell_for_next_input(t_minishell *shell, bool interactive_mode)
{
	errno = 0;
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
	if (shell->commands)
	{
		free_all_cmds(shell->commands);
		shell->commands = NULL;
	}
	if (interactive_mode)
		dup2(shell->original_stdin, STDIN_FILENO);
	dup2(shell->original_stdout, STDOUT_FILENO);
}

// print_tokens(tokens);
// print_cmds(shell->commands);

void	build_cmds_struct(t_minishell *shell)
{
	t_token	*tokens;

	tokens = tokenizer(shell);
	if (!tokens)
		return ;
	shell->commands = parser(shell, tokens);
	free_tokens(tokens);
	if (!shell->commands)
	{
		return ;
	}
	expand_all_cmd_args(shell);
}

void	exit_ctrl_d(t_minishell *shell)
{
	free_shell(shell);
	printf("exit\n");
	exit(shell->exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	bool		interactive_mode;
	t_minishell	shell;

	init_shell_struct(&shell, envp);
	interactive_mode = decide_input_fd(&shell, argc, argv);
	setup_signals_parent();
	while (1)
	{
		if (g_signal)
			g_signal = 0;
		shell.input = get_input(&shell, interactive_mode);
		if (!shell.input)
			exit_ctrl_d(&shell);
		else
		{
			if (shell.input && shell.input[0] && shell.input[0] != '\n')
				build_cmds_struct(&shell);
			if (shell.commands)
				cmd_executor(&shell);
		}
		reset_shell_for_next_input(&shell, interactive_mode);
	}
	free_shell(&shell);
	return (shell.exit_status);
}

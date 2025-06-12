/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:45:50 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/11 12:52:34 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	decide_input_fd(t_minishell *shell, int argc, char **argv)
{
	int	fd;

	if (argc > 1)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
		{
			free_shell(shell);
			shell->exit_status = get_exit_status(errno);
			ft_fprintf(STDERR_FILENO, "MINISHELL: %s: %s\n", argv[1],
				strerror(errno));
			exit(shell->exit_status);
		}
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			free_shell(shell);
			exit(EXIT_FAILURE);
		}
		close(fd);
		return (false);
	}
	else if (!isatty(STDIN_FILENO))
		return (false);
	return (true);
}

t_env	*env_duplication(char **envp_srcs)
{
	t_env	*new_env;
	t_env	*head_of_copied_env;
	size_t	i;
	char	**splited_env;

	i = 0;
	head_of_copied_env = NULL;
	while (envp_srcs[i])
	{
		splited_env = ft_split(envp_srcs[i], "=");
		if (!splited_env || !splited_env[0])
		{
			return (free_copied_env(head_of_copied_env), NULL);
		}
		new_env = create_new_env_util(splited_env[0], splited_env[1], NULL);
		if (!new_env)
		{
			return (free_copied_env(head_of_copied_env), NULL);
		}
		env_add_back_util(&head_of_copied_env, new_env);
		ft_array_free(splited_env);
		i++;
	}
	return (head_of_copied_env);
}

void	keep_original_fds(t_minishell *shell)
{
	shell->original_stdin = dup2(STDIN_FILENO, 3);
	if (shell->original_stdin == -1)
		exit(EXIT_FAILURE);
	shell->original_stdout = dup2(STDOUT_FILENO, 4);
	if (shell->original_stdout == -1)
		exit(EXIT_FAILURE);
}

void	init_shell_struct(t_minishell *shell, char **envp)
{
	shell->input = NULL;
	shell->commands = NULL;
	shell->exit_status = 0;
	keep_original_fds(shell);
	shell->env = env_duplication(envp);
	if (!shell->env)
	{
		close(shell->original_stdin);
		close(shell->original_stdout);
		exit(EXIT_FAILURE);
	}
}

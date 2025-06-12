/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:32:43 by otaniyuhi         #+#    #+#             */
/*   Updated: 2025/04/11 20:05:09 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_prompt_str(t_minishell *shell)
{
	char	*cwd;
	char	*result;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (set_exit_failure_util(shell), NULL);
	result = ft_strjoin_three("MINISHELL:", cwd, "$> ");
	if (!result)
		shell->exit_status = EXIT_FAILURE;
	free(cwd);
	return (result);
}

char	*prompt(t_minishell *shell)
{
	char	*input;
	char	*prompt_str;

	prompt_str = get_prompt_str(shell);
	if (!prompt_str)
		return (NULL);
	input = readline(prompt_str);
	free(prompt_str);
	if (!input && g_signal)
		return (NULL);
	if (g_signal)
		shell->exit_status = 130;
	if (input && *input)
		add_history(input);
	return (input);
}

bool	contains_non_ascii(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (ft_isascii(str[i]) == 0)
		{
			free(str);
			str = NULL;
			errno = 1;
			printf("Error: Non-ASCII character detected.\n");
			return (true);
		}
		i++;
	}
	return (false);
}

char	*get_uncomment_line(void)
{
	char	*result;

	while (1)
	{
		result = ft_get_next_line(STDIN_FILENO);
		if (result && ft_strncmp(result, "#", 1) == 0)
			free(result);
		else
			return (result);
	}
}

char	*get_input(t_minishell *shell, bool interactive_mode)
{
	char	*input_line;

	if (!interactive_mode)
	{
		input_line = get_uncomment_line();
		if (!input_line || contains_non_ascii(input_line))
		{
			free_shell(shell);
			if (errno)
				exit(EXIT_FAILURE);
			exit(EXIT_SUCCESS);
		}
		return (input_line);
	}
	else
	{
		input_line = prompt(shell);
		return (input_line);
	}
}

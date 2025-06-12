/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:44:02 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/11 20:17:32 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	check_out_of_range(int neg, unsigned long long num, bool *error)
{
	unsigned long long	max_neg;

	max_neg = ((unsigned long long)(-(LONG_MIN + 1)) + 1);
	if ((neg == 1 && num > (unsigned long long)LONG_MAX) || (neg == -1
			&& num > max_neg))
	{
		*error = true;
	}
	return (*error);
}

int	atoi_long(const char *str, bool *error)
{
	unsigned long long	num;
	int					neg;
	int					i;

	num = 0;
	*error = false;
	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	neg = 1;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
	{
		neg = -1;
		i++;
	}
	while (str[i] && ft_isdigit(str[i]))
	{
		num = num * 10 + (str[i] - '0');
		if (check_out_of_range(neg, num, error))
			break ;
		i++;
	}
	return ((long long)(num * neg));
}

int	handle_exit_status(t_minishell *shell, t_command *cmd, long long exit_code,
		bool error)
{
	if (error)
	{
		ft_fprintf(STDERR_FILENO, "exit: %s: numeric argument required\n",
			cmd->args[1]);
		free_shell(shell);
		exit(2);
	}
	if (cmd->args[2])
	{
		ft_fprintf(STDERR_FILENO, "exit: too many arguments\n");
		shell->exit_status = 1;
		return (EXIT_FAILURE);
	}
	shell->exit_status = exit_code % 256;
	free_shell(shell);
	exit(shell->exit_status);
}

int	ft_exit(t_minishell *shell)
{
	t_command	*cmd;
	long long	exit_code;
	bool		error;

	cmd = shell->commands;
	printf("exit\n");
	if (cmd->args[1] && !ft_isnumber(cmd->args[1]))
	{
		ft_fprintf(STDERR_FILENO, "exit: %s: numeric argument required\n",
			cmd->args[1]);
		shell->exit_status = 2;
		free_shell(shell);
		exit(shell->exit_status);
	}
	if (cmd->args[1])
	{
		exit_code = atoi_long(cmd->args[1], &error);
		return (handle_exit_status(shell, cmd, exit_code, error));
	}
	free_shell(shell);
	exit(shell->exit_status);
}

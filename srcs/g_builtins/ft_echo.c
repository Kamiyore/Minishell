/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:14:15 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/02 13:59:39 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	parse_n_option(t_command *cmd, int *n_option)
{
	int	i;
	int	j;

	i = 1;
	*n_option = 0;
	while (cmd->args[i])
	{
		if (cmd->args[i][0] != '-')
			break ;
		j = 1;
		if (!cmd->args[i][j])
			break ;
		while (cmd->args[i][j] && cmd->args[i][j] == 'n')
			j++;
		if (j > 1 && cmd->args[i][j] == '\0')
		{
			*n_option = 1;
			i++;
		}
		else
			break ;
	}
	return (i);
}

bool	print_echo_output(t_command *cmd, int start_index, int n_option)
{
	while (cmd->args[start_index])
	{
		if (cmd->args[start_index + 1])
		{
			if (ft_fprintf(STDOUT_FILENO, "%s ", cmd->args[start_index]) == -1)
				return (false);
		}
		else
		{
			if (ft_fprintf(STDOUT_FILENO, "%s", cmd->args[start_index]) == -1)
				return (false);
		}
		start_index++;
	}
	if (!n_option)
	{
		if (ft_fprintf(STDOUT_FILENO, "\n") == -1)
			return (false);
	}
	return (true);
}

int	ft_echo(t_minishell *shell)
{
	int			n_option;
	int			start_index;
	t_command	*cmd;

	cmd = shell->commands;
	start_index = parse_n_option(cmd, &n_option);
	if (!print_echo_output(cmd, start_index, n_option))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

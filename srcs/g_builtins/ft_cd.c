/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:16:32 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/11 19:10:07 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_dest_path(t_minishell *shell)
{
	char		*path;
	t_command	*cmd;

	cmd = shell->commands;
	if (!cmd->args[1])
	{
		if (!get_env_value_util(shell, "HOME", &path))
			return (NULL);
		if (!path)
		{
			ft_fprintf(STDERR_FILENO, "MINISHELL: cd: HOME not set\n");
			return (NULL);
		}
	}
	else
	{
		path = ft_strdup(cmd->args[1]);
		if (!path)
			return (NULL);
	}
	return (path);
}

int	ft_cd(t_minishell *shell)
{
	char	*path;

	if (shell->commands->args[1] && shell->commands->args[2])
		return (ft_fprintf(STDERR_FILENO,
				"MINISHELL: cd: too many arguments\n"), EXIT_FAILURE);
	path = get_dest_path(shell);
	if (!path)
		return (EXIT_FAILURE);
	else if (!path[0])
	{
		free(path);
		return (EXIT_SUCCESS);
	}
	if (chdir(path) != 0)
	{
		ft_fprintf(STDERR_FILENO, "MINISHELL: cd: %s: %s\n", path,
			strerror(errno));
		free(path);
		return (EXIT_FAILURE);
	}
	free(path);
	return (EXIT_SUCCESS);
}

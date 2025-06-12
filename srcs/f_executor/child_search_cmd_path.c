/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_search_cmd_path.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 18:51:39 by oyuhi             #+#    #+#             */
/*   Updated: 2025/04/11 18:53:12 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**get_splited_path(t_minishell *shell, t_exec *exec_info,
		const char *cmd_str)
{
	char	*env_path;
	char	**splited_path;

	if (!get_env_value_util(shell, "PATH", &env_path))
		return (NULL);
	if (!env_path)
	{
		ft_fprintf(STDERR_FILENO, "MINISHELL: %s: No such file or directory\n",
			cmd_str);
		cleanup_and_exit_child(shell, exec_info, 127);
	}
	splited_path = ft_split(env_path, ":");
	free(env_path);
	return (splited_path);
}

// F_OK: Does the file exist?
// X_OK: Does the file have execute permissions?
// stat function is used to retrieve information about the file.
// This metadata includes details like file type, file permissions, size...
// The file's metadata is stored in the struct stat st structure.
// S_ISDIR is a macro checks if the file is a directory.
// When you pass st.st_mode to S_ISDIR, it internally checks if
// the bits corresponding to the file type indicate a directory.

static char	*build_and_check_cmd_path(t_minishell *shell, t_exec *exec_info,
		const char *dir, const char *cmd)
{
	char		*full_cmd_path;
	struct stat	st;

	full_cmd_path = ft_strjoin_three(dir, "/", cmd);
	if (!full_cmd_path)
		cleanup_and_exit_child(shell, exec_info, EXIT_FAILURE);
	if (access(full_cmd_path, F_OK | X_OK) == 0)
	{
		if (stat(full_cmd_path, &st) == 0 && S_ISDIR(st.st_mode))
		{
			free(full_cmd_path);
			return (NULL);
		}
		return (full_cmd_path);
	}
	free(full_cmd_path);
	return (NULL);
}

char	*search_command_in_path(t_minishell *shell, t_exec *exec_info,
		const char *command)
{
	char	**splited_path;
	int		i;
	char	*full_command_path;

	splited_path = get_splited_path(shell, exec_info, command);
	if (!splited_path)
		cleanup_and_exit_child(shell, exec_info, EXIT_FAILURE);
	i = 0;
	while (splited_path && splited_path[i])
	{
		full_command_path = build_and_check_cmd_path(shell, exec_info,
				splited_path[i], command);
		if (full_command_path)
		{
			ft_array_free(splited_path);
			return (full_command_path);
		}
		i++;
	}
	ft_array_free(splited_path);
	return (NULL);
}

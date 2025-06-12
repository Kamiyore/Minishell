/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc_child.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:31:11 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/12 12:13:15 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	fprintf_to_tmpfile(t_minishell *shell, char *line, int fd)
{
	char	*tmp;

	tmp = NULL;
	if (ft_strchr(line, '$'))
	{
		tmp = get_expanded_str(shell, line);
		if (!tmp)
			return (false);
		free(line);
		line = tmp;
		tmp = remove_quotes_util(line);
		if (!tmp)
			return (false);
		free(line);
		line = tmp;
	}
	if (ft_fprintf(fd, "%s\n", line) == -1)
		return (false);
	return (true);
}

bool	readline_till_eof(t_minishell *shell, const char *eof_name, int fd)
{
	char	*line;

	while (!g_signal)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, eof_name) == 0)
		{
			free(line);
			break ;
		}
		if (!fprintf_to_tmpfile(shell, line, fd))
		{
			return (free(line), false);
		}
		free(line);
	}
	return (true);
}

void	child_heredoc(t_minishell *shell, char *filename, char *eof_name)
{
	int	fd;

	setup_signals_heredoc();
	fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(eof_name);
		free_shell(shell);
		exit(EXIT_FAILURE);
	}
	if (!readline_till_eof(shell, eof_name, fd))
	{
		close(fd);
		free(eof_name);
		free_shell(shell);
		exit(EXIT_FAILURE);
	}
	close(fd);
	free(eof_name);
	free_shell(shell);
	if (g_signal)
		exit(130);
	exit(EXIT_SUCCESS);
}

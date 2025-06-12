/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:37:45 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/03 14:44:19 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	free_cmd_args(char **args)
{
	size_t	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

static void	free_cmd_infiles(t_redirection *infiles, size_t count)
{
	size_t	i;

	i = 0;
	if (!infiles)
		return ;
	while (i < count)
	{
		free(infiles[i].filename);
		i++;
	}
	free(infiles);
}

static void	free_cmd_outfiles(t_redirection *outfiles, size_t count)
{
	size_t	i;

	i = 0;
	if (!outfiles)
		return ;
	while (i < count)
	{
		free(outfiles[i].filename);
		i++;
	}
	free(outfiles);
}

void	free_all_cmds(t_command *head)
{
	t_command	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free_cmd_args(tmp->args);
		free_cmd_infiles(tmp->infiles, tmp->infile_count);
		free_cmd_outfiles(tmp->outfiles, tmp->outfile_count);
		free(tmp);
	}
}

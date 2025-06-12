/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:41:23 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/30 21:45:12 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_pwd(t_minishell *shell)
{
	char	cwd[PATH_MAX];

	(void)shell;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		if (ft_fprintf(STDOUT_FILENO, "%s\n", cwd) == -1)
			return (EXIT_FAILURE);
	}
	else
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

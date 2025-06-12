/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 15:15:42 by knemcova          #+#    #+#             */
/*   Updated: 2025/03/30 15:16:22 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_command	*parser(t_minishell *shell, t_token *tokens)
{
	if (is_syntax_error(shell, tokens))
		return (NULL);
	return (convert_token_into_cmd(shell, tokens));
}

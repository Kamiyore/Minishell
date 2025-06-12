/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_removal_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 20:06:50 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/09 20:06:52 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	strcpy_without_quotes_util(char *dst, const char *src)
{
	bool	in_single;
	bool	in_double;
	size_t	i;

	i = 0;
	in_single = false;
	in_double = false;
	while (*src)
	{
		if (*src == '\'' && !in_double)
			in_single = !in_single;
		else if (*src == '"' && !in_single)
			in_double = !in_double;
		else
		{
			dst[i] = *src;
			i++;
		}
		src++;
	}
	dst[i] = '\0';
}

char	*remove_quotes_util(const char *input)
{
	char	*result;

	result = (char *)ft_calloc(sizeof(char), ft_strlen(input) + 1);
	if (!result)
	{
		return (NULL);
	}
	strcpy_without_quotes_util(result, input);
	return (result);
}

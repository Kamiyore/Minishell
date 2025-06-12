/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quoted_words_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knemcova <knemcova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 20:07:19 by knemcova          #+#    #+#             */
/*   Updated: 2025/04/09 20:07:21 by knemcova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static size_t	ft_count_words(char const *s, const char *delimiters)
{
	size_t	words;
	bool	in_word;
	bool	in_quote;
	size_t	i;

	i = 0;
	words = 0;
	in_word = false;
	in_quote = false;
	while (s[i])
	{
		if (s[i] == '"')
			in_quote = !in_quote;
		if (!ft_strchr(delimiters, s[i]) && !in_word)
		{
			in_word = true;
			words++;
		}
		if (!in_quote && ft_strchr(delimiters, s[i]))
			in_word = false;
		i++;
	}
	return (words);
}

size_t	get_end_of_str(const char *s, size_t i, const char *delimiters,
		bool *in_quote)
{
	while (s[i])
	{
		if (s[i] == '"')
			*in_quote = !*in_quote;
		if (ft_strchr(delimiters, s[i]) && !*in_quote)
			break ;
		i++;
	}
	return (i);
}

static int	ft_insert_word(char **result, const char *s, const char *delimiters)
{
	size_t	i;
	size_t	start;
	size_t	index;
	bool	in_quote;

	i = 0;
	index = 0;
	in_quote = false;
	while (s[i])
	{
		while (s[i] && ft_strchr(delimiters, s[i]) && !in_quote)
			i++;
		if (!s[i])
			break ;
		start = i;
		i = get_end_of_str(s, i, delimiters, &in_quote);
		result[index] = ft_strndup(s + start, i - start);
		if (!result[index])
		{
			ft_array_free(result);
			return (0);
		}
		index++;
	}
	return (1);
}

char	**split_quoted_words_util(char const *s, const char *delimiters)
{
	char	**result;
	size_t	words;

	if (!s)
		return (NULL);
	words = ft_count_words(s, delimiters);
	result = (char **)ft_calloc((words + 1), sizeof(char *));
	if (!result)
		return (NULL);
	result[words] = NULL;
	if (!ft_insert_word(result, s, delimiters))
		return (NULL);
	return (result);
}

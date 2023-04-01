/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_validator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakahar <ysakahar@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 19:22:21 by ysakahar          #+#    #+#             */
/*   Updated: 2023/04/01 19:39:14 by ysakahar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	strdigit_to_int(char *str)
{
	unsigned long long int	n;
	unsigned long long int	prev_n;
	int						i;

	i = 0;
	n = 0;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		prev_n = n;
		n = n * 10 + (str[i] - '0');
		if (prev_n > n)
			return (-1);
		i++;
	}
	if (n > INT_MAX)
		return (-1);
	return ((int)n);
}

static bool	is_all_digits(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || '9' < str[i])
			return (false);
		i++;
	}
	return (true);
}

bool	is_valid_input(int argc, char **argv)
{
	int	i;
	int	n;

	i = 1;
	while (i < argc)
	{
		if (!is_all_digits(argv[i]))
			return (print_error(ERR_INPUT_DIGIT, argv[i], false));
		n = strdigit_to_int(argv[i]);
		if (i == 1)
		{
			if (n <= 0 || strdigit_to_int(MAX_PHILOSOPHERS_STR) < n)
				return (print_error(ERR_PHILO_RANGE, \
						MAX_PHILOSOPHERS_STR, false));
		}
		else
		{
			if (n == -1)
				return (print_error(ERR_INPUT_DIGIT, argv[i], false));
		}
		i++;
	}
	return (true);
}

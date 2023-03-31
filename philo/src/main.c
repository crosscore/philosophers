/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakahar <ysakahar@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 06:12:08 by ysakahar          #+#    #+#             */
/*   Updated: 2023/03/28 06:12:22 by ysakahar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	end_simulation(t_table	*table)
{
	unsigned int	i;

	i = 0;
	while (i < table->number_of_philos)
	{
		pthread_join(table->philos[i]->thread, NULL);
		i++;
	}
	if (table->number_of_philos > 1)
		pthread_join(table->grim_reaper, NULL);
	if (DEBUG_MODE == true && table->must_eat_count != -1)
		print_meal_completion_status(table);
	free_table(table);
}

static bool	begin_simulation(t_table *table)
{
	unsigned int	i;

	table->start_time = get_current_time_ms() + (table->number_of_philos * 30);
	i = 0;
	while (i < table->number_of_philos)
	{
		if (pthread_create(&table->philos[i]->thread, NULL,
				&philosopher, table->philos[i]) != 0)
			return ((bool)free_error_ret_null(ERR_THREAD, NULL, table));
		i++;
	}
	if (table->number_of_philos > 1)
	{
		if (pthread_create(&table->grim_reaper, NULL,
				&grim_reaper, table) != 0)
			return ((bool)free_error_ret_null(ERR_THREAD, NULL, table));
	}
	return (true);
}

int	main(int argc, char **argv)
{
	t_table	*table;

	table = NULL;
	if (argc < 5 || 6 < argc)
		return (print_error(USAGE, NULL, EXIT_FAILURE));
	if (!is_valid_input(argc, argv))
		return (EXIT_FAILURE);
	table = create_table(argc, argv);
	if (!table)
		return (EXIT_FAILURE);
	if (!begin_simulation(table))
		return (EXIT_FAILURE);
	end_simulation(table);
	return (EXIT_SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_table.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakahar <ysakahar@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 19:22:04 by ysakahar          #+#    #+#             */
/*   Updated: 2023/04/01 19:22:06 by ysakahar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static pthread_mutex_t	*create_forks(t_table *table)
{
	pthread_mutex_t	*forks;
	unsigned int	i;

	forks = ft_calloc(table->number_of_philos, sizeof(pthread_mutex_t));
	if (!forks)
		return (free_error_ret_null(ERR_MALLOC, NULL, table));
	i = 0;
	while (i < table->number_of_philos)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			return (free_error_ret_null(ERR_MUTEX, NULL, table));
		i++;
	}
	return (forks);
}

static bool	initialize_table_mutexes(t_table *table)
{
	table->fork_mutexes = create_forks(table);
	if (!table->fork_mutexes)
		return (false);
	if (pthread_mutex_init(&table->sim_ended_mutex, NULL) != 0)
		return ((bool)free_error_ret_null(ERR_MUTEX, NULL, table));
	if (pthread_mutex_init(&table->write_mutex, NULL) != 0)
		return ((bool)free_error_ret_null(ERR_MUTEX, NULL, table));
	return (true);
}

static void	assign_forks(t_philo *philo)
{
	if (philo->id % 2 == 1)
	{
		philo->fork[0] = (philo->id + 1) % philo->table->number_of_philos;
		philo->fork[1] = philo->id;
	}
	else
	{
		philo->fork[0] = philo->id;
		philo->fork[1] = (philo->id + 1) % philo->table->number_of_philos;
	}
}

static t_philo	**create_philosophers(t_table *table)
{
	t_philo			**philos;
	unsigned int	i;

	philos = ft_calloc(table->number_of_philos, sizeof(t_philo));
	if (!philos)
		return (free_error_ret_null(ERR_MALLOC, NULL, table));
	i = 0;
	while (i < table->number_of_philos)
	{
		philos[i] = ft_calloc(1, sizeof(t_philo));
		if (!philos[i])
			return (free_error_ret_null(ERR_MALLOC, NULL, table));
		if (pthread_mutex_init(&philos[i]->meal_time_mutex, NULL) != 0)
			return (free_error_ret_null(ERR_MUTEX, NULL, table));
		philos[i]->table = table;
		philos[i]->id = i;
		philos[i]->meal_count = 0;
		assign_forks(philos[i]);
		i++;
	}
	return (philos);
}

t_table	*create_table(int argc, char **argv)
{
	t_table	*table;
	int		i;

	i = 1;
	table = ft_calloc(1, sizeof(t_table));
	if (!table)
		return (free_error_ret_null(ERR_MALLOC, NULL, NULL));
	table->number_of_philos = strdigit_to_int(argv[i++]);
	table->time_to_die = strdigit_to_int(argv[i++]);
	table->time_to_eat = strdigit_to_int(argv[i++]);
	table->time_to_sleep = strdigit_to_int(argv[i++]);
	table->must_eat_count = -1;
	if (argc == 6)
		table->must_eat_count = strdigit_to_int(argv[i]);
	table->philos = create_philosophers(table);
	if (!table->philos)
		return (NULL);
	if (!initialize_table_mutexes(table))
		return (NULL);
	table->is_sim_ended = false;
	return (table);
}

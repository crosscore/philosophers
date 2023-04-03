/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakahar <ysakahar@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 19:22:38 by ysakahar          #+#    #+#             */
/*   Updated: 2023/04/03 20:35:17 by ysakahar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*one_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_mutexes[philo->fork[0]]);
	output_status(philo, false, GOT_FORK_1);
	wait_time_for_action(philo->table, philo->table->time_to_die);
	output_status(philo, false, DIED);
	pthread_mutex_unlock(&philo->table->fork_mutexes[philo->fork[0]]);
	return (NULL);
}

static void	philo_output_cycle(t_philo *philo)
{
	got_fork_and_eat_routine(philo);
	sleep_routine(philo);
}

void	*philosopher(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->table->must_eat_count == 0)
		return (NULL);
	pthread_mutex_lock(&philo->meal_time_mutex);
	philo->last_meal_time = philo->table->start_time;
	pthread_mutex_unlock(&philo->meal_time_mutex);
	wait_until_start_time(philo->table->start_time);
	if (philo->table->time_to_die == 0)
		return (NULL);
	if (philo->table->number_of_philos == 1)
		return (one_philo_routine(philo));
	if (philo->id % 2)
		think_routine(philo);
	while (!is_simulation_ended(philo->table))
	{
		philo_output_cycle(philo);
		think_routine(philo);
	}
	return (NULL);
}

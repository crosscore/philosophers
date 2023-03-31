/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakahar <ysakahar@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 10:51:27 by ysakahar          #+#    #+#             */
/*   Updated: 2023/03/31 10:51:29 by ysakahar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	think_routine(t_philo *philo, bool silent)
{
	if (silent == false)
		output_status(philo, false, THINKING);
	while (is_simulation_stopped(philo->table) == false)
	{
		pthread_mutex_lock(&philo->table->fork_mutexes[philo->fork[0]]);
		pthread_mutex_lock(&philo->table->fork_mutexes[philo->fork[1]]);
		if (is_simulation_stopped(philo->table) == false)
		{
			pthread_mutex_unlock(&philo->table->fork_mutexes[philo->fork[1]]);
			pthread_mutex_unlock(&philo->table->fork_mutexes[philo->fork[0]]);
			break ;
		}
		else
		{
			pthread_mutex_unlock(&philo->table->fork_mutexes[philo->fork[1]]);
			pthread_mutex_unlock(&philo->table->fork_mutexes[philo->fork[0]]);
		}
	}
}

static void	eat_sleep_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_mutexes[philo->fork[0]]);
	output_status(philo, false, GOT_FORK_1);
	pthread_mutex_lock(&philo->table->fork_mutexes[philo->fork[1]]);
	output_status(philo, false, GOT_FORK_2);
	output_status(philo, false, EATING);
	pthread_mutex_lock(&philo->meal_time_mutex);
	philo->last_meal_time = get_current_time_ms();
	pthread_mutex_unlock(&philo->meal_time_mutex);
	wait_philosopher(philo->table, philo->table->time_to_eat, 1);
	if (is_simulation_stopped(philo->table) == false)
	{
		pthread_mutex_lock(&philo->meal_time_mutex);
		philo->meal_count += 1;
		pthread_mutex_unlock(&philo->meal_time_mutex);
	}
	pthread_mutex_unlock(&philo->table->fork_mutexes[philo->fork[1]]);
	pthread_mutex_unlock(&philo->table->fork_mutexes[philo->fork[0]]);
	output_status(philo, false, SLEEPING);
	wait_philosopher(philo->table, philo->table->time_to_sleep, 1);
}

static void	*lone_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_mutexes[philo->fork[0]]);
	output_status(philo, false, GOT_FORK_1);
	wait_philosopher(philo->table, philo->table->time_to_die, 1);
	output_status(philo, false, DIED);
	pthread_mutex_unlock(&philo->table->fork_mutexes[philo->fork[0]]);
	return (NULL);
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
		return (lone_philo_routine(philo));
	else if (philo->id % 2 == 1)
		think_routine(philo, true);
	while (is_simulation_stopped(philo->table) == false)
	{
		eat_sleep_routine(philo);
		think_routine(philo, false);
	}
	return (NULL);
}

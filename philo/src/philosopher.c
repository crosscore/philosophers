/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakahar <ysakahar@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 19:22:38 by ysakahar          #+#    #+#             */
/*   Updated: 2023/04/01 20:06:55 by ysakahar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	got_fork_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_mutexes[philo->fork[0]]);
	output_status(philo, false, GOT_FORK_1);
	pthread_mutex_lock(&philo->table->fork_mutexes[philo->fork[1]]);
	output_status(philo, false, GOT_FORK_2);
}

void	eat_routine(t_philo *philo)
{
	output_status(philo, false, EATING);
	pthread_mutex_lock(&philo->meal_time_mutex);
	philo->last_meal_time = get_current_time_ms();
	pthread_mutex_unlock(&philo->meal_time_mutex);
	wait_time_for_action(philo->table, philo->table->time_to_eat);
	if (is_simulation_stopped(philo->table) == false)
	{
		pthread_mutex_lock(&philo->meal_time_mutex);
		philo->meal_count += 1;
		pthread_mutex_unlock(&philo->meal_time_mutex);
	}
	pthread_mutex_unlock(&philo->table->fork_mutexes[philo->fork[1]]);
	pthread_mutex_unlock(&philo->table->fork_mutexes[philo->fork[0]]);
}

void	sleep_routine(t_philo *philo)
{
	output_status(philo, false, SLEEPING);
	wait_time_for_action(philo->table, philo->table->time_to_sleep);
}

static void	got_fork_eat_sleep_routine(t_philo *philo)
{
	got_fork_routine(philo);
	eat_routine(philo);
	sleep_routine(philo);
}

static void	think_routine(t_philo *philo, bool silent)
{
	time_t	time_to_think;

	pthread_mutex_lock(&philo->meal_time_mutex);
	time_to_think = (philo->table->time_to_die
			- (get_current_time_ms() - philo->last_meal_time)
			- philo->table->time_to_eat) / 2;
	pthread_mutex_unlock(&philo->meal_time_mutex);
	if (time_to_think > 240)
		time_to_think = 240;
	else if (silent == true && time_to_think == 0)
		time_to_think = 1;
	else if (time_to_think < 0)
		time_to_think = 0;
	if (silent == false)
		output_status(philo, false, THINKING);
	wait_time_for_action(philo->table, time_to_think);
}

static void	*lone_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_mutexes[philo->fork[0]]);
	output_status(philo, false, GOT_FORK_1);
	wait_time_for_action(philo->table, philo->table->time_to_die);
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
		got_fork_eat_sleep_routine(philo);
		think_routine(philo, false);
	}
	return (NULL);
}

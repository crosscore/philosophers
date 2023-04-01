/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grim_reaper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakahar <ysakahar@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 19:22:15 by ysakahar          #+#    #+#             */
/*   Updated: 2023/04/01 19:22:18 by ysakahar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	set_simulation_stop_flag(t_table *table, bool stop_flag)
{
	pthread_mutex_lock(&table->sim_ended_mutex);
	table->is_sim_ended = stop_flag;
	pthread_mutex_unlock(&table->sim_ended_mutex);
}

bool	is_simulation_stopped(t_table *table)
{
	bool	stopped;

	stopped = false;
	pthread_mutex_lock(&table->sim_ended_mutex);
	if (table->is_sim_ended == true)
		stopped = true;
	pthread_mutex_unlock(&table->sim_ended_mutex);
	return (stopped);
}

static bool	check_philo_death(t_philo *philo)
{
	time_t	current_time;

	current_time = get_current_time_ms();
	pthread_mutex_lock(&philo->meal_time_mutex);
	if (current_time >= (philo->table->time_to_die + philo->last_meal_time))
	{
		set_simulation_stop_flag(philo->table, true);
		output_status(philo, true, DIED);
		pthread_mutex_unlock(&philo->meal_time_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->meal_time_mutex);
	return (false);
}

static bool	is_end_condition_met(t_table *table)
{
	unsigned int	i;
	bool			all_philos_ate_enough;

	all_philos_ate_enough = true;
	i = 0;
	while (i < table->number_of_philos)
	{
		if (check_philo_death(table->philos[i]))
			return (true);
		pthread_mutex_lock(&table->philos[i]->meal_time_mutex);
		if (table->must_eat_count != -1)
			if (table->philos[i]->meal_count
				< (unsigned int)table->must_eat_count)
				all_philos_ate_enough = false;
		pthread_mutex_unlock(&table->philos[i]->meal_time_mutex);
		i++;
	}
	if (all_philos_ate_enough == true && table->must_eat_count != -1)
	{
		set_simulation_stop_flag(table, true);
		return (true);
	}
	return (false);
}

void	*grim_reaper(void *data)
{
	t_table			*table;

	table = (t_table *)data;
	if (table->must_eat_count == 0)
		return (NULL);
	set_simulation_stop_flag(table, false);
	wait_until_start_time(table->start_time);
	while (true)
	{
		if (is_end_condition_met(table) == true)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}

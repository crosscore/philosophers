/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grim_reaper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakahar <ysakahar@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 20:47:25 by ysakahar          #+#    #+#             */
/*   Updated: 2023/04/04 14:33:19 by ysakahar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	check_philo_death(t_philo *philo)
{
	time_t	current_time;
	time_t	time_to_die;

	pthread_mutex_lock(&philo->meal_time_mutex);
	current_time = get_current_time_ms();
	time_to_die = philo->last_meal_time + philo->table->time_to_die;
	pthread_mutex_unlock(&philo->meal_time_mutex);
	if (current_time >= time_to_die)
		return (true);
	return (false);
}

static int	find_dead_philosopher_index(t_table *table)
{
	unsigned int	i;

	i = 0;
	while (i < table->number_of_philos)
	{
		if (check_philo_death(table->philos[i]))
			return (i);
		i++;
	}
	return (-1);
}

static bool	have_all_philos_eaten_enough(t_table *table)
{
	unsigned int	i;

	i = 0;
	while (i < table->number_of_philos)
	{
		pthread_mutex_lock(&table->philos[i]->meal_time_mutex);
		if (table->must_eat_count != -1)
		{
			if (table->philos[i]->meal_count < \
									(unsigned int)table->must_eat_count)
			{
				pthread_mutex_unlock(&table->philos[i]->meal_time_mutex);
				return (false);
			}
		}
		pthread_mutex_unlock(&table->philos[i]->meal_time_mutex);
		i++;
	}
	return (true);
}

static void	*grim_reaper_helper(t_table *table)
{
	int		dead_philo_index;

	while (true)
	{
		dead_philo_index = find_dead_philosopher_index(table);
		if (dead_philo_index != -1)
		{
			set_simulation_ended(table);
			if (table->must_eat_count != -1 && \
					!have_all_philos_eaten_enough(table))
				output_status(table->philos[dead_philo_index], true, DIED);
			else if (table->must_eat_count == -1)
				output_status(table->philos[dead_philo_index], true, DIED);
			return (NULL);
		}
		if (table->must_eat_count != -1)
		{
			if (have_all_philos_eaten_enough(table))
			{
				set_simulation_ended(table);
				return (NULL);
			}
		}
		usleep(699);
	}
}

void	*grim_reaper(void *data)
{
	t_table	*table;

	table = (t_table *)data;
	if (table->must_eat_count == 0)
		return (NULL);
	wait_until_start_time(table->start_time);
	grim_reaper_helper(table);
	return (NULL);
}

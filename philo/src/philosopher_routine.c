/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_routine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakahar <ysakahar@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 21:32:59 by ysakahar          #+#    #+#             */
/*   Updated: 2023/04/04 06:07:37 by ysakahar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	think_routine(t_philo *philo)
{
	output_status(philo, false, THINKING);
	usleep(1000);
}

void	got_fork_and_eat_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_mutexes[philo->fork[0]]);
	output_status(philo, false, GOT_FORK_1);
	pthread_mutex_lock(&philo->table->fork_mutexes[philo->fork[1]]);
	output_status(philo, false, GOT_FORK_2);
	output_status(philo, false, EATING);
	pthread_mutex_lock(&philo->meal_time_mutex);
	philo->last_meal_time = get_current_time_ms();
	philo->meal_count += 1;
	pthread_mutex_unlock(&philo->meal_time_mutex);
	wait_time_for_action(philo->table, philo->table->time_to_eat);
	pthread_mutex_unlock(&philo->table->fork_mutexes[philo->fork[1]]);
	pthread_mutex_unlock(&philo->table->fork_mutexes[philo->fork[0]]);
}

void	sleep_routine(t_philo *philo)
{
	output_status(philo, false, SLEEPING);
	wait_time_for_action(philo->table, philo->table->time_to_sleep);
}

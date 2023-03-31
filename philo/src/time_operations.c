/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakahar <ysakahar@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 06:11:45 by ysakahar          #+#    #+#             */
/*   Updated: 2023/03/28 06:11:47 by ysakahar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

time_t	get_current_time_ms(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void	wait_philosopher(t_table *table, time_t sleep_time, useconds_t interval)
{
	time_t	wake_up_time;

	wake_up_time = get_current_time_ms() + sleep_time;
	while (get_current_time_ms() < wake_up_time)
	{
		if (is_simulation_stopped(table))
			break ;
		usleep(interval);
	}
}

void	wait_until_start_time(time_t start_time)
{
	while (get_current_time_ms() < start_time)
		continue ;
}

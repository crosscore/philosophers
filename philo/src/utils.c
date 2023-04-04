/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakahar <ysakahar@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 19:22:51 by ysakahar          #+#    #+#             */
/*   Updated: 2023/04/04 13:59:28 by ysakahar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t	total_size;
	void	*ptr;

	total_size = count * size;
	ptr = malloc(total_size);
	if (!ptr)
		return (NULL);
	memset(ptr, 0, total_size);
	return (ptr);
}

bool	is_simulation_ended(t_table *table)
{
	bool	result;

	result = false;
	pthread_mutex_lock(&table->is_sim_ended_mutex);
	if (table->is_sim_ended)
		result = true;
	pthread_mutex_unlock(&table->is_sim_ended_mutex);
	return (result);
}

void	set_simulation_ended(t_table *table)
{
	pthread_mutex_lock(&table->is_sim_ended_mutex);
	table->is_sim_ended = true;
	pthread_mutex_unlock(&table->is_sim_ended_mutex);
}

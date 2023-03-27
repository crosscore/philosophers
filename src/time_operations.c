/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakahar <ysakahar@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 15:16:32 by ysakahar          #+#    #+#             */
/*   Updated: 2023/03/26 15:16:34 by ysakahar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* get_current_time_ms:
Epoch(1970-01-01 00:00:00)からの現在の時間をミリ秒単位で取得します。
時間値を返します。*/
time_t	get_current_time_ms(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

/* wait_philosopher:
Philosopherスレッドをミリ秒単位で一定時間一時停止します。
停止時間中にシミュレーションが終了したかどうかを定期的に確認し、
終了している場合は停止することなくbreakします。*/
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

/* wait_until_start_time:
すべてのスレッドが同じ開始時刻参照で同時に開始されるように、
各スレッド実行の開始時にわずかな遅延を待ちます。
これにより、死神スレッドが哲学者スレッドと同期されます。*/
void	wait_until_start_time(time_t start_time)
{
	while (get_current_time_ms() < start_time)
		continue ;
}

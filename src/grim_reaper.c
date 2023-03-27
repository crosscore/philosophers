/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grim_reaper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakahar <ysakahar@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 21:07:21 by ysakahar          #+#    #+#             */
/*   Updated: 2023/03/27 21:18:24 by ysakahar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* set_simulation_stop_flag:
シミュレーション停止フラグをtrueまたはfalseに設定します。
このフラグを設定できるのは死神スレッドだけです。
シミュレーション停止フラグがtrueに設定されている場合は、
シミュレーションが終了条件を満たしたことを意味します。*/
static void	set_simulation_stop_flag(t_table *table, bool stop_flag)
{
	pthread_mutex_lock(&table->sim_ended_mutex);
	table->is_sim_ended = stop_flag;
	pthread_mutex_unlock(&table->sim_ended_mutex);
}

/* is_simulation_stopped:
シミュレーションが終了しているかどうかを確認します。
停止フラグはミューテックス ロックによって保護されているため、
どのスレッドでも競合することなくシミュレーションステータスをチェックできます。
シミュレーション停止フラグがtrueに設定されている場合はtrue、
フラグがfalseに設定されている場合はfalseを返します。*/
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

/* check_philo_death:
現在の時刻が、最後の食事をした時刻＋死亡までの残り時間を超えた場合、
set_simulation_stop_flag == true にします。
哲学者が死亡した場合はtrue、そうでない場合はfalseを返します。*/
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

/* is_end_condition_met:
各哲学者をチェックして、2つの終了条件が満たされているかかどうかを確認します。
哲学者が死亡した場合、またはすべての哲学者が十分に食べた場合、シミュレーションを停止します。
終了条件に達した場合はtrue、そうでない場合はfalseを返します。
meal_time_mutexはt_philo構造体内にあり、それぞれの哲学者の
最後の食事時刻(last_meal_time)と食事回数(meal_count)を保護するためのミューテックスです。
must_eat_count == -1 はeat_countが未設定の場合の初期値。*/
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

/* grim_reaper:
死神スレッドのルーチン。
哲学者の死亡条件に到達したか、あるいは、全ての哲学者が十分に食べたかどうかを確認します。
これら2つの終了条件のいずれかに到達すると、シミュレーションが停止します。
usleep(1000): 哲学者は実際に死亡してから10ms以内に死亡していることを表示する必要がある*/
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

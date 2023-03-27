/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakahar <ysakahar@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 15:16:20 by ysakahar          #+#    #+#             */
/*   Updated: 2023/03/27 17:59:34 by ysakahar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* eat_sleep_routine:
哲学者は食事をする準備ができたら、フォークミューテックスがロック解除されるのを待ってからロックします。
その後、哲学者は一定時間食事をします。
最後の食事の時間は、被験者の要求に従って、食事の終わりではなく、食事の始めに記録されます。*/
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

/* think_routine:
哲学者はひとたび眠りを終えると、再び食事を始める前に一定時間考えます。
philo->table->time_to_die -
	(get_current_time_ms() - philo->last_meal_time)) - philo->table->time_to_eat:
哲学者の次の食事までの残り時間を意味します。
哲学者が次に死亡するまでの残り時間から、食事にかかる時間（philo->table->time_to_eat）を引くことで得られます。
ある哲学者がフォークを不必要に独占して他の哲学者に損害を与えることを避けるために、
哲学者の食事のルーチンをずらすのに役立ちます。*/
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
	wait_philosopher(philo->table, time_to_think, 1);
}

/* lone_philo_routine:
このルーチンは、哲学者が1人しかいない場合に呼び出されます。
一人の哲学者はフォークを一つしか持っていないので、食べることができません。
哲学者はそのフォークを手に取り、time_to_dieだけ待ってから死ぬ。
これは、eatルーチンで2番目のforkを待ってスレッドがスタックしないようにするための別のルーチンです。*/
static void	*lone_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_mutexes[philo->fork[0]]);
	output_status(philo, false, GOT_FORK_1);
	wait_philosopher(philo->table, philo->table->time_to_die, 1);
	output_status(philo, false, DIED);
	pthread_mutex_unlock(&philo->table->fork_mutexes[philo->fork[0]]);
	return (NULL);
}

/* philosopher:
哲学者スレッドルーチン。哲学者は食べて、寝て、考えなければならない。
哲学者のスレッド間の競合を避けるために、奇数idの哲学者はthink_routine(silent == true)から始めます。
これにより、全ての哲学者が同時にフォークを取ろうとするのを避け、デットロックを回避できます。

pthread_create関数を使用してスレッドを生成する場合、
スレッドのエントリポイント関数は、次の形式でなければなりません： void *func_name(void *data);
この形式に従って、philosopher関数はvoid *dataを引数として受け取ります。
しかし、philosopher関数内では、t_philo型のデータを使用する必要があります。
そのため、void *型のdataをt_philo *型にキャストしています： philo = (t_philo *)data;

meal_time_mutexはt_philo構造体内にあり、それぞれの哲学者の
最後の食事時刻(last_meal_time)と食事回数(meal_count)を保護するためのミューテックスです。*/
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

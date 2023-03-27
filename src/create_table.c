/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_table.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakahar <ysakahar@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 21:07:13 by ysakahar          #+#    #+#             */
/*   Updated: 2023/03/27 09:29:53 by ysakahar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* intit_forks:
メモリを割り当て、forkミューテックスを初期化します。
forkミューテックス配列へのポインターを返します。エラーが発生した場合はNULLを返します。*/
static pthread_mutex_t	*create_forks(t_table *table)
{
	pthread_mutex_t	*forks;
	unsigned int	i;

	forks = malloc(sizeof(pthread_mutex_t) * table->number_of_philos);
	if (!forks)
		return (free_error_ret_null(ERR_MALLOC, NULL, table));
	i = 0;
	while (i < table->number_of_philos)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			return (free_error_ret_null(ERR_MUTEX, NULL, table));
		i++;
	}
	return (forks);
}

/* initialize_table_mutexes:
フォーク、書き込み、および停止シミュレーション フラグのミューテックスロックを初期化します。
初期化が成功した場合はtrue、失敗した場合はfalseを返します。*/
static bool	initialize_table_mutexes(t_table *table)
{
	table->fork_mutexes = create_forks(table);
	if (!table->fork_mutexes)
		return (false);
	if (pthread_mutex_init(&table->sim_ended_mutex, NULL) != 0)
		return ((bool)free_error_ret_null(ERR_MUTEX, NULL, table));
	if (pthread_mutex_init(&table->write_mutex, NULL) != 0)
		return ((bool)free_error_ret_null(ERR_MUTEX, NULL, table));
	return (true);
}

/* assign_forks:
哲学者 0 (偶数番号):
fork[0] = 0 (左側のフォーク)
fork[1] = (0 + 1) % 5 = 1 (右側のフォーク)
哲学者 1 (奇数番号):
fork[0] = (1 + 1) % 5 = 2 (左側のフォーク)
fork[1] = 1 (右側のフォーク)
哲学者 2 (偶数番号):
fork[0] = 2 (左側のフォーク)
fork[1] = (2 + 1) % 5 = 3 (右側のフォーク)
哲学者 3 (奇数番号):
fork[0] = (3 + 1) % 5 = 4 (左側のフォーク)
fork[1] = 3 (右側のフォーク)
哲学者 4 (偶数番号):
fork[0] = 4 (左側のフォーク)
fork[1] = (4 + 1) % 5 = 0 (右側のフォーク)
5人の哲学者がいる場合、各哲学者が独自のフォークを持っていると10本のフォークが必要ですが、
隣接する哲学者がフォークを共有することで、
合計で5本のフォークだけで全員が食事をすることが可能になります。*/
static void	assign_forks(t_philo *philo)
{
	if (philo->id % 2 == 1)
	{
		philo->fork[0] = (philo->id + 1) % philo->table->number_of_philos;
		philo->fork[1] = philo->id;
	}
	else
	{
		philo->fork[0] = philo->id;
		philo->fork[1] = (philo->id + 1) % philo->table->number_of_philos;
	}
}

/* create_philosophers:
各哲学者にメモリを割り当て、その値を初期化します。
pthread_mutex_initの第2引数がNULLの場合、デフォルトのmutex属性を使用することを意味する。
meal_time_mutexは、哲学者が最後に食事をした時刻（last_meal_time）へのアクセスを同期するために使用されます。
初期化が失敗した場合は、哲学者の配列へのポインターまたはNULLを返します。*/
static t_philo	**create_philosophers(t_table *table)
{
	t_philo			**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo) * table->number_of_philos);
	if (!philos)
		return (free_error_ret_null(ERR_MALLOC, NULL, table));
	i = 0;
	while (i < table->number_of_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (free_error_ret_null(ERR_MALLOC, NULL, table));
		if (pthread_mutex_init(&philos[i]->meal_time_mutex, NULL) != 0)
			return (free_error_ret_null(ERR_MUTEX, NULL, table));
		philos[i]->table = table;
		philos[i]->id = i;
		philos[i]->meal_count = 0;
		assign_forks(philos[i]);
		i++;
	}
	return (philos);
}

/* create_table:
プログラムのすべてのパラメーターを含むデータ構造である「テーブル」を初期化します。
割り当てられたテーブル構造体へのポインターを返します。
構造体のサイズはメンバーのサイズ合計となるため、終端文字は必要ありません。
初期化中にエラーが発生した場合はNULLを返します。*/
t_table	*create_table(int argc, char **argv)
{
	t_table	*table;
	int		i;

	i = 1;
	table = malloc(sizeof(t_table) * 1);
	if (!table)
		return (free_error_ret_null(ERR_MALLOC, NULL, NULL));
	table->number_of_philos = strdigit_to_int(argv[i++]);
	table->time_to_die = strdigit_to_int(argv[i++]);
	table->time_to_eat = strdigit_to_int(argv[i++]);
	table->time_to_sleep = strdigit_to_int(argv[i++]);
	table->must_eat_count = -1;
	if (argc == 6)
		table->must_eat_count = strdigit_to_int(argv[i]);
	table->philos = create_philosophers(table);
	if (!table->philos)
		return (NULL);
	if (!initialize_table_mutexes(table))
		return (NULL);
	table->is_sim_ended = false;
	return (table);
}

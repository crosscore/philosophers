/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakahar <ysakahar@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 21:06:27 by ysakahar          #+#    #+#             */
/*   Updated: 2023/03/27 23:12:36 by ysakahar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* end_simulation:
すべてのスレッドが終了するまで待機し、mutexを破棄し、割り当てられたメモリをfreeする。
pthread_join(table->philos[i]->thread, NULL); の行が実行されると、
table->philos[i]->thread で指定されたスレッドが終了するまで待機します。
この処理は、table->number_of_philos の数だけ繰り返され、すべてのスレッドが終了するまで待機します。
if (table->number_of_philos > 1) の理由: begin_simulation関数では、
table->number_of_philos > 1 の時にgrim_reaperスレッドを作成しています。*/
static void	end_simulation(t_table	*table)
{
	unsigned int	i;

	i = 0;
	while (i < table->number_of_philos)
	{
		pthread_join(table->philos[i]->thread, NULL);
		i++;
	}
	if (table->number_of_philos > 1)
		pthread_join(table->grim_reaper, NULL);
	if (DEBUG_MODE == true && table->must_eat_count != -1)
		print_meal_completion_status(table);
	free_table(table);
}

/* begin_simulation:
grim_reaperスレッドの動作：
pthread_create(&table->grim_reaper, NULL, &grim_reaper, table)
第1引数: &table->grim_reaper - 新しく作成されるスレッドの識別子（pthread_t型）へのポインタ(&)。
この識別子は、後にpthread_join関数の第1引数に渡される識別子となる。
第2引数: NULL - スレッド属性を指定するためのポインタ。NULLはデフォルトの属性が使用されることを意味する。
第3引数: &grim_reaper - 新しく作成されたスレッドで実行される関数へのポインタ。
第4引数: table - 第3引数の関数（この場合はgrim_reaper）の引数として渡されます。
pthread_createが呼び出されると、新しいスレッドが作成され、そのスレッドでgrim_reaper関数が実行されます。
grim_reaper関数は、t_table構造体へのポインタを引数として受け取り、各哲学者のスレッドの生存状態を監視します。
もし哲学者が所定の時間内に食事をしない場合、シミュレーションを終了させる役割を担います。*/
static bool	begin_simulation(t_table *table)
{
	unsigned int	i;

	table->start_time = get_current_time_ms() + (table->number_of_philos * 30);
	i = 0;
	while (i < table->number_of_philos)
	{
		if (pthread_create(&table->philos[i]->thread, NULL,
				&philosopher, table->philos[i]) != 0)
			return ((bool)free_error_ret_null(ERR_THREAD, NULL, table));
		i++;
	}
	if (table->number_of_philos > 1)
	{
		if (pthread_create(&table->grim_reaper, NULL,
				&grim_reaper, table) != 0)
			return ((bool)free_error_ret_null(ERR_THREAD, NULL, table));
	}
	return (true);
}

/* main:
コマンドライン引数を解析し、シミュレーションを開始して、
すべてのスレッドが終了するまで待機し、mutexを破棄し、割り当てられたメモリを解放する。*/
int	main(int argc, char **argv)
{
	t_table	*table;

	table = NULL;
	if (argc < 5 || 6 < argc)
		return (print_error(USAGE, NULL, EXIT_FAILURE));
	if (!is_valid_input(argc, argv))
		return (EXIT_FAILURE);
	table = create_table(argc, argv);
	if (!table)
		return (EXIT_FAILURE);
	if (!begin_simulation(table))
		return (EXIT_FAILURE);
	end_simulation(table);
	return (EXIT_SUCCESS);
}

/*
__attribute__((destructor)) static void	destructor(void)
{
	system("leaks -q philo");
}
*/
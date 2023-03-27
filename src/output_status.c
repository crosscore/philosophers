/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_status.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakahar <ysakahar@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 15:16:42 by ysakahar          #+#    #+#             */
/*   Updated: 2023/03/26 15:16:44 by ysakahar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* print_meal_completion_status:
食べる回数が指定されている場合は、シミュレーションの結果を出力します。
デバッグ目的でのみ使用されます。*/
void	print_meal_completion_status(t_table *table)
{
	unsigned int	i;
	unsigned int	full_count;

	full_count = 0;
	i = 0;
	while (i < table->number_of_philos)
	{
		if (table->philos[i]->meal_count >= (unsigned int)table->must_eat_count)
			full_count++;
		i++;
	}
	pthread_mutex_lock(&table->write_mutex);
	printf("%d/%d philosophers had at least %d meals.\n",
		full_count, table->number_of_philos, table->must_eat_count);
	pthread_mutex_unlock(&table->write_mutex);
	return ;
}

/* debug_print_status:
哲学者のステータスを読みやすくカラフルな形式で出力して、デバッグに役立てます。
フォークの取得状況については、哲学者がどのフォークを取得したかを示す追加情報が表示されます。*/
static void	debug_print_status(t_philo *philo, char *color,
								char *str, t_ph_status status)
{
	if (status == GOT_FORK_1)
		printf("[%6ld] %s%03d %s\e[0m: fork[%d]\n",
			get_current_time_ms() - philo->table->start_time,
			color, philo->id + 1, str, philo->fork[0]);
	else if (status == GOT_FORK_2)
		printf("[%6ld] %s%03d %s\e[0m: fork[%d]\n",
			get_current_time_ms() - philo->table->start_time,
			color, philo->id + 1, str, philo->fork[1]);
	else
		printf("[%6ld] %s%03d %s\e[0m\n",
			get_current_time_ms() - philo->table->start_time,
			color, philo->id + 1, str);
}

/* debug_output_status:
デバッグモードのステータス書き込みをリダイレクトします。
このオプションでは、philo.hでDEBUG_FORMATTINGオプションを1に設定する必要があります。*/
static void	debug_output_status(t_philo *philo, t_ph_status status)
{
	if (status == DIED)
		debug_print_status(philo, RED, "died", status);
	else if (status == EATING)
		debug_print_status(philo, YELLOW, "is eating", status);
	else if (status == SLEEPING)
		debug_print_status(philo, CYAN, "is sleeping", status);
	else if (status == THINKING)
		debug_print_status(philo, CYAN, "is thinking", status);
	else if (status == GOT_FORK_1)
		debug_print_status(philo, GREEN, "has taken a fork", status);
	else if (status == GOT_FORK_2)
		debug_print_status(philo, GREEN, "has taken a fork", status);
}

/* print_status:
Prints a philosopher's status in plain text as required by the project
subject:
	timestamp_in_ms X status */
static void	print_status(t_philo *philo, char *str)
{
	printf("%ld %d %s\n", get_current_time_ms() - philo->table->start_time,
		philo->id + 1, str);
}

/* output_status:
シミュレーションがまだアクティブである限り、哲学者のステータスを出力します。
書き込みミューテックスをロックして、異なるスレッドからのメッセージが絡み合わないようにします。
philo.hでDEBUG_FORMATTING=1の時、ステータスはデバッグに役立つ色と追加情報でフォーマットされます。
それ以外の場合、出力はプロジェクトの対象で必要な通常の形式になります。*/
void	output_status(t_philo *philo, bool reaper_report, t_ph_status status)
{
	pthread_mutex_lock(&philo->table->write_mutex);
	if (is_simulation_stopped(philo->table) == true && reaper_report == false)
	{
		pthread_mutex_unlock(&philo->table->write_mutex);
		return ;
	}
	if (DEBUG_MODE == true)
	{
		debug_output_status(philo, status);
		pthread_mutex_unlock(&philo->table->write_mutex);
		return ;
	}
	if (status == DIED)
		print_status(philo, "died");
	else if (status == EATING)
		print_status(philo, "is eating");
	else if (status == SLEEPING)
		print_status(philo, "is sleeping");
	else if (status == THINKING)
		print_status(philo, "is thinking");
	else if (status == GOT_FORK_1 || status == GOT_FORK_2)
		print_status(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->table->write_mutex);
}

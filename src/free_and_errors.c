/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_and_errors.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakahar <ysakahar@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 21:07:27 by ysakahar          #+#    #+#             */
/*   Updated: 2023/03/27 09:28:20 by ysakahar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* destroy_mutexts:
プログラムによって作成されたすべてのミューテックス (fork_mutexes、meal_time_mutex、
write_mutex、sim_ended_mutex) を破棄します。*/
void	destroy_mutexes(t_table *table)
{
	unsigned int	i;

	i = 0;
	while (i < table->number_of_philos)
	{
		pthread_mutex_destroy(&table->fork_mutexes[i]);
		pthread_mutex_destroy(&table->philos[i]->meal_time_mutex);
		i++;
	}
	pthread_mutex_destroy(&table->write_mutex);
	pthread_mutex_destroy(&table->sim_ended_mutex);
}

/* free_table:
プログラムによって割り当てられたすべてのメモリを解放します。
解放するものが何もない場合、またはすべてのメモリが解放された場合は、NULLポインターを返します。
pthread_mutex_destroy()関数はmutexの破棄を行いますが、mutexが格納されているメモリ領域自体は解放しません。
table->fork_mutexesは、malloc()で確保されたメモリ領域を指しており、
このメモリ領域は明示的にfree()関数を呼び出すことで解放する必要があります。*/
void	*free_table(t_table *table)
{
	unsigned int	i;

	if (!table)
		return (NULL);
	destroy_mutexes(table);
	if (table->fork_mutexes != NULL)
		free(table->fork_mutexes);
	if (table->philos != NULL)
	{
		i = 0;
		while (i < table->number_of_philos)
		{
			if (table->philos[i] != NULL)
				free(table->philos[i]);
			i++;
		}
		free(table->philos);
	}
	free(table);
	return (NULL);
}

/* print_error:
コンソールにメッセージを書き込みます。指定されたexit_noを返します。
エラー管理に使用されます。*/
int	print_error(char *str, char *detail, int exit_no)
{
	if (!detail)
		printf("%s", str);
	else
		printf(str, detail);
	return (exit_no);
}

/* free_error_ret_null:
割り当てられたメモリを解放し、エラーメッセージを出力してNULLポインタを返します。
初期化中のエラー管理に使用されます。*/
void	*free_error_ret_null(char *str, char *details, t_table *table)
{
	if (table != NULL)
		free_table(table);
	print_error(str, details, EXIT_FAILURE);
	return (NULL);
}

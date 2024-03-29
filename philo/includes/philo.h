/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakahar <ysakahar@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 21:07:35 by ysakahar          #+#    #+#             */
/*   Updated: 2023/04/04 14:41:59 by ysakahar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>
# include <limits.h>
# include <sys/time.h>
# include <pthread.h>

/* Macros */
# define MAX_PHILOSOPHERS_STR "200"

# ifndef DEBUG_MODE
#  define DEBUG_MODE 0
# endif

# define RESET	"\e[0m"
# define RED	"\e[31m"
# define GREEN	"\e[32m"
# define YELLOW	"\e[33m"
# define BLUE	"\e[34m"
# define PURPLE	"\e[35m"
# define CYAN	"\e[36m"

# define USAGE	"usage: ./philo <number_of_philosophers> \
<time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]\n"
# define ERR_INPUT_DIGIT	"Invalid input: '%s' is \
not a valid unsigned integer between 0 and 2147483647.\n"
# define ERR_PHILO_RANGE	"Invalid input: \
The number of philosophers must be between 1 and %s.\n"
# define ERR_THREAD	"Error: Failed to create thread.\n"
# define ERR_MUTEX	"Error: Failed to create mutex.\n"
# define ERR_MALLOC	"Error: Failed to allocate memory.\n"

/* Structures */
typedef struct s_table	t_table;

typedef struct s_philo
{
	pthread_t			thread;
	unsigned int		id;
	unsigned int		fork[2];
	unsigned int		meal_count;
	time_t				last_meal_time;
	pthread_mutex_t		meal_time_mutex;
	t_table				*table;
}	t_philo;

typedef struct s_table
{
	time_t			start_time;
	unsigned int	number_of_philos;
	pthread_t		grim_reaper;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				must_eat_count;
	bool			is_sim_ended;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	is_sim_ended_mutex;
	pthread_mutex_t	*fork_mutexes;
	t_philo			**philos;
}	t_table;

typedef enum e_ph_status
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	GOT_FORK_1 = 4,
	GOT_FORK_2 = 5
}	t_ph_status;

/* create_table.c */
int				strdigit_to_int(char *str);
t_table			*create_table(int argc, char **argv);

/* free_and_errors.c */
int				print_error(char *str, char *detail, int exit_no);
void			*free_error_ret_null(char *str, char *details, t_table *table);
void			*free_table(t_table *table);

/* grim_reaper.c */
void			*grim_reaper(void *data);

/* input_validator.c */
bool			is_valid_input(int argc, char **argv);

/* output.c */
void			print_meal_completion_status(t_table *table);
void			output_status(t_philo *philo, bool reaper, t_ph_status status);

/* philosopher_routine.c */
void			think_routine(t_philo *philo);
void			got_fork_and_eat_routine(t_philo *philo);
void			sleep_routine(t_philo *philo);

/*	philosopher.c */
void			*philosopher(void *data);

/* time_operations.c */
time_t			get_current_time_ms(void);
void			wait_time_for_action(t_table *table, time_t wait_time);
void			wait_until_start_time(time_t start_time);

/* utils.c */
void			*ft_calloc(size_t count, size_t size);
bool			is_simulation_ended(t_table *table);
void			set_simulation_ended(t_table *table);

#endif
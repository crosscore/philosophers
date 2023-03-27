/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_validator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakahar <ysakahar@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 21:06:43 by ysakahar          #+#    #+#             */
/*   Updated: 2023/03/24 18:12:51 by ysakahar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* strdigit_to_int:
数字のみの文字列を正の整数に変換します。
0からINT MAXの間の変換された数値を返します。
変換後の数値がINT MAXを超える場合は-1を返します。*/
int	strdigit_to_int(char *str)
{
	unsigned long long int	n;
	int						i;

	i = 0;
	n = 0;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		n = n * 10 + (str[i] - '0');
		i++;
	}
	if (n > INT_MAX)
		return (-1);
	return ((int)n);
}

/* is_all_digits:
文字列に0～9の数字のみが含まれているかどうかを確認します。
文字列に数字のみが含まれている場合はtrueを返します。
文字列に数字以外の文字が含まれている場合はfalseを返します。*/
static bool	is_all_digits(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || '9' < str[i])
			return (false);
		i++;
	}
	return (true);
}

/* is_valid_input:
必要なすべての引数が有効かどうか、
つまりINT MAXを超えない数字のみの文字列かどうかを確認します。
また、哲学者の数が有効かどうかもチェックします(1~MAX_PHILOSの間)。
哲学者の数が0(n == 0)の時もエラーを返す必要があるため、 n <= 0 の条件は必要です。
すべての引数が有効な場合はtrueを返し、そのうちの1つが無効な場合はfalseを返します。*/
bool	is_valid_input(int argc, char **argv)
{
	int	i;
	int	n;

	i = 1;
	while (i < argc)
	{
		if (!is_all_digits(argv[i]))
			return (print_error(ERR_INPUT_DIGIT, argv[i], false));
		n = strdigit_to_int(argv[i]);
		if (i == 1)
		{
			if (n <= 0 || strdigit_to_int(MAX_PHILOSOPHERS_STR) < n)
				return (print_error(ERR_PHILO_RANGE, \
						MAX_PHILOSOPHERS_STR, false));
		}
		else
		{
			if (n == -1)
				return (print_error(ERR_INPUT_DIGIT, argv[i], false));
		}
		i++;
	}
	return (true);
}

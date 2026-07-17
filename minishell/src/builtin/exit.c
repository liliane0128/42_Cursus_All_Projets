/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 14:31:04 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/17 12:55:32 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** check_over() verifies whether adding a new digit to the current
** result (res * 10 + digit) would overflow the given limit.
**
** Example:
** limit = LLONG_MAX (9223372036854775807)
** res   = 922337203685477580
** digit = 8  → would overflow
*/
static bool	check_over(long long limit, unsigned long long res, int digit)
{
	return (res > (unsigned long long)limit / 10
		|| (res == (unsigned long long)limit / 10
			&& (unsigned long long)digit > (unsigned long long)limit % 10));
}

static bool	is_overflow(const char *arg)
{
	int					i;
	int					sign;
	unsigned long long	res;

	i = 0;
	sign = 1;
	res = 0;
	if (arg[i] == '+' || arg[i] == '-')
	{
		if (arg[i++] == '-')
			sign = -1;
	}
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (true);
		if ((sign == 1 && check_over(LLONG_MAX, res, arg[i] - '0')) \
			|| (sign == -1 && check_over(LLONG_MAX + 1ULL, res, arg[i] - '0')))
			return (true);
		res = res * 10 + (arg[i++] - '0');
	}
	return (false);
}

/*
** Use 'long long' instead of 'long' to ensure 64-bit width
** across all platforms (e.g., on 32-bit systems, 'long' may be only 4 bytes).
*/
long long	ft_atoll(const char *str)
{
	int			i;
	int			sign;
	long long	res;

	res = 0;
	i = 0;
	sign = 1;
	if (!str)
		return (0);
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * sign);
}

static bool	check_numeric_arg(const char *arg)
{
	int	i;

	if (!arg)
		return (false);
	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

int	exit_builtin(t_data *data, char **args)
{
	long long	status;

	(void)data;
	status = 0;
	if (args[1])
	{
		if (!check_numeric_arg(args[1]) || is_overflow(args[1]))
		{
			status = errmsg_cmd("exit", args[1],
					"numeric argument required", 2);
			cleanup_and_exit(2, data, NULL, 0);
		}
		status = ft_atoll(args[1]);
		if (args[2])
			return (errmsg_cmd("exit", NULL, "too many arguments", 1));
	}
	printf("exit\n");
	cleanup_and_exit((int)(status % 256), data, NULL, 0);
	return (0);
}

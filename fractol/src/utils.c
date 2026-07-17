/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:22:23 by lilwang           #+#    #+#             */
/*   Updated: 2025/08/09 13:13:31 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

void	change_iterations(t_fractal *f, int delta)
{
	f->max_iter += delta * 100;
	if (f->max_iter < 10)
		f->max_iter = 10;
	else if (f->max_iter > 1000)
		f->max_iter = 1000;
}

static double	parse_fraction(const char *s, int *i)
{
	double	fraction;
	int		frac_div;

	fraction = 0.0;
	frac_div = 1;
	(*i)++;
	while (ft_isdigit(s[*i]))
	{
		fraction = fraction * 10.0 + (s[*i] - '0');
		frac_div *= 10;
		(*i)++;
	}
	return (fraction / frac_div);
}

double	ft_atof(const char *s)
{
	double	result;
	double	sign;
	int		i;

	result = 0.0;
	sign = 1.0;
	i = 0;
	while (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13))
		i++;
	if (s[i] == '-' || s[i] == '+')
	{
		if (s[i] == '-')
			sign = -1.0;
		i++;
	}
	while (ft_isdigit(s[i]))
	{
		result = result * 10.0 + (s[i] - '0');
		i++;
	}
	if (s[i] == '.')
		result += parse_fraction(s, &i);
	return (sign * result);
}

int	is_valid_float(const char *s)
{
	int	i;
	int	has_dot;
	int	has_digit;

	i = 0;
	has_dot = 0;
	has_digit = 0;
	if (!s || !*s)
		return (0);
	if (s[i] == '-' || s[i] == '+')
		i++;
	while (s[i])
	{
		if (s[i] == '.' && !has_dot)
			has_dot = 1;
		else if (ft_isdigit(s[i]))
			has_digit = 1;
		else
			return (0);
		i++;
	}
	return (has_dot && has_digit);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 16:07:10 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/07 16:07:12 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>
#include <cstdlib>

bool		is_only_whitespace(std::string str);
std::string get_input(std::string input);
bool		is_numeric(std::string str);

#endif
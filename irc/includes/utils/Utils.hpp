/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 14:04:55 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/10 14:39:38 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <climits>
#include <cerrno>
#include <stdexcept>

namespace Utils
{
	std::vector<std::string> splitCommaSeparatedList(const std::string& list);
	std::vector<std::string> splitSpaceSeparatedList(const std::string& list);

	void validatePort(const std::string &portStr, int &port);
	void validatePassword(const std::string &password);
}
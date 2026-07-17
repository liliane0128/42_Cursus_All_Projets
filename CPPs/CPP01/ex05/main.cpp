/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:15:31 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/16 17:15:32 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Harl.hpp"

int main()
{
	Harl npc;

	npc.complain("DEBUG");
	npc.complain("INFO");
	npc.complain("WARNING");
	npc.complain("ERROR");
	npc.complain("NOTHING");

	return 0;
}
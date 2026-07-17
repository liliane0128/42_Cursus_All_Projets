/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCNumericCode.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 12:58:47 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/08 16:33:11 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef IRCNUMERICCODE_HPP
#define IRCNUMERICCODE_HPP

// Numeric reply and error codes (as per RFC 2812 §5)
// (with the excption of ERR_NONE, project specific constant of our ft_irc).

enum NumericCode
{
	ERR_NONE = 0, // FT_IRC project specific constant

	// Replies (RPL)
	//--------------
	RPL_WELCOME = 001, // Welcome message upon successful user registration
	RPL_YOURHOST = 002, // Your host information
	RPL_CREATED = 003, // Server creation date
	RPL_MYINFO = 004, // Server information
	// RPL_ISUPPORT = 005,
	RPL_UMODEIS = 221,
	// RPL_NONE = 300, // Dummy reply
	// RPL_ISON = 303, // User is online
	RPL_WHOISUSER = 311, // WHOIS user information
	RPL_WHOISSERVER = 312, // WHOIS server information
	RPL_ENDOFWHOIS = 318,
	RPL_WHOISCHANNEL = 319,
	RPL_LIST = 322, // Channel list
	RPL_LISTEND = 323, // End of channel list
	RPL_CHANNELMODEIS = 324, // Channel mode information
	RPL_NOTOPIC = 331, // No topic is set
	RPL_TOPIC = 332, // Channel topic
	RPL_INVITING = 341, // Inviting user to channel
	RPL_WHOREPLY = 352, // WHO reply
	RPL_NAMREPLY = 353, // Channel user list
	RPL_ENDOFNAMES = 366, // End of channel user list
 	RPL_YOUREOPER = 381, //You are now an IRC operator"
  
	// Errors (ERR)
	//-------------
	ERR_NOSUCHNICK = 401, // No such nick/channel
	ERR_NOSUCHCHANNEL = 403, // No such channel
	ERR_CANNOTSENDTOCHAN = 404, // Cannot send to channel
	ERR_NORECIPIENT = 411, // No recipient
	ERR_NOTEXTTOSEND = 412, // No text to send
	ERR_UNKNOWNCOMMAND = 421, // Unknown command
	ERR_NONICKNAMEGIVEN = 431, // No nickname given
	ERR_ERRONEUSNICKNAME = 432, // Erroneous nickname
	ERR_NICKNAMEINUSE = 433, // Nickname is already in use
	ERR_NICKCOLLISION = 436, // Nickname collision
	ERR_USERNOTINCHANNEL = 441, // User not in channel
	ERR_NOTONCHANNEL = 442, // You're not on that channel
	ERR_USERONCHANNEL = 443, // User is already on that channel
	ERR_NEEDMOREPARAMS = 461, // Not enough parameters
	ERR_ALREADYREGISTRED = 462, // You may not reregister
	ERR_PASSWDMISMATCH = 464, // Password incorrect
	ERR_CHANNELISFULL = 471, // Channel is full
	ERR_UNKNOWNMODE = 472, // Unknown mode
	ERR_INVITEONLYCHAN = 473, // Invite-only channel
	ERR_BANNEDFROMCHAN = 474, // Banned from channel
	ERR_BADCHANNELKEY = 475, // Key incorrect
	ERR_NOCHANMODES = 477, // Modes not supported on channel
	ERR_NOPRIVILEGES = 481, // Permission denied (not an IRC operator)
	ERR_CHANOPRIVSNEEDED = 482, // You're not a channel operator
	ERR_CANTKILLSERVER = 483, // Can't kill server
	ERR_RESTRICTED = 484, // Connection is restricted
	ERR_NOTREGISTERED = 451, // YOu have not registered 
	ERR_UMODEUNKNOWNFLAG = 501, // Unknown MODE flag
	ERR_USERSDONTMATCH = 502, // Users don't match

	// Extra (ft_irc specific, design choice)
	//-------------
	ERR_NICKPOLICYVIOLATION = 901 
};

#endif
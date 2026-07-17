*This project has been created as part of the 42 curriculum by lilwang, neandrie*

---

# **ft_irc**
*A minimalist IRC server implemented in C++98.*

---

## **Description**
**ft_irc** is a simplified implementation of an **IRC server** (Internet Relay Chat). This project aims to understand the basics of networking, object-oriented programming in C++98, and concurrent management of multiple clients.
This is a single-server implementation, which follows for applicable commands for a single-server implementation of the protocol [RFC 2812](https://tools.ietf.org/html/rfc2812).
MODE command is implemented as per RFC 2811 §4 [RFC 2811](https://tools.ietf.org/html/rfc2811) for the the required channel modes (+i, +t, +k, +o, +l) (with overall understanding drawn from the examples in RFC 1459 §4.2.3.1 [RFC 1459](https://tools.ietf.org/html/rfc1459) which remained relevant in particular for these specific flags).
Refer to the section key features here-in for the exact list of commands implemented in this ft_irc server.

Please note some specific design choices have been made in this IRC implementation when connecting with IRSSI clients :
- When MODE nickname +i is received upon a successfull client connection, the +i (command to make the client invisible) is ignored and an error is returned : ERR_UMODEUNKNOWNFLAG = 501. No user mode implemented, only IRC operator status (with OPER command).
- When MODE #channel b is received upon joining a channel, the +b (command to set a ban mask to keep users out of the channel) is ignored and an error is returned : ERR_UNKNOWNMODE = 472. b flag not implemented.
- With regards to nickname change, we acknowledge that upon connecting with a nickname already in use, IRSSI will generate an alternative nickname (defined as nickname + "_"), we decide to reject such nickname by design and return a custom-made ERR_NICKPOLICYVIOLATION = 901.

---

## **Key Features**

**Core IRC Protocol Implementation**
- User authentication (PASS, NICK, USER)
- IRC operator privileges (OPER) (but no client mode)
- Private messaging (PRIVMSG)
- File transfer (DCC)
- Channel management (JOIN, PART, KICK, INVITE, MODE)
- Channel modes (+i, +t, +k, +o, +l exclusively)
- Channel type (# public channel and & local channel only)
- Information sharing (WHOIS, LIST)
- Connection check (PING)
- Disconnection (QUIT)

**Network Architecture**
- Non-blocking I/O with poll()
- TCP socket programming
- Concurrent client handling

**Error Handling**
- Comprehensive RFC-compliant error replies
- Graceful client disconnection handling

---

## **Reference client**
- IRC client program : IRSSI as installed on 42 Paris Unbuntu-run computers.
dpkg -l | grep irssi => irssi/1.2.3-1ubuntu4/amd64/"terminal based IRC client"
- Network utility tool acting act as a simple TCP client or server (used for debugging and file transfer) : Netcast as installed on 42 Paris Unbuntu-run computers.
 dpkg -l | grep netcat => netcat-openbsd/1.218-4ubuntu1/amd64/"TCP/IP swiss army knife"

---

## **Instructions**

### **Prerequisites**
- Unix-based system (Linux)
- C++98 compiler (g++/clang++)
- Tested with: 
	- Ubuntu clang v. 12.0.1-19ubuntu3, 
	- irssi v. 1.2.3-1ubuntu4
	- netcat v. 1.218-4ubuntu1

---

### **Build & Run**
   Command               | Description                          |
 |-----------------------|--------------------------------------|
 | `make`                | Compile the server                  |
 | `make all`            | Compile all targets (server + bot)  |
 | `make re`             | Recompile the server                |
 | `make clean`          | Remove object files                 |
 | `make fclean`         | Remove all generated files          |
 | `make run`            | Run server with default port & pswd |
 | `make valgrind`       | Run server with valgrind            |

---

### **Run the Server**
- ./ircserv < port> < password>
Example : "./ircserv 6667 password" or "./ircserv 6666 6"

### **Run the Bot**
- ./ircbot < IP_localhost> < port> < password>

### **Connect with IRSSI**
- irssi
then in IRSSI
- /connect < IP_localhost> < port> < password> < nickname>

### **Connect with Netcat**
- nc -C < IP_localhost> < port> (Note : -C flag => Send CRLF as line-ending)

### **ChatBot Commands**
With IRSSI :
- /msg ChatBot !dice/!time/!joke

With Netcat :
- /privmsg ChatBot !dice/!time/!joke

### **DCC File Transfer (IRSSI only, not supported by Netcast)**:
Sender :
- /dcc send < target> < origin_file_path>

Receiver :
- /set dcc
- /set dcc_download_path < destination_folder>
- /dcc get

Note : DCC does not use any form of encryption and SHOULD be avoided.

---

## **Resources**

### **Websites**
- https://en.wikipedia.org/wiki/IRC general information about IRC history, functionning, basic command, file sharing (DCC) and protocol specification references.
- https://www.reddit.com/r/videos/comments/1n4oixu/the_history_of_irc_internet_relay_chat/?tl=fr about early adopter nostalgia, but also nickname and channel operators collision and network split issues on multi-server implementation (refer to EFnet/"A-net" case) which led to separate approaches and reduced momentum on the original IRC adoption.
- http://chi.cs.uchicago.edu/chirc/irc_examples.html IRC communication examples represented as UML sequence diagrams.
- https://tools.ietf.org/html/rfc1459 original IRC protocol specification (published in 1993).
	- comprehensive (though very accessible !) protocol description (67 pages).
	- detailed parsing rules and authorised characters.
	- list of numeric codes for replies and errors (§6).
- https://tools.ietf.org/html/rfc2812 updates and clarifies ambiguities in RFC 1459. It provides more precise definitions for commands and replies (published in 2000).
	- latest list of numeric codes for replies and errors (§5).
- https://modern.ircdocs.horse IRC documentation made easier to navigate with hypertext link.
- https://www.varonis.com/blog/netcat-commands about netcast, a generic client and server program with a large range of possible features (including port scanning with verbose, file transfer, etc). Refer to the cheat-sheet of nc commands at the end of the webpage for quick reference.
- https://en.wikipedia.org/wiki/Irssi general information about Irssi, an IRC client program.
- https://irssi.org/New-users/ users guide for Irssi IRC
- https://en.wikipedia.org/wiki/Direct_Client-to-Client brief description of DCC, an IRC subprotocol enabling peer-to-peer file transfer after IRC server handshake.
- https://www.indusface.com/learning/osi-model/ introduction to the OSI (Open Systems Interconnection) 7-layers model for standardizing how network protocols and technologies communicate. Developped by the ISO in 1984.
	- IRC at the Application level (including DCC handshaking as a process in the Client IRC program)
	- TCP at the Transport level (TCP connection for IRC (client-server-client) and DCC(client-client))
	- IP at the Network level (packet transfer)
- https://web.maths.unsw.edu.au/~lafaye/CCM/sockets/sockintro.htm?utm_source=chatgpt.com socket() is a system call C funtion that allows applications to create and use TCP/UDP connections provided by the operating system.
- https://en.wikibooks.org/wiki/The_World_of_Peer-to-Peer_(P2P)/Networks_and_Protocols/IRC explains the scope of each RFC version past 1459 :
	- RFC 2810 : IRC Architecture
	- RFC 2811 : IRC Channel-Management
	- RFC 2812 : IRC Client-Protocol
	- RFC 2813 : IRC Server-Protocol
- https://www.ietf.org/process/rfcs/ defines RFC as Requests for Comments, the historic titles for the technical documentation published by the IETF (Internet Engineering Task Force). RFCs are the foundational documents that define Internet protocols — TCP, IP, HTTP, DNS, SMTP, IRC, and hundreds of others.
- https://patorjk.com/software/taag/ create ASCII Art from text.
- https://www.iana.org/assignments/service-names-port-numbers/service-names-port-numbers.xhtml?search=irc service name and transport protocol port number registry, specifies 6665-6669 for IRCU (plain text) and 6697 for IRCS-U (note : IRC via TLS/SSL => TCP connection as an encrypted tunnels based on cryptographic protocols, changes are at the transport layer only, does not affect the IRC protocol itself).
- https://datatracker.ietf.org/doc/html/rfc7194 [RFC7194] indicates that although system port 194 is registered for IRC, it is not used in practice (ports below 1024 are restricted). IRC networks have defaulted to listening on TCP port 6667 for plain text connections for a considerable time now.  This is covered by the IRCU assignment of TCP/UDP ports 6665-6669. Port 6697 is the one commonly used for TLS/SSL encrypted connections [RFC5246].
- https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form Backus-Naur form, a.k.a BNF, is a notation system for defining the syntax of programming languages, such as, for example : <postal-address> ::= <name-part> <street-address> <zip-part> <EOL> . A pseudo BNF notation is widely used in RFC 1459.

---

### **Use of AI**
- identifying which IRC client programs are installed on 42 Paris computers.
Example : "which irssi hexchat weechat xchat quassel konversation" =>
	/usr/bin/irssi
	hexchat not found
	weechat not found
	xchat not found
	quassel not found
	konversation not found
- finding adequate shell commands for running Irssi / Netcast based tests.
- questionning on pro and cons of choosing DCC for file transfer over alternatives (XDCC, HTTP/FTP...)
- investigating possibilities for spliting the project scope and defining possible task list.
- questionning on which RFC standard to choose from as a reference for our project (note : RFC 2810, 2811, 2813 focus on architecture, extensions, and services which are not necessary for a basic single node server)
- Colors.hpp : proposing a standard list of colors and matching ANSI Color Code.
- IRCNumericCode.hpp, IRCReplyBuilder.hpp : listing of IRC numeric codes for replies/errors as a substract of RFC 2812 §5 and creating matching template text messages according to RFC 2812 §5.
- brainstorming on which classes to instantiate, which to define as static class and which sequencing for calling the contructors that are maintained as long as the REPL loop is active.
- debating on how to implement non-blocking I/O with poll().
- understanding that ports 0-1023 are privileged on POSIX systems per RFC 7194.
- defining ChannelMode with bitwise formulation (e.g., MODE_INVITE = (1 << 0)) for efficiency, flexibility, and clarity to enable bitmasking for combining modes and fast mode checking (i.e. same principles as chmod -r and chmod 777 with bash).
- deciding to implement channels as maps of name (key) and Channel (i.e. std::map<std::string, Channel> _channels).
- decipher RFC 2812 §2.3.1 permited characters for "chanstring" (i.e. a channel given name).
- implement most getter and setter methods.
- debug of the "r" vs. "/r" typo. in user name validity check.
- explore how to setup InspIRCd in Docker for IRC Server comparison testing using script tailored to netcast messaging.
- code to test IP valid address for DCC message forwarding.
- reorganise the whole list of client and server::methods() by family type for greater visibility.
- facilitate generation of redundant code for parsing of the command parameters.
- brainstorming on finding some function/method and variable/attribute unambiguous names.
- debug on user mode confusion vs. operator right.

---

### **Peer-learning**
- understanding of the overall project objective with demonstration of ft_IRC communication with Irssi (presented as the easy default choice for IRC client).
- identifying DCC as the optimum and native option for file transfer protocol over IRC-based chats.
- learning about the existance of Crtl^Z ("terminal stop", SIGTSTP), which causes the process to suspend execution.
- Alt + 1 / Alt + 2 / etc. and Ctrl^N for accessing the different active channel opened in irssi and privmsg (last Alt = privmsg)

---

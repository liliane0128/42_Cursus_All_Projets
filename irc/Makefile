# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/11 14:06:27 by neandrie          #+#    #+#              #
#    Updated: 2026/04/10 19:21:14 by neandrie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ==============================
#   Configuration
# ==============================

# Project name
NAME        := ircserv
BOT_NAME    := ircbot

# Default parameters
PORT ?= 6666
PASSWD ?= 6

# Directories
SRC_DIR     := src
OBJ_DIR     := obj
INC_DIR     := includes

# Compiler and flags
CXX         := c++
CXXFLAGS    := -std=c++98 -Wall -Wextra -Werror
DBG_FLAGS   := -g
ASAN_FLAGS  := -g -O1 -fsanitize=address,undefined -fno-omit-frame-pointer

# Valgrind options
VALGRIND    := valgrind \
			--leak-check=full \
			--show-leak-kinds=all \
 			--track-fds=all \
  			--track-origins=yes \

# Sources
SERV_SRCS        := \
	$(SRC_DIR)/main.cpp \
	$(SRC_DIR)/server/Server.cpp \
	$(SRC_DIR)/channel/Channel.cpp \
	$(SRC_DIR)/client/Client.cpp \
	$(SRC_DIR)/utils/IRCReplyBuilder.cpp \
	$(SRC_DIR)/commands/CommandHandler.cpp \
	$(SRC_DIR)/commands/CommandParser.cpp \
	$(SRC_DIR)/commands/DCCCommand.cpp \
	$(SRC_DIR)/commands/INVITECommand.cpp \
	$(SRC_DIR)/commands/JOINCommand.cpp \
	$(SRC_DIR)/commands/KICKCommand.cpp \
	$(SRC_DIR)/commands/LISTCommand.cpp \
	$(SRC_DIR)/commands/MODECommand.cpp \
	$(SRC_DIR)/commands/PARTCommand.cpp \
	$(SRC_DIR)/commands/PRIVMSGCommand.cpp \
	$(SRC_DIR)/commands/TOPICCommand.cpp \
	$(SRC_DIR)/commands/PASSCommand.cpp \
	$(SRC_DIR)/commands/NICKCommand.cpp \
	$(SRC_DIR)/commands/USERCommand.cpp \
	$(SRC_DIR)/commands/QUITCommand.cpp \
	$(SRC_DIR)/commands/PINGCommand.cpp \
	$(SRC_DIR)/commands/WHOISCommand.cpp \
	$(SRC_DIR)/commands/OPERCommand.cpp \

BOT_SRCS		:= \
	$(SRC_DIR)/main_bot.cpp \
	$(SRC_DIR)/bot/Bot.cpp

COMMON_SRCS := \
	$(SRC_DIR)/utils/Utils.cpp \

# Header files
HEADERS     := \
	$(INC_DIR)/server/Server.hpp \
	$(INC_DIR)/channel/Channel.hpp \
	$(INC_DIR)/client/Client.hpp \
	$(INC_DIR)/bot/Bot.hpp \
	$(INC_DIR)/utils/Colors.hpp \
	$(INC_DIR)/utils/IRCNumericCode.hpp \
	$(INC_DIR)/utils/IRCReplyBuilder.hpp \
	$(INC_DIR)/utils/Utils.hpp \
	$(INC_DIR)/commands/CommandHandler.hpp \
	$(INC_DIR)/commands/CommandParser.hpp \
	$(INC_DIR)/commands/DCCCommand.hpp \
	$(INC_DIR)/commands/INVITECommand.hpp \
	$(INC_DIR)/commands/JOINCommand.hpp \
	$(INC_DIR)/commands/KICKCommand.hpp \
	$(INC_DIR)/commands/LISTCommand.hpp \
	$(INC_DIR)/commands/MODECommand.hpp \
	$(INC_DIR)/commands/PARTCommand.hpp \
	$(INC_DIR)/commands/PRIVMSGCommand.hpp \
	$(INC_DIR)/commands/TOPICCommand.hpp \
	$(INC_DIR)/commands/PASSCommand.hpp \
	$(INC_DIR)/commands/NICKCommand.hpp \
	$(INC_DIR)/commands/USERCommand.hpp \
	$(INC_DIR)/commands/QUITCommand.hpp \
	$(INC_DIR)/commands/PINGCommand.hpp \
	$(INC_DIR)/commands/WHOISCommand.hpp \
	$(INC_DIR)/commands/OPERCommand.hpp \


# Object files
SERV_OBJS   	:= $(SERV_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)\
				   $(COMMON_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
				   
BOT_OBJS    	:= $(BOT_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)\
				   $(COMMON_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Object directories
ALL_OBJS    := $(SERV_OBJS) $(BOT_OBJS)
OBJ_DIRS    := $(sort $(dir $(ALL_OBJS)))

# Include flags
INCLUDES    := -I$(INC_DIR)

# ==============================
#   Rules
# ==============================

all: $(NAME) $(BOT_NAME)

$(NAME): $(SERV_OBJS)
	@echo "🔧 Linking $(NAME)..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@
	@echo "✅ Build finished!"

$(BOT_NAME): $(BOT_OBJS)
	@echo "🔧 Linking $(BOT_NAME)..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@
	@echo "✅ $(BOT_NAME) built!"

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIRS)
	@echo "⚙️  Compiling $<"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# Create object directories if they don't exist
$(OBJ_DIRS):
	@echo "📁 Creating object directories..."
	@mkdir -p $@

# ==============================
#   Additional Rules
# ==============================

run: all
	@echo "🚀 Running ./$(NAME) $(PORT) $(PASSWD)..."
	./$(NAME) $(PORT) $(PASSWD)

valgrind: all
	@echo "🔍 Running valgrind on ./$(NAME) $(PORT) $(PASSWD)..."
	$(VALGRIND) ./$(NAME) $(PORT) $(PASSWD)

clean:
	@echo "🧹 Cleaning object files and directories..."
	rm -rf $(OBJ_DIR)

fclean: clean
	@echo "🧨 Removing binary..."
	rm -f $(NAME) $(BOT_NAME)

re: fclean all

.PHONY: all clean fclean re run valgrind

# ==============================
#   Dependency Management
# ==============================

-include $(ALL_OBJS:.o=.d)

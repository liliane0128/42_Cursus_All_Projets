NAME	= philo_bonus

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -pthread
RM		= rm -f
OBJDIR	= objs

SRCS	= \
			./src/main_bonus.c \
			./src/parsing_bonus.c \
			./src/init_bonus.c \
			./src/start_philos_bonus.c \
			./src/philosopher_bonus.c \
			./src/end_process_bonus.c \
			./src/utils_bonus.c

GREEN        = \033[32m
CYAN        = \033[36m
RESET        = \033[0m
ORANGE        = \033[33m
BOLD_CYAN    = \033[1;36m
ROSE        = \033[38;5;218m

define ASCII_ART
$(CYAN)

██████╗ ██╗  ██╗██╗██╗      ██████╗ ███████╗ ██████╗ ██████╗ ██╗  ██╗███████╗██████╗ 
██╔══██╗██║  ██║██║██║     ██╔═══██╗██╔════╝██╔═══██╗██╔══██╗██║  ██║██╔════╝██╔══██╗
██████╔╝███████║██║██║     ██║   ██║███████╗██║   ██║██████╔╝███████║█████╗  ██████╔╝
██╔═══╝ ██╔══██║██║██║     ██║   ██║╚════██║██║   ██║██╔═══╝ ██╔══██║██╔══╝  ██╔══██╗
██║     ██║  ██║██║███████╗╚██████╔╝███████║╚██████╔╝██║     ██║  ██║███████╗██║  ██║
╚═╝     ╚═╝  ╚═╝╚═╝╚══════╝ ╚═════╝ ╚══════╝ ╚═════╝ ╚═╝     ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝


$(RESET)
endef
export ASCII_ART

OBJS	= $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$$ASCII_ART"

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	$(RM) $(OBJS)
	$(RM) -r $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
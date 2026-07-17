NAME = libftprintf.a

SRCS = ft_printf.c ft_convert_base.c ft_printf_char.c \
	ft_printf_str.c ft_printf_utils.c

OBJS = $(SRCS:.c=.o)

CC = gcc

CFLAGS = -Wall -Wextra -Werror

RM = rm -f

AR = ar rcs

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean:
	$(MAKE) clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

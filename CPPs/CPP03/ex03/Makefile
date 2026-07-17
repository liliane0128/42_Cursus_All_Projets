# -Wshadow:
# Enables warnings when a variable declaration shadows another variable 
# in an outer scope (e.g., a local variable hiding a global one, or 
# a derived class member hiding a base class member). This helps prevent 
# bugs where the programmer accidentally operates on the wrong data.

# -Wno-shadow:
# Disables the shadowing warning. This is used when variable shadowing 
# is a deliberate architectural choice, allowing multiple variables with 
# the same name to exist in different scopes of the same object hierarchy 
# without the compiler flagging it as a potential error.

NAME = DiamondTrap

CXX	= c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD -MP -Wshadow

INCLUDE = DiamondTrap.hpp ClapTrap.hpp FragTrap.hpp ScavTrap.hpp

OBJ_DIR = objs/
SRCS = ClapTrap.cpp FragTrap.cpp main.cpp ScavTrap.cpp DiamondTrap.cpp

OBJS = $(addprefix $(OBJ_DIR), $(SRCS:.cpp=.o))
DEPS =	$(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(DEPS)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
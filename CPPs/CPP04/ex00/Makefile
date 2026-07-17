NAME = Polymorphism

CXX	= c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD -MP

INCLUDE = Animal.hpp Dog.hpp Cat.hpp WrongCat.hpp WrongAnimal.hpp

OBJ_DIR = objs/
SRCS = Animal.cpp Dog.cpp Cat.cpp main.cpp WrongCat.cpp WrongAnimal.cpp
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
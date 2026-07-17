NAME = Form2

CXX	= c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD -MP

INCLUDE = Bureaucrat.hpp AForm.hpp ShrubberyCreationForm.hpp RobotomyRequestForm.hpp \
		  PresidentialPardonForm.hpp

OBJ_DIR = objs/
SRCS = Bureaucrat.cpp main.cpp AForm.cpp ShrubberyCreationForm.cpp RobotomyRequestForm.cpp \
	   PresidentialPardonForm.cpp
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
# DEPS converts the list of object files (.o) to dependency files (.d).
# Combined with -include and compiler flags -MMD -MP, this ensures that
# any change in a header file (.hpp) triggers a re-compilation of all 
# source files that include it.
# -MMD: Generate dependency files (.d) for user headers, ignoring system ones.
# -MP:  Adds a phony target for each dependency to prevent 'make' errors 
#       if a header file is deleted or renamed.

NAME = phonebook

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD -MP

INCLUDES = Contact.hpp Phonebook.hpp Utils.hpp

SRC = Contact.cpp Phonebook.cpp main.cpp Utils.cpp
OBJS = $(SRC:.cpp=.o)
DEPS =	$(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	rm -f $(DEPS)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re

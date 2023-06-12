# Variable
NAME		= project3
CXX			= g++
CXXFLAGS	= -std=c++14 # -g -fsanitize=address
RM			= rm -rf

# srcs
SRCS =	main.cpp \
		Kernel.cpp \
		Kernel_util.cpp \
		Process.cpp \
		Process_util.cpp \
		Memory.cpp \
		Pmemory.cpp
OBJS = $(SRCS:.cpp=.o)

# rules
all : $(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean : 
	$(RM) $(OBJS)

re : clean all

.PHONY : all re clean

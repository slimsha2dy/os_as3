# Variable
NAME		= project2
CXX			= g++
CXXFLAGS	= -std=c++14
RM			= rm -rf

# srcs
SRCS =	main.cpp \
		Kernel.cpp \
		Kernel_util.cpp \
		Process.cpp \
		Process_util.cpp
OBJS = $(SRCS:.cpp=.o)

# rules
all : $(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean : 
	$(RM) $(OBJS) $(NAME)

re : clean all

.PHONY : all re clean

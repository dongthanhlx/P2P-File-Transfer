# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -Wall

# the list of header files
#INCL   = server.h
#the list of source files
SRC = server.cpp
OBJ = $(SRC:.cpp=.o)

# the build target executable:
TARGET = server

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -pthread -o $(TARGET) $(OBJ)

$(OBJ): $(INCL)

clean:
	$(RM) $(TARGET) $(OBJ)

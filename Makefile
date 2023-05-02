# Compiler
CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -std=c++11 -O2 -g

# Directories
SRC_DIR=./src
OBJ_DIR=./obj
LIB_DIR=./libs/gmp

# Libraries
LIBS=-L$(LIB_DIR) -lgmp

# Source files
SRCS=$(wildcard $(SRC_DIR)/*.cpp)
OBJS=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Executable
EXEC=JOAOPLEX.out

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $(EXEC)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
	
run:
	./$(EXEC) input.txt 


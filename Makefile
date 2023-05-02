# folders
INCLUDE_FOLDER = ./include/
BIN_FOLDER = ./bin/
OBJ_FOLDER = ./obj/
SRC_FOLDER = ./src/
GMP_DIR = ./lib/gmp/
GMP_SRC = $(wildcard $(GMP_DIR)/*.c)
GMP_OBJ = $(patsubst $(GMP_DIR)/%.c, $(GMP_DIR)/%.o, $(GMP_SRC))
GMP_INC = -I$(GMP_DIR)

# cc and flags
CC = g++
CXXFLAGS = -std=c++11 -g -Wall
CXXFLAGS += $(GMP_INC)
LDFLAGS += $(GMP_OBJ)

# all sources, objs, and header files
MAIN = main
TARGET = JOAOPLEX.exe
SRC = $(wildcard $(SRC_FOLDER)*.cc)
OBJ = $(patsubst $(SRC_FOLDER)%.cc, $(OBJ_FOLDER)%.o, $(SRC))
INPUT = input/input.txt

$(OBJ_FOLDER)%.o: $(SRC_FOLDER)%.cc
	$(CC) $(CXXFLAGS) -c $< -o $@ -I$(INCLUDE_FOLDER)

all: $(OBJ)
	$(CC) $(CXXFLAGS) -o $(BIN_FOLDER)$(TARGET) $(OBJ)

clean:
	@rm -rf $(OBJ_FOLDER)* $(BIN_FOLDER)*
run:
	./bin/NOME_EXECUTAVEL.out < $(INPUT) 

CC = g++
CFLAGS = -pedantic -Wall
LIBS = -L./libs -I./src/include
SRC_DIR = src/
OBJ_DIR = obj/
BIN = ./sabirdan

SRCS = $(wildcard $(SRC_DIR)*.cpp)
OBJS = $(patsubst $(SRC_DIR)%.cpp,$(OBJ_DIR)%.o,$(SRCS))

all: compile

compile: directories $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS) -o $(BIN)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

run: compile
	$(BIN)

clean:
	rm -rf $(BIN) $(OBJ_DIR)

directories:
	mkdir -p $(OBJ_DIR)
	
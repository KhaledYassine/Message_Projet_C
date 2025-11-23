CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -std=c11
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/globals.c $(SRC_DIR)/user.c $(SRC_DIR)/send.c $(SRC_DIR)/view.c $(SRC_DIR)/ui.c $(SRC_DIR)/storage.c $(SRC_DIR)/command_handler.c
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
EXECUTABLE = $(BIN_DIR)/messaging_app

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJECTS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

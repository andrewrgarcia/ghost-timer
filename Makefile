CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = 

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Target executable
TARGET = $(BIN_DIR)/countdown

# Source files
SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/timer.c

# Object files (placing them in obj/)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

# Default target
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

# Compiling source files to object files inside obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create directories if they don't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Run the compiled program
run: all
	./$(TARGET)

# Clean up
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

# Install target
install: $(TARGET)
	install -m 755 $(TARGET) /usr/local/bin/

# Uninstall target
uninstall:
	rm -f /usr/local/bin/countdown

# Dependencies
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c $(SRC_DIR)/timer.h
$(OBJ_DIR)/timer.o: $(SRC_DIR)/timer.c $(SRC_DIR)/timer.h

.PHONY: all clean install uninstall run

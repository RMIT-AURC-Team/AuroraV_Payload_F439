# Compiler
CC = gcc

# Directories
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj
BIN_DIR = bin
LIB_DIR = lib

# Output executable name
TARGET = $(BIN_DIR)/data_interpretter
LIBRARY = $(LIB_DIR)/libdata.so

# Compiler flags
CFLAGS_DEBUG = -I$(INC_DIR) -Wall -Wextra -O0 -g -fPIC
CFLAGS_RELEASE = -I$(INC_DIR) -Wall -Wextra -O2 -fPIC

LDFLAGS = -L$(LIB_DIR) -ldata

# Find all .c files in the SRC_DIR and convert them to .o files in OBJ_DIR
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: release

# Release target
release: CFLAGS = $(CFLAGS_RELEASE)
release: $(TARGET) $(LIBRARY)

# Debug target
debug: CFLAGS = $(CFLAGS_DEBUG)
debug: $(TARGET)

# Link object files to create the final executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) -o $(TARGET)

# Compile .c files into .o files (with dependency on header files)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/%.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build the shared library
$(LIBRARY): $(OBJS) | $(LIB_DIR)
	$(CC) -shared $(OBJS) -o $(LIBRARY)

# Create directories if they don't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

# Clean up object files and binary (does not delete directories)
clean:
	rm -f $(OBJ_DIR)/* $(BIN_DIR)/* $(LIB_DIR)/*

# Phony targets
.PHONY: all clean debug release

# Compiler
CC = gcc
CC_arm = aarch64-linux-gnu-gcc

# Directories
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj
LIB_DIR = lib

# Output library names
LIBRARY_x86 = $(LIB_DIR)/libdata_x86.so
LIBRARY_arm = $(LIB_DIR)/libdata_arm.so

# Compiler flags
CFLAGS_DEBUG = -I$(INC_DIR) -Wall -Wextra -O0 -g -fPIC
CFLAGS_RELEASE = -I$(INC_DIR) -Wall -Wextra -O2 -fPIC

# Find all .c files in the SRC_DIR and convert them to .o files in OBJ_DIR
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS_x86 = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/x86_%.o,$(SRCS))
OBJS_arm = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/arm_%.o,$(SRCS))

all: release_x86 release_arm

# Release target for x86
release_x86: CFLAGS = $(CFLAGS_RELEASE)
release_x86: $(LIBRARY_x86)

# Release target for ARM
release_arm: CFLAGS = $(CFLAGS_RELEASE)
release_arm: $(LIBRARY_arm)

# Debug target for x86
debug_x86: CFLAGS = $(CFLAGS_DEBUG)
debug_x86: $(LIBRARY_x86)

# Debug target for ARM
debug_arm: CFLAGS = $(CFLAGS_DEBUG)
debug_arm: $(LIBRARY_arm)

# Compile .c files into .o files for x86 (with dependency on header files)
$(OBJ_DIR)/x86_%.o: $(SRC_DIR)/%.c $(INC_DIR)/%.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile .c files into .o files for ARM (with dependency on header files)
$(OBJ_DIR)/arm_%.o: $(SRC_DIR)/%.c $(INC_DIR)/%.h | $(OBJ_DIR)
	$(CC_arm) $(CFLAGS) -c $< -o $@

# Build the x86 shared library
$(LIBRARY_x86): $(OBJS_x86) | $(LIB_DIR)
	$(CC) -shared $(OBJS_x86) -o $(LIBRARY_x86)

# Build the ARM shared library
$(LIBRARY_arm): $(OBJS_arm) | $(LIB_DIR)
	$(CC_arm) -shared $(OBJS_arm) -o $(LIBRARY_arm)

# Create directories if they don't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

# Clean up object files and libraries
clean:
	rm -f $(OBJ_DIR)/* $(LIB_DIR)/*

# Phony targets
.PHONY: all clean debug_x86 debug_arm release_x86 release_arm

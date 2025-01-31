#!/bin/bash

# Set the directories
SRC_DIR="src"
INCLUDE_DIR="include"
BIN_DIR="bin"

# Create the bin directory if it doesn't exist
mkdir -p $BIN_DIR

# Compiler and flags
CC=gcc
CFLAGS="-Wall -Wextra -I$INCLUDE_DIR"

# Source files
SRC_FILES=("$SRC_DIR/arraylist.c" "$SRC_DIR/circuit.c" "$SRC_DIR/parser.c" "$SRC_DIR/main.c")

# Output executable
OUTPUT="$BIN_DIR/program"

# Compile the program
echo "Compiling the project..."
$CC $CFLAGS ${SRC_FILES[@]} -o $OUTPUT

if [ $? -eq 0 ]; then
    echo "Compilation successful. Executable created at $OUTPUT"
else
    echo "Compilation failed."
    exit 1
fi

#!/bin/bash

FILENAME="hw3"

# Compile the source files
gcc -o $FILENAME -Iinclude src/arraylist.c src/circuit.c src/parser.c src/main.c

# Move the compiled executable to the script's directory
mv $FILENAME "$(dirname "$0")"

# Make the executable runnable
chmod +x $FILENAME

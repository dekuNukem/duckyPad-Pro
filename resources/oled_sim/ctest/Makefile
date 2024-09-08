# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Target executable
TARGET = main

# Default rule
all: $(TARGET)

# Rule to build the target executable
$(TARGET): main.o
	$(CC) -o $(TARGET) main.o

# Rule to compile main.c into an object file
main.o: main.c
	$(CC) $(CFLAGS) -c main.c

# Clean rule to remove generated files
clean:
	rm -f $(TARGET) main.o
# Define the compiler and the flags
CC = gcc
CFLAGS = -Wall -O2

# Define the target executable
TARGET = main

# Define the source files
SRCS = main.c

# Define the object files
OBJS = $(SRCS:.c=.o)

# The default target, which is the first target, is the one that is executed
# when you run 'make' without any arguments
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to build object files from source files
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Clean rule to remove generated files
clean:
	rm -f $(TARGET) $(OBJS)

# Phony targets to prevent conflicts with files named 'clean', 'all', etc.
.PHONY: all clean
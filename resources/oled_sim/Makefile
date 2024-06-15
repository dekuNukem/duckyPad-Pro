# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Target executable name
TARGET = my_program

# Default target
all: $(TARGET)

# Target for my_program
$(TARGET): main.o ssd1306.o ssd1306_fonts.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o ssd1306.o ssd1306_fonts.o

# Compile main.c to main.o
main.o: main.c ssd1306.h ssd1306_fonts.h
	$(CC) $(CFLAGS) -c main.c

# Compile ssd1306.c to ssd1306.o
ssd1306.o: ssd1306.c ssd1306.h
	$(CC) $(CFLAGS) -c ssd1306.c

# Compile ssd1306_fonts.c to ssd1306_fonts.o
ssd1306_fonts.o: ssd1306_fonts.c ssd1306_fonts.h
	$(CC) $(CFLAGS) -c ssd1306_fonts.c

# Clean target
clean:
	rm -f $(TARGET) *.o *.bin *.png

.PHONY: all clean

CC = gcc
CFLAGS = -fsanitize=address -g
TARGET = rectangle
SRC = rectangle.c

# Default target
all: $(TARGET)

# Compilation rule
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Clean rule
clean:
	rm -f $(TARGET)


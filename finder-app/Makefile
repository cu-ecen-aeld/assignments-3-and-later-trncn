# Compiler settings
CC = gcc
CROSS_COMPILE ?=
CFLAGS = -Wall -Wextra -Werror

# Source files
SRCS = writer.c

# Target executable
TARGET = writer

.PHONY: all clean

# Default target to build the application
default: $(TARGET)

$(TARGET): $(SRCS)
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o $@ $^

# Clean target to remove the application
clean:
	rm -f $(TARGET)

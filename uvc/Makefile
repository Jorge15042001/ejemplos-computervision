# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O2 -I/usr/local/include

# Linker flags
LDFLAGS = -L/usr/local/lib/

# Libraries
LIBS = -luvc

# Source files
SRCS = main.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = capture_frames

# Default target
all: $(TARGET)

# Rule to build the target
$(TARGET): $(OBJS)
	mkdir -p ./images
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean


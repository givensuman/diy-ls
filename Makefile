CC = gcc
CFLAGS = -Wall -Wextra -g
SOURCES=$(shell find -type f -iname '*.c')
OBJECTS=$(foreach x, $(basename $(SOURCES)), $(x).o)
TARGET = ls

all: run clean

build: $(TARGET)

run: build
	@./$(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	@rm -f $(TARGET) $(OBJECTS)

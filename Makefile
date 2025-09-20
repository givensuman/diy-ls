CC = gcc
CFLAGS = -Wall -Wextra -g
SOURCES=$(shell find -type f -iname '*.c' ! -name 'test.c')
OBJECTS=$(foreach x, $(basename $(SOURCES)), $(x).o)
TARGET = ls

build: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	@rm -f $(TARGET) test $(OBJECTS) test.o

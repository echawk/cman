.POSIX:
.PHONY: clean test
CC=gcc
LDFLAGS:=$(LDFLAGS) -lncurses
CFLAGS:=$(CFLAGS) -std=c99 -Wall -pedantic
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
TARGET=cman

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -vf $(TARGET) $(OBJS)

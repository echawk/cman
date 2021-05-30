.POSIX:
.PHONY: clean test
CC=gcc
LDFLAGS:=$(LDFLAGS) -lncurses
CFLAGS:=$(CFLAGS) -std=c89 -Wall -pedantic -D_BSD_SOURCE
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

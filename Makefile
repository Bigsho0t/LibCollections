CC = gcc
AR = ar

CFLAGS = -Wall -Wextra -Wpedantic -std=c23 \
		  -Iinclude \
		  -Isrc/internal

ARFLAGS = rcs

LIB = libcollections.a

SRC = $(wildcard src/*.c) \
	  $(wildcard src/internal/*.c)

OBJ = $(SRC:.c=.o)

TEST = tests/teste.c

.PHONY: all clean test

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(LIB)
	$(CC) $(CFLAGS) $(TEST) -L. -lcollections -o test.exe

clean:
	rm -f $(OBJ) $(LIB) test.exe
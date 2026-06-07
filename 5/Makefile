CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -D_POSIX_C_SOURCE=200809L
TARGET = lab
SRC = lab.c func.c heap.c algorithms.c utils.c
OBJ = $(SRC:.c=.o)
HDR = structures.h func.h heap.h algorithms.h utils.h

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
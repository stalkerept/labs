CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS = -L. -Wl,-rpath='$$ORIGIN'

# Выбор реализации: USE=hash или USE=tree
ifndef USE
    USE=hash
endif

ifeq ($(USE),hash)
    STORAGE_OBJ = storage_hash.o func_hash.o hash.o
else ifeq ($(USE),tree)
    STORAGE_OBJ = storage_tree.o func_tree.o
else
    $(error USE must be hash or tree)
endif

all: game

game: lab.o game.o utils.o $(STORAGE_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o game

.PHONY: all clean

#get all files *.c
C_SOURCE = $(wildcard ../*.c)

#get all file *.o
OBJECTS = $(C_SOURCE:.c=.o)

CFLAGS = -Wall -Wextra -pedantic -O2 -g


# tests

test_queue: test_queue.o ../queue.o
	gcc $^ -o $@

test_prio: test_prio.o $(OBJECTS)
	gcc $^ -lpthread -o $@

# compila objetos
%.o: %.c %.h


# exec

run_prio: test_prio
	./test_prio

run_queue: test_queue
	./test_queue

.PHONY: clean

clean:
	@ find . ../ -type f -name '*.o' -delete

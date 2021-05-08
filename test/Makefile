
CFLAGS = -Wall -Wextra -pedantic -O0 -g

test_queue: test_queue.o ../queue.o
	gcc test_queue.o ../queue.o -o test_queue

# compila objetos
%.o: %.c %.h


run_queue: test_queue
	./test_queue

.PHONY: clean

clean:
	@ find . ../ -type f -name '*.o' -delete

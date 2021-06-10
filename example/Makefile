

PROGRAM_NAME = resolver
DIR_LIB = ..

# get all files *.c
C_SOURCE = $(wildcard $(DIR_LIB)/*.c)

#get all file *.o
OBJECTS = $(C_SOURCE:.c=.o) main.o

CC = gcc
LDLIBS = -lpthread
CFLAGS = -Wall -Wextra -pedantic

ifdef DEBUG
	CFLAGS+= -g -O0
else
	CFLAGS+= -O2
endif

all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

# compila objetos
%.o: %.c %.h

.PHONY: clean

clean:
	@ find $(DIR_LIB) -type f -name '*.o' -delete

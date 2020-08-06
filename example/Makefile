

PROGRAM_NAME = resolver
DIR_LIB = ..

# get all files *.c
C_SOURCE = $(wildcard $(DIR_LIB)/*.c)
#
# #get all file *.o
OBJECTS = $(C_SOURCE:.c=.o) main.o

# OBJECTS= main.o \
# 				$(DIR_LIB)/domain.o \
# 				$(DIR_LIB)/service.o \
# 				$(DIR_LIB)/sock_util.o

LDLIBS = -lpthread

all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

# compila objetos
%.o: %.c %.h

.PHONY: clean

clean:
	@ find $(DIR_LIB) -type f -name '*.o' -delete

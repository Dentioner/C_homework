TARGET = lookup

all: $(TARGET)

CC = gcc
LD = gcc

CFLAGS = -g -Wall -Wno-unused-variable -Wno-unused-function -Iinclude
LDFLAGS = -L.

#LIBS = -lipstack -lpthread

HDRS = ./*.h

SRCS = main.c binary.c multipletrie.c poptrie.c
OBJS = $(patsubst %.c,%.o,$(SRCS))

$(OBJS) : %.o : %.c ./*.h
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET): $(LIBIP) $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(TARGET) 

clean:
	rm -f *.o $(TARGET)

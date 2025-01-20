CC=gcc
CFLAGS=-Wall -Wextra -g
SOURCES=src/main.c src/repl.c src/btree.c 
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=db

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
	

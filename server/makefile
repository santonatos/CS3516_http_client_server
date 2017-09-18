#Author:Spiros Antonatos 

PROGRAM = http_server
CC = g++
CFLAGS = -std=c++0x
DEPS = include/*.hpp
OBJ = server.o main.o
LFLAGS = -lpthread

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

main: $(OBJ)
	$(CC) $(CFLAGS) -o $(PROGRAM) $^ $(LFLAGS)
	rm -f *.o

clean:
	rm -f $(PROGRAM)


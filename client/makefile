#Author:Spiros Antonatos 

PROGRAM = http_client
CC = g++
CFLAGS = -std=c++0x
DEPS = include/*.hpp
OBJ = client.o main.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

main: $(OBJ)
	$(CC) $(CFLAGS) -o $(PROGRAM) $^
	rm -f *.o

clean:
	rm -f $(PROGRAM)

clean-txt:
	rm -f *.txt

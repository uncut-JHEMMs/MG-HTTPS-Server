CC 		:= g++
CFLAGS 	        := -I/usr/local/include -L/usr/local/lib -g
OBJECTS 	:=
LIBRARIES 	:= -lhttpserver

.PHONY: all clean

all: hello_world

hello_world: 
	$(CC) $(CFLAGS) -o hello_world hello_world.cpp $(LIBRARIES)

clean:
	rm -f *.o
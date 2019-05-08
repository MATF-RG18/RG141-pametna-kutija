PROGRAM    = brainBox
CC	   = gcc
CFLAGS     = -g -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDLIBS     = -lSOIL -lglut -lGLU -lGL -lm

$(PROGRAM): main.o 
	$(CC) $(CFLAGS) -o $(PROGRAM) main.o $(LDLIBS)
	
main.o: main.c lights.h SOIL.h
	$(CC) $(CFLAGS) -c main.c -o main.o $(LDLIBS)

.PHONY: clean 

clean: 
	-rm *.o $(PROGRAM) *core

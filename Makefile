PROGRAM    = brainBox
CC	   = gcc
CFLAGS     = -g -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDLIBS     = -lSOIL -lglut -lGLU -lGL -lm

$(PROGRAM): main.o lights.o
	$(CC) $(CFLAGS) -o $(PROGRAM) main.o lights.o $(LDLIBS)
	
main.o: main.c lights.h SOIL.h
	$(CC) $(CFLAGS) -c main.c -o main.o $(LDLIBS)
	
light.o: lights.c lights.h
	$(CC) $(CFLAGS) -c lights.c -o lights.o $(LDLIBS)

.PHONY: clean 

clean: 
	-rm *.o $(PROGRAM) *core

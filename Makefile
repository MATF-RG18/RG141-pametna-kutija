PROGRAM    = brainBox
CC	   = gcc
CFLAGS     = -g -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDLIBS     = -lSOIL -lglut -lGLU -lGL -lm

$(PROGRAM): main.o lights.o
	$(CC) $(CFLAGS) -o $(PROGRAM) main.o lights.o $(LDLIBS)
	
main.o: src/main.c header/lights.h header/SOIL.h
	$(CC) $(CFLAGS) -c src/main.c -o main.o $(LDLIBS)
	
lights.o: src/lights.c header/lights.h
	$(CC) $(CFLAGS) -c src/lights.c -o lights.o $(LDLIBS)

.PHONY: clean 

clean: 
	-rm *.o $(PROGRAM) *core

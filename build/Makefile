# makefile for the raytracer
# from quickcf tutorial:  g++ *.cpp -lSDL -O3 

CC = g++

vpath %.cpp ../source
vpath %.h ../source
#VPATH = ../source

OBJECTS = particleSystem.o sphFluid.o
HEADERS = openGL.h parameters.h math.h

FLAGS   = #-I.. -Wall -W -ansi -pedantic -lm -Dcimg_use_vt100
       #   -L/usr/X11R6/lib -lpthread -lX11 -lXext -lXrandr \
       #   -I/usr/X11R6/include -Dcimg_use_xshm -Dcimg_use_xrandr 

CCFLAGS = -c -g -O3 -fopenmp
CCFLAGS = -c -O3 -fopenmp -I.. -Wall -W -ansi -pedantic -Dcimg_use_vt100 -I/usr/X11R6/include
#CLFLAGS = -framework GLUT -framework OpenGL -fopenmp -L/usr/X11R6/lib -lpthread -lX11
CLFLAGS = -lglut -lGL -lGLU -fopenmp -L/usr/X11R6/lib -lpthread -lX11


main:   main.o $(OBJECTS) $(HEADERS)
	$(CC) main.o $(OBJECTS) $(CLFLAGS) -o main $(FLAGS)

main.o: openGL.h parameters.h

.cpp.o: 
	$(CC) $(CCFLAGS) $<

clean:
	rm $(OBJECTS) main main.o


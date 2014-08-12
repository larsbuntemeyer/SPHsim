# makefile for the raytracer
# from quickcf tutorial:  g++ *.cpp -lSDL -O3 

CC = g++

vpath %.cpp ../source
vpath %.h ../source
#VPATH = ../source

EXE = main

OBJECTS = particleSystem.o sphFluid.o
HEADERS = openGL.h parameters.h math.h

FLAGS   = #-I.. -Wall -W -ansi -pedantic -lm -Dcimg_use_vt100
       #   -L/usr/X11R6/lib -lpthread -lX11 -lXext -lXrandr \
       #   -I/usr/X11R6/include -Dcimg_use_xshm -Dcimg_use_xrandr 

CCFLAGS = -g -c -O3
LCFLAGS = -framework GLUT -framework OpenGL

main:   main.o $(OBJECTS)
	$(CC) main.o $(OBJECTS) $(LCFLAGS) -o $(EXE)
# 	$(CC) main.o $(OBJECTS) -lglut -lGL -lGLU -o main $(FLAGS)

sphFluid.o: particleSystem.o sphFluid.h

main.o: openGL.h

.cpp.o:
	$(CC) $(CCFLAGS) $<

clean:
	rm $(OBJECTS) $(EXE) $(EXE).o

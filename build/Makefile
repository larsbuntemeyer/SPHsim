# makefile for the raytracer
# from quickcf tutorial:  g++ *.cpp -lSDL -O3 

CC = g++

vpath %.cpp ../source
vpath %.h ../source
#VPATH = ../source

OBJECTS = particleSystem.o
HEADERS = openGL.h parameters.h math.h

FLAGS   = #-I.. -Wall -W -ansi -pedantic -lm -Dcimg_use_vt100
       #   -L/usr/X11R6/lib -lpthread -lX11 -lXext -lXrandr \
       #   -I/usr/X11R6/include -Dcimg_use_xshm -Dcimg_use_xrandr 

CCFLAGS = -framework GLUT -framework OpenGL

main:   main.o $(OBJECTS) $(HEADERS)
	$(CC) main.o $(OBJECTS) $(CCFLAGS) -o main $(FLAGS)
# 	$(CC) main.o $(OBJECTS) -lglut -lGL -lGLU -o main $(FLAGS)

.cpp.o: 
	$(CC) -g -c $<

clean:
	rm $(OBJECTS) main main.o

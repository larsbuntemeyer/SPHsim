#include <stdio.h>              /* I/O lib         ISOC  */
#include <stdlib.h>             /* Standard Lib    ISOC  */
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <omp.h>

using namespace std;

#include "parameters.h"
#include "openGL.h"
#include "particleSystem.h"
#include "sphFluid.h"


int main(int argc, char **argv)
{

   cout << "starting...\n";

   initGL(argc, argv);
   
   particleSystem pSystem(1000);
   sphFluid fluid(1000);
    
   glutMainLoop();
	
}

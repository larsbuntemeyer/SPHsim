#include <stdio.h>              /* I/O lib         ISOC  */
#include <stdlib.h>             /* Standard Lib    ISOC  */
#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <cmath>
#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <omp.h>
#include "CImg.h"

using namespace std;

#include "parameters.h"
#include "openGL.h"
#include "particleSystem.h"
#include "sphFluid.h"
#include "math.h"


Vec2 mousePosition;
particleSystem pSystem(1000);
int nStep = 0;
int iFile = 0;

// Between [0,1]
float rand01() { return (float)rand() * (1.f / RAND_MAX); }
// Between [a,b]
float randab(float a, float b) { return a + (b-a)*rand01(); }

string stringify(int x) {
    std::ostringstream o;
    o << setfill('0') << setw(4) << x;
    return o.str();
} 

void render()
{
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    //glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClear( GL_COLOR_BUFFER_BIT );
    glLoadIdentity();
    pSystem.draw();
    glutSwapBuffers();
}
//
void keyboard(unsigned char c, __attribute__((unused)) int x, __attribute__((unused))  int y)
{
}
//
void motion(int x, int y)
{
    static int notInit;
    static int oldX;
    static int oldY;
    if(notInit) {
       // printf("MOTION: (%d,%d) -> (%d,%d)\n", oldX, oldY, x, y);
        oldX = x;
        oldY = y;
    } else {
       // printf("MOTION: (%d,%d)\n", x, y);
        oldX = x;
        oldY = y;
        notInit = 1;
    } /* end if/else */
    
	// This simply updates the location of the mouse position
    // in the simulation space.
	float relx = (float)(x - window_w/2) / window_w;
	float rely = -(float)(y - window_h) / window_h;
	Vec2 current = Vec2(relx*worldSize*2.f, rely*worldSize*2.f);
	mousePosition = current;
    pSystem.addParticle(mousePosition);
}
//
void drawPoint(int x, int y) {
    x = x - 250;
    y = 250-y;
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0,0.0,1.0);
    glPointSize(10);
    glBegin(GL_POINTS);
    glVertex2f(x , y);
    glEnd();
    glFlush();
}
void mouse(int button,int state,int x,int y)
{
    char*b, *m;
    int kbMod;
    
    if       (button == GLUT_LEFT_BUTTON)   {  /* MacOS X: Ctr-Click */
        b = "left button";
    } else if(button == GLUT_MIDDLE_BUTTON) {  /* MacOS X: Opt-Click */
        b = "middle button";
    } else if(button == GLUT_RIGHT_BUTTON)  {  /* MacOS X: Click */
        b = "right button";
    } else {
        b = "unknown";
    } /* end if/else */
    
    kbMod = glutGetModifiers();
    if(kbMod == GLUT_ACTIVE_SHIFT) {
        m = "SHIFT";
    } else if(kbMod == GLUT_ACTIVE_CTRL) {
        m = "CTRL";
    } else if(kbMod == GLUT_ACTIVE_ALT) {
        m = "ALR";
    } else {
        m = "NONE";
    } /* end if/else */
    
    if(state == GLUT_DOWN) {
        printf("Mouse Depress: b(%s/%d/%s)@(%d,%d)\n", b, button, m, x, y);
	    float relx = (float)(x - window_w/2) / window_w;
     	float rely = -(float)(y - window_h) / window_h;
	    Vec2 current = Vec2(relx*worldSize*2.f, rely*worldSize*2.f);
	    mousePosition = current;
        pSystem.addParticle(mousePosition);
    } else if(state == GLUT_UP) {
        printf("Mouse Release: b(%s/%d/%s)@(%d,%d)\n", b, button, m, x, y);
    } else {
        printf("Unknown Mouse Click Event: b(%d/%s)@(%d,%d)\n", button, m, x, y);
    } /* end if/else */
}
//
void menuCall(int value) {
    printf("MENU: %d\n", value);
    if       (value == 20) {
        eangle = 0;
    } else if(value == 21) {
    } else if(value == 10) {
        glutPostRedisplay();
    } else if(value == 11) {
        glutDestroyWindow(mainWindow);
        exit(0);
    } else if(value == 0) {
        printf("Hello to you too!!");
    } /* end if/else */
} /* end func menuCall */

void buildMenu() {
    int subMenu1, subMenu2;
    int mainMenu;
    
    subMenu1 = glutCreateMenu(menuCall);
    glutAddMenuEntry("Redraw", 10);
    glutAddMenuEntry("Quit",   11);
    
    subMenu2 = glutCreateMenu(menuCall);
    glutAddMenuEntry("Reset Angle", 20);
    glutAddMenuEntry("Reset Size",  21);
    
    mainMenu = glutCreateMenu(menuCall);
    glutAddMenuEntry("Hello", 0);
    glutAddSubMenu("Reset", subMenu2);
    glutAddSubMenu("System", subMenu1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
} /* end func buildMenu */


void renderParticles(){
    string file = filename + stringify(iFile) + ".png";
    cimg_library::CImg<unsigned char>image(window_w,window_h,1,3);
    image.fill(255);
    printf("-------------------------------------------------\n");
    printf("starting rendering...\n");
    char *name = new char[file.length()+1];
    vector<particle> particles = pSystem.getParticles();
    float r = pSystem.getSpacing();
    for (int i=0; i<(int)particles.size(); i++) {
        float x = particles[i].pos.x;
        float y = particles[i].pos.y;
        float relX = (float)(0.5*(x- (-worldSize))/worldSize);
        float relY = (float)(0.5*(y-bottom)/worldSize);
        int pixelX;
        int pixelY;
        pixelX = static_cast<int>(relX * window_w);
        pixelY = window_h - static_cast<int>(relY * window_h);
       // printf("mapping particle pos %f, %f \n",x,y);
       // printf("to pixel pos %i, %i \n",pixelX,pixelY);
       // printf("relX,relY, %f, %f \n",relX,relY);
       // printf("test %i \n",int(0.528750*580));
        image.draw_circle(pixelX,pixelY,2.0*r,particleColor,1.0);
    }
    strcpy(name, file.c_str());
    image.save(name);
    printf("finished rendering to file %s \n",name);
    //image.display();
    iFile++;
}

void idle()
{
    pSystem.advance(timestep);
    if (nStep%plotIntervall==0) {
     renderParticles();
    }
    render();
    nStep++;
}

void initGL(int argc, char **argv){
    //   glClearColor (0.0, 0.0, 0.0, 0.0);
    //   glMatrixMode(GL_MODELVIEW);
    //   glLoadIdentity();
    //   glFlush();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
    glutInitWindowSize(window_w, window_h);
    
    // glutCreateWindow("SPH");
    mainWindow = glutCreateWindow("Particle Sandbox");
    
    glutDisplayFunc(render);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    buildMenu();

	// create a world with dimensions x:[-SIM_W,SIM_W] and y:[0,SIM_W*2]
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-worldSize,worldSize,0,2*worldSize);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPointSize(5.f);
}

int main(int argc, char **argv)
{

    initGL(argc, argv);
  
    pSystem.setNumberOfParticles(numberOfParticles);
    pSystem.setWorldSize(worldSize);
    pSystem.setBottom(bottom);
    pSystem.addGlobalForce(Vec2(0,-G));
    pSystem.setLifetime(lifetime);
    pSystem.killParticles(kill);
    pSystem.setSetup(setup);

    glutMainLoop();
	
}

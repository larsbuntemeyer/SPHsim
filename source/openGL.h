//
//  openGL.h
//  SPHsim
//
//  Created by Lars on 02.08.14.
//  Copyright (c) 2014 Lars. All rights reserved.
//

#ifndef SPHsim_openGL_h
#define SPHsim_openGL_h

void render()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    glEnd();
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
        printf("MOTION: (%d,%d) -> (%d,%d)\n", oldX, oldY, x, y);
        oldX = x;
        oldY = y;
    } else {
        printf("MOTION: (%d,%d)\n", x, y);
        oldX = x;
        oldY = y;
        notInit = 1;
    } /* end if/else */
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
    } else if(state == GLUT_UP) {
        printf("Mouse Release: b(%s/%d/%s)@(%d,%d)\n", b, button, m, x, y);
    } else {
        printf("Unknown Mouse Click Event: b(%d/%s)@(%d,%d)\n", button, m, x, y);
    } /* end if/else */
}
//
void idle()
{
}

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

void initGL(int argc, char **argv){
    //   glClearColor (0.0, 0.0, 0.0, 0.0);
    //   glMatrixMode(GL_MODELVIEW);
    //   glLoadIdentity();
    //   glFlush();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
    glutInitWindowSize(window_w, window_h);
    // glutCreateWindow("SPH");
    mainWindow = glutCreateWindow("SPH");
    
    glutDisplayFunc(render);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    buildMenu();
}
#endif
#include <windows.h>
#include <iostream>
#include <GL/glut.h>

using namespace std;

void init(){
    glClearColor(1.0,1.0,1.0,1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,640,0,480);
}

void dda_line(int x1, int y1, int x2, int y2){
    int dx = x2 - x1; int dy = y2 - y1;
    int steps = 1, length,x = x1,y = y1;
    length = max(abs(dx),abs(dy));
    int xinc = dx/length, yinc = dy/length;
    while(steps<=length){
        glBegin(GL_POINTS);
            glVertex2d(x, y);
        glEnd();
        x += xinc; y += yinc;
        steps++;
    }
    glFlush();
}

void world(){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0,0,0);
    glPointSize(4.0);
    dda_line(20,20,250,250);
}

int main(int argc, char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(200,200);
    glutInitWindowSize(640,480);
    glutCreateWindow("End Sem Yahoo");
    glutDisplayFunc(world);
    init();
    glutMainLoop();
    return 1;
}


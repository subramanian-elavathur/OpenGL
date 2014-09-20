#include <iostream>
#include <math.h>
#include <time.h>
#include <GL/glut.h>

using namespace std;

void delay(float ms){
    clock_t goal = ms + clock();
    while(goal>clock());
}

void init(){
    glClearColor(0.0,0.0,0.0,0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,640,0,480);
}

void drawDot (GLint x, GLint y, GLfloat r, GLfloat g, GLfloat b)
{
    glColor3f(r,g,b);
    glPointSize(1.5);
    glBegin (GL_POINTS);
        glVertex2i (x,y);
    glEnd();
}

void symmetricPixels (int x, int y, int xc, int yc, float r, float g, float b)
{   drawDot (xc + x, yc + y, r,g,b);
    drawDot (xc - x, yc + y,r,g,b);
    drawDot (xc + x, yc - y,r,g,b);
    drawDot (xc - x, yc - y,r,g,b);
}

void Ellipse (int a, int b, int xc, int yc, float r, float g, float bl)
{
    int aSq,bSq,twoASq,twoBSq,d,dx,dy,x,y;
    aSq = a*a;
    bSq = b*b;
    twoASq = 2*aSq;
    twoBSq = 2*bSq;
    d = bSq - b*aSq + aSq/4;
    dx = 0;
    dy = twoASq*b;
    x = 0;
    y = b;
    symmetricPixels(x,y,xc,yc,r,g,bl);
    while (dx < dy)
    {
        x++;
        dx += twoBSq;
        if (d >= 0)
        {
            y--;
            dy -= twoASq;
        }
        if (d < 0)
            d += bSq + dx;
        else
            d += bSq + dx - dy;
        symmetricPixels (x,y,xc,yc,r,g,bl);
    }
    d = (int)(bSq*(x+0.5)*(x+0.5) + aSq*(y-1)*(y-1) - aSq*bSq);
    while (y > 0)
    {   y--;
        dy -= twoASq;
        if (d <= 0)
        {
            x++;
            dx += twoBSq;
        }
        if (d > 0)
            d += aSq - dy;
        else
            d += aSq -dy +dx;
        symmetricPixels(x,y,xc,yc,r,g,bl);
    }
    glFlush();
}

void ellipsy(){
    glClear(GL_COLOR_BUFFER_BIT);
}

void gravity_on(int x, int y){
    int yinit = y, xinit = x;
    int count = 0; int flag = 0;
    float factor=1.0; int q;
    while(factor!=0.0){
        if(count==10)
            return;
        for(q=yinit*factor;q>=50;q--){
            glClear(GL_COLOR_BUFFER_BIT);
            Ellipse (50,50,x,q,0,0,1);
            if(q%6==0)
                x++;
            if(flag!=0)
                delay(1);
        }

        for(int y=50;y>40;y--){
            glClear(GL_COLOR_BUFFER_BIT);
            Ellipse (50,y,x,y,0,0,1);
            delay(1);
        }
        for(int y=40;y<50;y++){
            glClear(GL_COLOR_BUFFER_BIT);
            Ellipse (50,y,x,y,0,0,1);
            delay(1);
        }

        factor = factor - 0.1;
        for(q=50;q<=yinit*factor;q++)
        {
            glClear(GL_COLOR_BUFFER_BIT);
            Ellipse (50,50,x,q,0,0,1);
            if(q%6==0)
                 x++;
            delay(1);
        }
        for(int z=0; z<8; z++)
        {
            glClear(GL_COLOR_BUFFER_BIT);
            Ellipse (50,50,x,q,0,0,1);
            delay(1);
        }
        count++;
    }
    cout<<"done";
}

void mouse(int btn, int state, int x, int y){
    y = 480-y;
    if(btn==GLUT_LEFT_BUTTON)
    {
        if(state==GLUT_DOWN)
        {
            Ellipse (50,50,x,y,0,0,1);
            gravity_on(x,y);
        }
    }
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(200,200);
    glutCreateWindow("Many Amaze Very GL WOW");
    glutDisplayFunc(ellipsy);
    glutMouseFunc(mouse);
    init();
    glutMainLoop();
    return 0;
}

#include <windows.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include <GL/glut.h>

using namespace std;

void delay(unsigned int ms){
    clock_t goal = ms + clock();
    while(goal>clock());
}

void init(){
    glClearColor(1.0,1.0,1.0,0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-320,320,-240,240);
}

void middy_circle(float r, float xc, float yc){
    float blue = 0.01;
    glPointSize(3.0);
    float d = 1-r;
    float x = 0, y = r;
    glBegin(GL_POINTS);
        glVertex2i(round(x+xc), round(y+yc));
    glEnd();
    glFlush();
    glBegin(GL_POINTS);
        while(y>x){
            if(d<0)
            {
                d+=((2*x)+3);
                x++;
            }
            else if(d>=0)
            {
                d+=(2*(x-y))+5;
                x++;
                y--;
            }
            glVertex2i(round(x+xc), round(y+yc));
            glVertex2i(round(y+xc), round(x+yc));
            glVertex2i(round(x+xc), round(-y+yc));
            glVertex2i(round(y+xc), round(-x+yc));
            glColor3f(0.0,blue,0.0);
            glVertex2i(round(-x+xc), round(-y+yc));
            glVertex2i(round(-y+xc), round(-x+yc));
            glVertex2i(round(-x+xc), round(y+yc));
            glVertex2i(round(-y+xc), round(x+yc));
            glColor3f(0.0,0.0,blue);
            blue = blue + 0.01;
            if(blue==1.0)
                blue = 0.01;
        }
        glEnd();
        glFlush();
}

void circly(){
    int flager=0;
    for(float x=-160;x<=160;)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        middy_circle(160.0,x,0.0);
        if(flager==0)
            x++;
        else
            x--;
        if(x>=160)
            flager=1;
        if(x<=-160)
            flager=0;
            delay(2);
    }
}


void drawDot (GLint x, GLint y, GLfloat r, GLfloat g, GLfloat b)
{ glColor3f(r,g,b);
glPointSize(3.0);
  glBegin (GL_POINTS);
	  glVertex2i (x,y);
  glEnd();
}

void symmetricPixels (int x, int y, int xc, int yc, float r, float g, float b)
{ drawDot (xc + x, yc + y, r,g,b);
  drawDot (xc - x, yc + y,r,g,b);
  drawDot (xc + x, yc - y,r,g,b);
  drawDot (xc - x, yc - y,r,g,b);
}

void Ellipse (int a, int b, int xc, int yc, float r, float g, float bl)
{ int aSq,bSq,twoASq,twoBSq,d,dx,dy,x,y;
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
  { x++;
    dx += twoBSq;
    if (d >= 0)
    { y--;
      dy -= twoASq;
    }
    if (d < 0)
     d += bSq + dx;
    else
     d += bSq + dx - dy;
    symmetricPixels (x,y,xc,yc,r,g,bl);
  }
  d = (int)(bSq*(x+0.5)*(x+0.5) + aSq*(y-1)*(y-1) -
                 aSq*bSq);
  while (y > 0)
  { y--;
    dy -= twoASq;
    if (d <= 0)
    { x++;
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
    Ellipse (100,50,0,0,0,1,1);
    int flager=0;
    float blue = 0.0;
    for(int x=0;x<=221;)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        Ellipse (100,70,x,0,0,0,blue);
        if(flager==0)
            x++;
        else
            x--;
        if(x>=220){
            flager=1;
            for(int y=100;y>0;y--){
                glClear(GL_COLOR_BUFFER_BIT);
                Ellipse (y,70,x+(100-y),0,0,0,blue);
                delay(1);
            }
            x = 320;
            for(int y=0;y<100;y++){
                glClear(GL_COLOR_BUFFER_BIT);
                Ellipse (y,70,x-y,0,0,0,blue);
                delay(1);
            }
            x=220;
        }
        if(x<=-220){
            flager=0;
            for(int y=100;y>0;y--){
                glClear(GL_COLOR_BUFFER_BIT);
                Ellipse (y,70,x-(100-y),0,0,0,blue);
                delay(2);
            }
            x = -320;
            for(int y=0;y<100;y++){
                glClear(GL_COLOR_BUFFER_BIT);
                Ellipse (y,70,x+y,0,0,0,blue);
                delay(2);
            }
            x=-220;
        }
        blue += 0.001;
        if(blue>=1)
            blue = 0.000;
        delay(1);
    }
}

void launcher(int a, int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(200,200);
    glutCreateWindow("Many Amaze Very GL WOW");
    switch(a){
        case 0: glutDisplayFunc(circly);break;
        case 1: glutDisplayFunc(ellipsy);break;
    }
    init();
    glutMainLoop();
}

int main(int argc, char** argv){
    int choice;
    cout<<"\nWow Launcher Much Amaze\n1. Mid Point Circle\n2. Mid Point Ellipse\n3. Arc\n4. Exit\n Enter choice - ";
    cin>>choice;
    switch(choice){
        case 1: cout<<"\nLaunching now\n";launcher(0,argc,argv);break;
        case 2: cout<<"\nLaunching now\n";launcher(1,argc,argv);break;
        case 3: cout<<"\nLaunching now\n";launcher(2,argc,argv);break;
        case 4: return 0;break;
        default: cout<<"\nInvalid Option\n";break;
    }
    return 0;
}

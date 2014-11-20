#include <iostream>
#include <math.h>
#include <GL/glut.h>

using namespace std;

void init(){
    glClearColor(1.0,1.0,1.0,0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,320,0,240);
}

void bresenham_line(float x1, float y1, float x2, float y2){
    glPointSize(2.0);
    int flag = 0, flag1 = 0;
    float m = ((y2-y1)/(x2-x1));
    if(m>0 && m<=1)
        flag=1;
    if(m>1)
        flag=2;
    if(m<0&&abs(m)<=1)
        flag=3;
    if(m<-1)
        flag=4;
    // vars
    float dx=x2-x1; float dy=y2-y1;
    float dinit, d;
    float x = x1; float y = y1;
    glBegin(GL_POINTS);
    cout<<flag;
    switch(flag){
        case 1: while(x<=x2)
                {
                    if(flag1==0)
                    {
                        dinit=(2*dy)-dx;
                        d=dinit;
                        glVertex2i(round(x), round(y));
                        flag1=1;
                    }
                    else
                    {
                        if(d<=0)
                        {
                            glVertex2i(round(x), round(y));
                            x++;
                            d = d+(2*dy);
                        }
                        else
                        {
                            glVertex2i(round(x), round(y));
                            x++;
                            y++;
                            d = d + (2*(dy-dx));
                        }
                    }
                }break;
        case 2: while(x<=x2)
                {
                    if(flag1==0)
                    {
                        dinit=(2*dx)-dy;
                        d=dinit;
                        glVertex2i(round(x), round(y));
                        flag1=1;
                    }
                    else
                    {
                        if(d<=0)
                        {
                            glVertex2i(round(x), round(y));
                            y++;
                            d+=(2*dx);
                        }
                        else
                        {
                            glVertex2i(round(x), round(y));
                            x++;
                            y++;
                            d+=2*(dx-dy);
                        }
                    }
                }break;
        case 3: while(x<=x2)
                {
                    if(flag1==0)
                    {
                        dinit=-(2*dy)-dx;
                        d=dinit;
                        glVertex2i(round(x), round(y));
                        flag1=1;
                    }
                    else
                    {
                        if(d<=0)
                        {
                            glVertex2i(round(x), round(y));
                            x++;
                            d-=(2*dy);
                        }
                        else
                        {
                            glVertex2i(round(x), round(y));
                            x++;
                            y--;
                            d = d-(2*(dy+dx));
                        }
                    }
                }break;
        case 4: while(x<=x2)
                {
                    if(flag1==0)
                    {
                        dinit=(2*x)+dy;
                        d=dinit;
                        glVertex2i(round(x), round(y));
                        flag1=1;
                    }
                    else
                    {
                        if(d<=0)
                        {
                            glVertex2i(round(x), round(y));
                            y--;
                            d+=(2*dx);
                        }
                        else
                        {
                           glVertex2i(round(x), round(y));
                           x++;
                           y++;
                           d+=2*(dy+dx);
                        }
                    }
                }break;
    }
    glEnd();
    glFlush();
}

void bresenham_spoke(){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0,0.0,1.0);
    bresenham_line(100,50,105,150);
    bresenham_line(50,100,150,105);
    glColor3f(1.0,0.0,0.0);
    bresenham_line(68,140,138,70);
    bresenham_line(68,70,138,140);
    glColor3f(0.0,1.0,0.0);
    bresenham_line(56,85,144,120);
    bresenham_line(56,120,144,85);
}

void dda_line(int x1, int y1, int x2, int y2){
    float blue = 0.01;
    glPointSize(15.0);
    float dy = y2-y1; float dx = x2-x1;
    int steps = 1, length;
    if(abs(dy)>abs(dx))
        length = abs(dy);
    else
        length = abs(dx);
    float x = x1; float y = y1;
    float xinc = dx/length; float yinc = dy/length;
    glBegin(GL_POINTS);
        while(steps<length){
            glVertex2i(round(x), round(y));
            x += xinc;
            y += yinc;
            steps++;
            glColor3f(0.0,0.0,blue);
            blue = blue + 0.01;
            if(blue==1.0)
                blue = 0.01;
        }
    glEnd();
    glFlush();
}

void dda_star_draw(){
    glClear(GL_COLOR_BUFFER_BIT);
    dda_line(37,70,100,70);
    dda_line(100,70,120,10);
    dda_line(120,10,140,70);
    dda_line(140,70,203,70);
    dda_line(203,70,153,108);
    dda_line(153,108,174,175);
    dda_line(174,175,120,140);
    dda_line(120,140,66,175);
    dda_line(66,175,87,108);
    dda_line(87,108,37,70);
}

void launcher(int a, int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(200,200);
    glutCreateWindow("Many Amaze Very GL WOW");
    switch(a){
        case 0: glutDisplayFunc(dda_star_draw);break;
        case 1: glutDisplayFunc(bresenham_spoke);break;
    }
    init();
    glutMainLoop();
}

int main(int argc, char** argv){
    int choice;
    cout<<"\nWow Launcher Much Amaze\n1. DDA Line\n2. Bresenham Line\n3. Exit\nEnter choice - ";
    cin>>choice;
    switch(choice){
        case 1: cout<<"\nLaunching now\n";launcher(0,argc,argv);break;
        case 2: cout<<"\nLaunching now\n";launcher(1,argc,argv);break;
        case 3: return 0;break;
        default: cout<<"\nInvalid Option\n";break;
    }
    return 0;
}

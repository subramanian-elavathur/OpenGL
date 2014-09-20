#include <iostream>
#include <math.h>
#include <time.h>
#include <GL/glut.h>

using namespace std;

int r = 200;
int c = 10;
int** points;

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
    float d = 1-r;
    float x = 0, y = r;
    glPointSize(2);
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

    for(int i=0;i<c+1;i++)
    {
        delay(10);
        for(int j=0;j<c+1;j++)
        {
            glColor3f(0.0,1.0,0.3);
            glBegin(GL_LINES);
                glVertex2i(points[i][0],points[i][1]);
                glVertex2i(points[j][0],points[j][1]);
            glEnd();
            glFlush();
            glBegin(GL_LINES);
                glVertex2i(points[i][0],points[i][1]);
                glVertex2i(-points[j][0],points[j][1]);
            glEnd();
            glFlush();
            glBegin(GL_LINES);
                glVertex2i(points[i][0],points[i][1]);
                glVertex2i(points[j][0],-points[j][1]);
            glEnd();
            glFlush();
            glBegin(GL_LINES);
                glVertex2i(points[i][0],points[i][1]);
                glVertex2i(-points[j][0],-points[j][1]);
            glEnd();
            glFlush();
            //glColor3f(0.6,1.0,0.2);
            glBegin(GL_LINES);
                glVertex2i(-points[i][0],-points[i][1]);
                glVertex2i(points[j][0],-points[j][1]);
            glEnd();
            glFlush();
            glBegin(GL_LINES);
                glVertex2i(-points[i][0],-points[i][1]);
                glVertex2i(-points[j][0],points[j][1]);
            glEnd();
            glFlush();
            glBegin(GL_LINES);
                glVertex2i(-points[i][0],-points[i][1]);
                glVertex2i(-points[j][0],-points[j][1]);
            glEnd();
            glFlush();
            //glColor3f(0.8,1.0,0.4);
            glBegin(GL_LINES);
                glVertex2i(-points[i][0],points[i][1]);
                glVertex2i(points[j][0],-points[j][1]);
            glEnd();
            glFlush();
            glBegin(GL_LINES);
                glVertex2i(-points[i][0],points[i][1]);
                glVertex2i(points[j][0],points[j][1]);
            glEnd();
            glFlush();
            //glColor3f(1.0,1.0,0.7);
            glBegin(GL_LINES);
                glVertex2i(points[i][0],-points[i][1]);
                glVertex2i(points[j][0],-points[j][1]);
            glEnd();
            glFlush();
        }
    }
}

void patternizer(){
    glClear(GL_COLOR_BUFFER_BIT);
    middy_circle(r,0,0);
}

void point_gen(int z){
    points = new int*[c+1];
    for(int i=0;i<c+1;i++)
        points[i] = new int[2];
    points[0][0] = 0;
    points[0][1] = r;
    float deginc = 90/z;
    float x = 0, y = r;
    for(int i=1;i<=z;i++){
        x = sqrtf((r*r)/(1+(tan(deginc)*tan(deginc))));
        y = sqrtf((r*r)-(x*x));
        points[i][0] = x;
        points[i][1] = y;
        deginc+=deginc;
    }
    //for(int i=0;i<c+1;i++)
    //   cout<<points[i][0]<<" "<<points[i][1]<<endl;
}


int main(int argc, char** argv){
    point_gen(c);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(200,200);
    glutCreateWindow("Many Amaze Very GL WOW");
    glutDisplayFunc(patternizer);
    init();
    glutMainLoop();
    return 0;
}

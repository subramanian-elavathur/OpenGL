#include <iostream>
#include <windows.h>
#include <math.h>
#include <GL/glut.h>
#include <vector>

#define CUBE 8
#define SPHERE 100

using namespace std;

//  GLOBALS

float** pts, **origin;
float R,G,B;

int sz = CUBE;

float X1,Y1,z1,x2,y2,z2;

// input start flag

int in_start=0;

// translate flags

int tr_start=0, tr_active=0;

// rotation flags

int rot = 1;


//  END OF GLOBALS






//  OGL INITS


void init(){
    glClearColor(1.0,1.0,1.0,1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,640,0,480);
}



// END OGL INITS






// HELPERS

class arr{
    public:
        vector<float>* cols;
        vector< vector<float> > rows;
        arr(int m, int n, float arr[]){
            cols = new vector<float>[m];
            for(int i=0; i<m; i++)
                rows.push_back(cols[i]);
            int count = 0;
            for(int i=0; i<m; i++)
                for(int j=0; j<n; j++){
                    rows[i].push_back(arr[count++]);
                }
        }
        arr(int m, int n){
            cols = new vector<float>[m];
            for(int i=0; i<m; i++)
                rows.push_back(cols[i]);
        }
};



arr* mult(arr* x, arr* y){
    int i,j,k;
    int r1 = x->rows.size(), c1 = x->rows[0].size(), r2 = y->rows.size(), c2 = y->rows[0].size();
    arr* answ = new arr(r1,c2);
    float** ans = new float*[r1];
    for(i=0; i<r1; i++)
        ans[i] = new float[c2];
    for(i=0; i<r1; i++)
        for(j=0; j<c2; ++j)
            ans[i][j] = 0.0;
    for(i=0; i<r1; ++i)
        for(j=0; j<c2; ++j){
            for(k=0; k<c1; ++k)
                ans[i][j]+=x->rows[i][k]*y->rows[k][j];
            answ->rows[i].push_back(ans[i][j]);
        }
    return answ;
}


// END OF HELPERS

// TRANSFORMS

void draw();

void oblique_it(){
    float tx = cos(30)/tan(63.4);
    float tt = sin(30)/tan(63.4);
    float tsup[] = {1,0,tx,0, 0,1,tt,0, 0,0,0,0 ,0,0,0,1};
    arr* tmatrix = new arr(4, 4, tsup);
    //begin magic
    for(int i=0; i<sz; i++){
        float temp[] = {origin[i][0],origin[i][1],origin[i][2],1};
        arr* mtemp = new arr(4, 1,temp);
        arr* atemp;
        atemp = mult(tmatrix,mtemp);
        pts[i][0]  = atemp->rows[0][0];
        if(i<4)
            pts[i][1]  = atemp->rows[1][0];
        else
            pts[i][1]  = atemp->rows[1][0];
        pts[i][2]  = atemp->rows[2][0];
    }
}

void oblique_it2(){
    float tx = cos(30)/tan(45);
    float tt = sin(30)/tan(45);
    float tsup[] = {1,0,tx,0, 0,1,tt,0, 0,0,0,0 ,0,0,0,1};
    arr* tmatrix = new arr(4, 4, tsup);
    //begin magic
    for(int i=0; i<sz; i++){
        float temp[] = {origin[i][0],origin[i][1],origin[i][2],1};
        arr* mtemp = new arr(4, 1,temp);
        arr* atemp;
        atemp = mult(tmatrix,mtemp);
        pts[i][0]  = atemp->rows[0][0];
        if(i<4)
            pts[i][1]  = atemp->rows[1][0];
        else
            pts[i][1]  = atemp->rows[1][0];
        pts[i][2]  = atemp->rows[2][0];
    }
}

void translate(int tx, int ty, int flush){
    float tsup[] = {1,0,0,tx ,0,1,0,ty ,0,0,0,0 ,0,0,0,1};
    arr* tmatrix = new arr(4, 4, tsup);
    //begin magic
    for(int i=0; i<sz; i++){
        float temp[] = {origin[i][0],origin[i][1],origin[i][2],1};
        arr* mtemp = new arr(4, 1,temp);
        arr* atemp;
        atemp = mult(tmatrix,mtemp);
        origin[i][0]  = atemp->rows[0][0];
        origin[i][1]  = atemp->rows[1][0];
    }
    oblique_it();
    if(flush)
        draw();
}

void rz(float deg){
    int cs = cos(deg);
    int sn = sin(deg);
    float tsup[] = {cs,-sn,0,0 ,sn,cs,0,0 ,0,0,1,0 ,0,0,0,1};
    translate(-origin[7][0],-origin[7][1],0);
    arr* tmatrix = new arr(4, 4, tsup);
    //begin magic
    for(int i=0; i<sz; i++){
        float temp[] = {origin[i][0],origin[i][1],origin[i][2],1};
        arr* mtemp = new arr(4, 1,temp);
        arr* atemp;
        atemp = mult(tmatrix,mtemp);
        origin[i][0]  = atemp->rows[0][0];
        origin[i][1]  = atemp->rows[1][0];
        origin[i][2]  = atemp->rows[2][0];
    }
    translate(origin[7][0],origin[7][1],0);
    oblique_it();
    draw();
}


// END OF TRANSFORMS


// RENDER AND COMPUTE FUNCTIONS

void rgb_set();
void rgb(float r, float g, float b);

void draw(){
    cout<<endl<<endl<<endl;
    cout<<"X1 - "<<X1<<" Y1 - "<<Y1<<" Z1 - "<<z1<<" X2 - "<<x2<<" Y2 - "<<y2<<" Z2 - "<<z2<<endl;
    glClear(GL_COLOR_BUFFER_BIT);
    //glColor3f(1,0,0);
    rgb(155.0, 89.0, 182.0);
    glBegin(GL_LINE_LOOP);
        for(int i = 0; i<4; i++){
            glVertex2f(pts[i][0],pts[i][1]);
            cout<<endl<<pts[i][0]<<" "<<pts[i][1];
        }
    glEnd();
    //glColor3f(0,1,0);
    rgb(241.0, 196.0, 15.0);
    glBegin(GL_LINE_LOOP);
        for(int i = 4; i<8; i++){
            glVertex2f(pts[i][0],pts[i][1]);
            cout<<endl<<pts[i][0]<<" "<<pts[i][1];
        }
    glEnd();
    //glColor3f(0,0,1);
    rgb(26.0, 188.0, 156.0);
    glBegin(GL_LINES);
        glVertex2f(pts[0][0],pts[0][1]);
        glVertex2f(pts[4][0],pts[4][1]);
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(pts[1][0],pts[1][1]);
        glVertex2f(pts[5][0],pts[5][1]);
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(pts[2][0],pts[2][1]);
        glVertex2f(pts[6][0],pts[6][1]);
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(pts[3][0],pts[3][1]);
        glVertex2f(pts[7][0],pts[7][1]);
    glEnd();
    glFlush();
    //rgb_set();
    glPointSize(5);
    glBegin(GL_POINTS);
        glVertex2f(pts[7][0],pts[7][1]);
    glEnd();
    glFlush();
    glPointSize(1);
}

void compute_and_set(){
    pts = new float*[sz];
    for(int i=0; i<sz; i++){
        pts[i] = new float[3];
    }
    origin = new float*[sz];
    for(int i=0; i<sz; i++){
        origin[i] = new float[3];
    }

    pts[0][0] = X1; pts[0][1] = Y1; pts[0][2] = z1;

    pts[1][0] = x2; pts[1][1] = Y1; pts[1][2] = z1;

    pts[2][0] = x2; pts[2][1] = y2; pts[2][2] = z1;

    pts[3][0] = X1; pts[3][1] = y2; pts[3][2] = z1;

    pts[4][0] = X1; pts[4][1] = Y1; pts[4][2] = z2;

    pts[5][0] = x2; pts[5][1] = Y1; pts[5][2] = z2;

    pts[6][0] = x2; pts[6][1] = y2; pts[6][2] = z2;

    pts[7][0] = X1; pts[7][1] = y2; pts[7][2] = z2;

    origin[0][0] = X1; origin[0][1] = Y1; origin[0][2] = z1;

    origin[1][0] = x2; origin[1][1] = Y1; origin[1][2] = z1;

    origin[2][0] = x2; origin[2][1] = y2; origin[2][2] = z1;

    origin[3][0] = X1; origin[3][1] = y2; origin[3][2] = z1;

    origin[4][0] = X1; origin[4][1] = Y1; origin[4][2] = z2;

    origin[5][0] = x2; origin[5][1] = Y1; origin[5][2] = z2;

    origin[6][0] = x2; origin[6][1] = y2; origin[6][2] = z2;

    origin[7][0] = X1; origin[7][1] = y2; origin[7][2] = z2;

    oblique_it();

    draw();

}

void redraw(){

}


// END OF RENDER FUNCTIONS



// COLORING FUNCTIONS

void rgb(float r, float g, float b){
    R = (r * 3.92)/1000;
    G = (g * 3.92)/1000;
    B = (b * 3.92)/1000;
    glColor3f(R,G,B);
    glBegin(GL_POLYGON);
        glVertex2i(0,0);
        glVertex2i(20,0);
        glVertex2i(20,20);
        glVertex2i(0,20);
    glEnd();
    glFlush();
}

void rgb_set(){
    glColor3f(R,G,B);
    glBegin(GL_POLYGON);
        glVertex2i(0,0);
        glVertex2i(20,0);
        glVertex2i(20,20);
        glVertex2i(0,20);
    glEnd();
    glFlush();
    rgb(26.0, 188.0, 156.0);
}

//END COLORING FUNCTIONS



// GLUT ACTION CALLBACKS


void key(unsigned char key_t, int x, int y){
    if(key_t<=57 && key_t>=48){
        int val = key_t - 48;
        switch(val){
            case 0: rgb(26.0, 188.0, 156.0);break;
            case 1: rgb(46.0, 204.0, 113.0);break;
            case 2: rgb(52.0, 152.0, 219.0);break;
            case 3: rgb(155.0, 89.0, 182.0);break;
            case 4: rgb(52.0, 73.0, 94.0);break;
            case 5: rgb(241.0, 196.0, 15.0);break;
            case 6: rgb(230.0, 126.0, 34.0);break;
            case 7: rgb(231.0, 76.0, 60.0);break;
            case 8: rgb(236.0, 240.0, 241.0);break;
            case 9: rgb(149.0, 165.0, 166.0);break;
        }
    }

    if(key_t=='r'){
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
        in_start = 0;
        tr_start = 0;
    }

    if(key_t=='t'){
        tr_start = 1;
    }

     if(key_t=='p'){
        rz(60);
        //if(rot==360)
        //    rot = 1;
    }
}

void drag_start(GLint x, GLint y){
    y = 480-y;
    if(in_start==0){
        X1 = x; Y1 = y; z1 = 0;
        in_start = 1;
    }
    else if(in_start==1){
        x2 = x; y2 = y; z2 = 0.3 * ( sqrtf( ((x2-X1)*(x2-X1)) + ((y2-Y1)*(y2-Y1)) ) );
        compute_and_set();
    }

    if(tr_start==1){
        translate(x-pts[7][0],y-pts[7][1],1);
        tr_active=1;
    }
}

void drag_end(GLint x, GLint y){
    y = 480-y;
    if(in_start==1){
        in_start = -1;
    }
    if(tr_start==1&&tr_active==1){
        tr_start=0;
    }
}


// END ACTION CALLBACKS

// Experimental Filler



// OGL LAUNCHERS


void world(){
    glPointSize(2);
    glColor3f(1,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    rgb_set();
}


int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(200,200);
    glutCreateWindow("Many Amaze Very GL WOW");
    glutDisplayFunc(world);
    glutMotionFunc(drag_start);
    glutPassiveMotionFunc(drag_end);
    glutKeyboardFunc(key);
    init();
    glutMainLoop();
    return 0;
}

// END OGL LAUNCHERS

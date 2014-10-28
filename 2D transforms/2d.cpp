/** 2D Tranforms
  * V1.0
  * Author - E.V.Subramanian
  * USAGE - D - draw | R - RESET | T - Translate - DRAG | Q - redraw | S - scale | P - Pencil
  *         X - rotate | , - SHEAR X
  */
#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <list>
#include <vector>

using namespace std;


float** pts;
float R = 1.0, G = 0.0, B = 0.0;
int sz, enter = 1;
int gymin,gxmin;
//control vars
int reset = 1;
//translate vars
int trans=0,transtrack=0;
//scale vars
int sx=0, sy=0, sc=0, strack=0;
//pencil
int pencil=0;
//rotation
int rot = 0,rx = 0, ry = 0, rottrack=0, grxmin, grymin;
//shear
int sx=0, sy=0, sc=0, strack=0;


class points{
    int x;
    int y;
    public:
    points(int x,int y){
        this->x = x;
        this->y = y;
    }

    int getx(){
        return x;
    }

    int gety(){
        return y;
    }
};
list <points*> in;

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


void init(){
    glClearColor(1.0,1.0,1.0,1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,640,0,480);
}

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

void display(arr* tree){
    for(int i=0; i<tree->rows.size(); i++){
        for(int j=0; j<tree->rows[0].size();j++)
            cout<<tree->rows[i][j]<<" ";
        cout<<endl;
    }
}

void drawPolygon(){
    glBegin(GL_POLYGON);
    pts = new float*[in.size()];
    for(int i=0; i<in.size(); i++){
        pts[i] = new float[2];
    }
    sz = in.size();
    while(in.size()>0){
        points* temp = in.front();
        pts[in.size()-1][0] = temp->getx();
        pts[in.size()-1][1] = temp->gety();
        glVertex2i(temp->getx(),temp->gety());
        in.pop_front();
    }
    glEnd();
    glFlush();
}

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
}

void redraw(){
    glClear(GL_COLOR_BUFFER_BIT);
    rgb_set();
    glBegin(GL_POLYGON);
    //set globals
    gxmin = pts[0][0];
    gymin = pts[0][1];
    for(int i=0; i<sz; i++){
        int x = pts[i][0], y = pts[i][1];
        //cout<<pts[i][0]<<" "<<pts[i][1]<<endl;
        if(gxmin>=pts[i][0]&&gymin>=pts[i][1])
        {
            gxmin = pts[i][0];
            gymin = pts[i][1];
        }
        glVertex2i(x,y);
    }
    glEnd();
    glFlush();
    //cout<<"MINX - "<<gxmin<<" MINY - "<<gymin<<endl;
}

// TRANSLATE

void translate(int tx, int ty, int flush){
    float tsup[] = {1,0,tx,0,1,ty,0,0,1};
    arr* tmatrix = new arr(3, 3, tsup);
    //begin magic
    for(int i=0; i<sz; i++){
        float temp[] = {pts[i][0],pts[i][1],1};
        arr* mtemp = new arr(3, 1,temp);
        arr* atemp;
        atemp = mult(tmatrix,mtemp);
        pts[i][0]  = atemp->rows[0][0];
        pts[i][1]  = atemp->rows[1][0];
    }
    if(flush)
        redraw();
}

// SCALE

void scale(float tempx, float tempy){
    float scalex = 1 + (tempx - sx)/100;
    float scaley = 1 + (tempy - sy)/100;
    if(scalex<0)
        scalex = -1*scalex;
    if(scaley<0)
        scaley = -1*scaley;


    //cout<<"SX - "<<sx<<" SY - "<<sy<<"corx - "<<tempx<<" cory - "<<tempy<<"    "<<scalex<<" "<<scaley<<endl;

    translate(-gxmin,-gymin,0);

    float ssup[] = {scalex,0,0,0,scaley,0,0,0,1};
    arr* smatrix = new arr(3, 3, ssup);
    //begin magic
    for(int i=0; i<sz; i++){
        float temp[] = {pts[i][0],pts[i][1],1};
        arr* mtemp = new arr(3, 1,temp);
        arr* atemp;
        atemp = mult(smatrix,mtemp);
        pts[i][0]  = atemp->rows[0][0];
        pts[i][1]  = atemp->rows[1][0];
    }

    translate(gxmin,gymin,0);

    redraw();
}

// ROTATE

void rotate(float tempy){
    float deg = (tempy - ry);
    int degrees = (((int)deg)/10)%360;
    float cosine = cos(degrees);
    float sine = sin(degrees);

    //cout<<"degrees - "<<cosine<<" "<<sine<<endl;

    //translate(-grxmin,-grymin,0);
    translate(-rx,-ry,0);

    float rsup[] = {cosine,-sine,0,sine,cosine,0,0,0,1};
    arr* rmatrix = new arr(3, 3, rsup);
    //begin magic
    for(int i=0; i<sz; i++){
        float temp[] = {pts[i][0],pts[i][1],1};
        arr* mtemp = new arr(3, 1,temp);
        arr* atemp;
        atemp = mult(rmatrix,mtemp);
        pts[i][0]  = atemp->rows[0][0];
        pts[i][1]  = atemp->rows[1][0];
    }

    //translate(grxmin,grymin,0);
    translate(rx,ry,0);

    redraw();
}

//Listeners

void key(unsigned char key_t, int x, int y){
    if(key_t=='d' && reset == 1){
        enter = 0;
        reset = 0;
        drawPolygon();
        redraw();
        in.empty();
    }
    if(key_t=='r'){
        reset = 1;
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
        rgb_set();
        enter = 1;
        //reset transform vars
        trans = 0;
        transtrack = 0;
        sc = 0;
        strack = 0;
		rot = 0;
		rotrack = 0;
		pencil = 0;
    }
    if(key_t=='q'){
        redraw();
    }
    if(key_t=='t'){
        trans=1;
    }
    if(key_t=='s'){
        sc=1;
    }
    if(key_t=='p'){
        pencil=1;
    }
    if(key_t=='x'){
        rot=1;
        grxmin = gxmin;
        grymin = gymin;
    }

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
}

void mouse(int btn, int state, int x, int y){
    y = 480-y;
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN && enter)
    {
        points* temp = new points(x,y);
        in.push_front(temp);
        glBegin(GL_POINTS);
            glVertex2i(x,y);
        glEnd();
        glFlush();
    }
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN && sc)
    {
        sx = x; sy = y;
    }
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN && rot)
    {
        rx = x; ry = y;
    }
}

void drag_start(GLint x, GLint y){
    y = 480-y;
    if(trans){
        translate(x-gxmin,y-gymin,1);
        transtrack = 1;
    }
    if(sc){
        scale(x,y);
        strack = 1;
    }
    if(rot){
        rotate(y);
        rottrack = 1;
    }
    if(pencil){
        glPointSize(4);
        glBegin(GL_POINTS);
            glVertex2i(x,y);
        glEnd();
        glFlush();
        glPointSize(2);
    }
}

void drag_end(GLint x, GLint y){
    y = 480-y;
    if(trans && transtrack)
    {
        trans = 0;
        transtrack = 0;
    }
    if(sc && strack)
    {
        sc = 0;
        strack = 0;
    }
    if(rot && rottrack)
    {
        rot = 0;
        rottrack = 0;
    }
    if(pencil){
        pencil = 0;
    }
}

void world(){
    glPointSize(2);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,0,0);
    rgb_set();
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(200,200);
    glutCreateWindow("Many Amaze Very GL WOW");
    glutDisplayFunc(world);
    glutMouseFunc(mouse);
    glutMotionFunc(drag_start);
    glutPassiveMotionFunc(drag_end);
    glutKeyboardFunc(key);
    init();
    glutMainLoop();
    return 0;
}
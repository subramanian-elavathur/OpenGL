#include <iostream>
#include <windows.h>
#include <cmath>
#include <GL/glut.h>
#include <vector>
#include <fstream>
#include <string>
#include <unistd.h>

#define BEZIER_QUALITY_FACTOR 50
#define BEZIER_ANIMATOR_CP_COUNT 12
#define BEZIER_ANIMATOR_TIME_FACTOR 20
#define BEZIER_ANIMATOR_OBJECT_COUNT 10;

using namespace std;

// classes

class points{
    public:
        float x,y,z;
        points(int x1, int y1, int z1){
            x = x1; y = y1; z = z1;
        }
        points(int x1, int y1){
            x = x1; y = y1; z = 0;
        }
};


float RawData [][BEZIER_ANIMATOR_CP_COUNT][2] = {
// ZERO
{
    {288, 389},{223, 376},{171, 313},{165, 234},{206, 127},{322, 89},
    {438, 95},{488, 145},{497, 214},{481, 337},{426, 405},{290, 388}
},
// ONE
{
   {148, 218},{29, 319},{240, 377},{293, 56},{458, 204},{453, 388},
   {150, 218},{150, 218},{150, 218},{150, 218},{150, 218},{150, 218}
},
// TWO
{
    {255, 307},{301, 364},{385, 368},{422, 317},{416, 246},{364, 177},
    {310, 144},{54, 113},{339, 127},{396, 132},{412, 132},{412, 132},
},
// THREE
{
    {268, 329},{315, 342},{382, 350},{448, 279},{468, 206},{285, 216},
    {268, 258},{250, 205},{437, 182},{481, 191},{384, 94},{260, 119},
},
// FOUR
{
    {401, 183},{191, 222},{103, 21},{414, 460},{360, 504},{357, 307},
    {351, 237},{351, 104},{351, 104},{351, 104},{351, 104},{351, 104}
},
// FIVE
{
    {361, 372},{301, 364},{89, 436},{268, 304},{250, 204},{139, 146},
    {180, 404},{426, 306},{496, 112},{275, 134},{216, 139},{216, 139}
},
// SIX
{
    {347, 369},{275, 343},{236, 285},{208, 195},{163, 105},{305, 37},
    {459, 70},{493, 178},{428, 254},{336, 264},{240, 225},{240, 225}
},
// SEVEN
{
   {276, 324},{486, 341},{350, 313},{415, 321},{553, 344},{511, 391},
   {437, 263},{407, 216},{387, 177},{357, 145},{329, 102},{329, 102}
},
// EIGHT
{
   {322, 278},{456, 156},{236, 104},{192, 206},{239, 269},{311, 296},
   {378, 339},{472, 391},{318, 428},{128, 391},{324, 278},{324, 278}
},
// NINE
{
   {388, 265},{289, 239},{243, 273},{228, 351},{286, 398},{406, 436},
   {396, 342},{396, 286},{393, 245},{379, 184},{378, 127},{378, 116}
}
};

// global vars

float R,G,B;

int ControlPointCount = 0;
vector<points*> ControlPoints;
vector<points*> ControlPointInput;
vector<points*> BezierPoints;
vector<int> Coeffs;

int InputControl = 0, ResetControl = 0, TranslateCP = 0, InsertCP = 0, RemoveCP = 0;
int SelectedIndex = 0; int AnimationControl = 1;

string BEZDATA = "cpdata.bez";

// prototypes

void resetCanvas();
void highlightSelectedControlPoint(int index);
void binomialCoeffs();
void binomialDump();
void bezierDump();
void controlPointDump();
void renderBezier();
void rgb(float r, float g, float b);


class animator{
    public:
        vector<points*> ControlPoints;
        vector<points*> DifferenceFactor;
        vector<points*> BezierPoints;
        vector<int> Coeffs;

        animator(){
            this->loadCP();
            this->genCoeff();
            this->bezierCurve();
        }

        void genCoeff(){
            this->Coeffs.clear();
            int n = this->ControlPoints.size() - 1;
            int temp;
            for(int k = 0; k <= n; k++){
                temp = 1;
                for(int j = n; j >= k + 1; j--)
                    temp *= j;
                for(int j = n - k; j >= 2; j--)
                    temp /= j;
                this->Coeffs.push_back(temp);
            }
        }
        void loadCP(int c=0){
            ControlPoints.clear();
            for(int i=0; i<BEZIER_ANIMATOR_CP_COUNT; i++)
                this->ControlPoints.push_back(new points(RawData[c][i][0],RawData[c][i][1]));
        }
        void constructiveRender(){
            glPointSize(2.0);
            glBegin(GL_POINTS);
                for(int i = 0; i < BEZIER_QUALITY_FACTOR; i++)
                    glVertex2i(round(this->BezierPoints[i]->x),round(this->BezierPoints[i]->y));
            glEnd();
            glFlush();
            glPointSize(1.0);
        }
        void destructiveRender(int NextIndex){
            glClear(GL_COLOR_BUFFER_BIT);
            glFlush();
            if(::BezierPoints.size()>0)
                renderBezier();
            this->colorSwitcher(NextIndex);
            this->constructiveRender();
        }

        void computeBezier(float u){
            int n = this->ControlPoints.size() - 1;
            float BezierBlendFunction;
            points* BezierPoint = new points(0,0,0);
            for(int k = 0; k <= n; k++){
                BezierBlendFunction = (float) (this->Coeffs[k]) * pow(u,k) * pow(1 - u, n - k);
                BezierPoint->x += this->ControlPoints[k]->x * BezierBlendFunction;
                BezierPoint->y += this->ControlPoints[k]->y * BezierBlendFunction;
                BezierPoint->z += this->ControlPoints[k]->z * BezierBlendFunction;
            }
            this->BezierPoints.push_back(BezierPoint);
        }

        void bezierCurve(){
            this->BezierPoints.clear();
            float u;
            for(int k = 0; k <= BEZIER_QUALITY_FACTOR; k++){
                u = (float) ((float) k / (float) BEZIER_QUALITY_FACTOR);
                computeBezier(u);
            }
        }

        void reset(){
            resetCanvas();
            this->BezierPoints.clear();
        }

        void colorSwitcher(int NextIndex){
            switch(NextIndex){
                case 0: rgb(26.0, 188.0, 156.0);break;
                case 1: rgb(46.0, 204.0, 113.0);break;
                case 2: rgb(52.0, 152.0, 219.0);break;
                case 3: rgb(155.0, 89.0, 182.0);break;
                case 4: rgb(52.0, 73.0, 94.0);break;
                case 5: rgb(241.0, 196.0, 15.0);break;
                case 6: rgb(230.0, 126.0, 34.0);break;
                case 7: rgb(231.0, 76.0, 60.0);break;
                case 8: rgb(255.0, 0.0, 0.0);break;
                case 9: rgb(149.0, 165.0, 166.0);break;
            }
        }

        void computeFactor(int NextIndex){
            DifferenceFactor.clear();
            for(int i=0; i<BEZIER_ANIMATOR_CP_COUNT; i++)
                this->DifferenceFactor.push_back(
                    new points(
                           (RawData[NextIndex][i][0]-this->ControlPoints[i]->x)/BEZIER_ANIMATOR_TIME_FACTOR,
                           (RawData[NextIndex][i][1]-this->ControlPoints[i]->y)/BEZIER_ANIMATOR_TIME_FACTOR
                    )
                );
        }

        void addFactor(){
            for(int i=0; i<BEZIER_ANIMATOR_CP_COUNT; i++)
            {
                this->ControlPoints[i]->x += this->DifferenceFactor[i]->x;
                this->ControlPoints[i]->y += this->DifferenceFactor[i]->y;
            }
            this->bezierCurve();
        }

        void animate(int NextIndex){
            this->computeFactor(NextIndex);
            for(int i = 0; i < BEZIER_ANIMATOR_TIME_FACTOR; i++){
                this->addFactor();
                this->destructiveRender(NextIndex);
                usleep(1000*25);
            }
            this->loadCP(NextIndex);
            this->bezierCurve();
            this->destructiveRender(NextIndex);
        }
};

animator* magic = NULL;

// init

void init(){
    glClearColor(1.0,1.0,1.0,1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,640,0,480);
}


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

void rgb_set(int x=0){
    glColor3f(R,G,B);
    glBegin(GL_POLYGON);
        glVertex2i(0,0);
        glVertex2i(20,0);
        glVertex2i(20,20);
        glVertex2i(0,20);
    glEnd();
    glFlush();
    if(x==0)
        rgb(46.0, 204.0, 113.0);
}


// compute and debug functions

void globalReset(){
    InputControl = 1;
    ControlPointInput.clear();
    ControlPoints.clear();
    BezierPoints.clear();
    TranslateCP = 0;
    ResetControl = 0;
    magic = NULL;
    resetCanvas();
}

void assignControlPoint(){
    ControlPoints.clear();
    ControlPointCount = ControlPointInput.size();
    for(vector<points*>::iterator iter = ControlPointInput.begin(); iter != ControlPointInput.end(); iter++){
       ControlPoints.push_back((*iter));
    }
    ControlPointInput.clear();
    binomialCoeffs();
//    binomialDump();
}



void selectTranslatableControlPoint(int x, int y){
    int distx = 99999;
    int disty = 99999;
    for(int i=0; i<ControlPoints.size(); i++){
        int tdistx = abs(x-ControlPoints[i]->x);
        int tdisty = abs(y-ControlPoints[i]->y);
        if(tdistx<=distx && tdisty<=disty){
            SelectedIndex = i;
            distx = tdistx;
            disty = tdisty;
        }
    }
    highlightSelectedControlPoint(SelectedIndex);
}

void binomialCoeffs(){
    Coeffs.clear();
    int n = ControlPoints.size() - 1;
    int temp;
    for(int k = 0; k <= n; k++){
        temp = 1;
        for(int j = n; j >= k + 1; j--)
            temp *= j;
        for(int j = n - k; j >= 2; j--)
            temp /= j;
        Coeffs.push_back(temp);
    }
}

void computeBezier(float u){
    int n = ControlPointCount - 1;
    float BezierBlendFunction;
    points* BezierPoint = new points(0,0,0);
    for(int k = 0; k <= n; k++){
        BezierBlendFunction = (float) (Coeffs[k]) * pow(u,k) * pow(1 - u, n - k);
        BezierPoint->x += ControlPoints[k]->x * BezierBlendFunction;
        BezierPoint->y += ControlPoints[k]->y * BezierBlendFunction;
        BezierPoint->z += ControlPoints[k]->z * BezierBlendFunction;
    }
    BezierPoints.push_back(BezierPoint);
}

void bezierCurve(){
    BezierPoints.clear();
    float u;
    for(int k = 0; k <= BEZIER_QUALITY_FACTOR; k++){
        u = (float) ((float) k / (float) BEZIER_QUALITY_FACTOR);
        computeBezier(u);
    }
    renderBezier();
}

void bezierDump(){
    cout<<endl<<"BEZIER DATA DUMP"<<endl;
    for(int i = 0; i < BezierPoints.size(); i++)
        cout<<" X - "<<BezierPoints[i]->x<<" Y - "<<BezierPoints[i]->y<<endl;
}

void binomialDump(){
    cout<<endl<<"BINOMIAL DATA DUMP"<<endl;
    for(int i = 0; i < Coeffs.size(); i++)
        cout<<i<<" - "<<Coeffs[i]<<endl;
}

void controlPointDump(){
    cout<<endl<<"Control Point DUMP"<<endl;
    for(int i = 0; i < ControlPoints.size(); i++)
        cout<<" X - "<<ControlPoints[i]->x<<" Y - "<<ControlPoints[i]->y<<endl;
}

void controlPointDumpToFile(){
    string Descriptor;
    cout<<"Enter Object Descriptor - ";
    cin>>Descriptor;
    ofstream File;
    File.open (BEZDATA.c_str(), ios_base::app);
    File << "\n*****" << Descriptor << "*****\n\n";
    for(int i = 0; i < ControlPoints.size(); i++)
        File<<"{"<<ControlPoints[i]->x<<", "<<ControlPoints[i]->y<<"},";
    File << endl;
    File.close();
    cout<<"\nDATA WRITTEN\n";
}

// rendering functions

void drawLoopAroundControlPoints(){
    glBegin(GL_LINE_LOOP);
    for(vector<points*>::iterator iter = ControlPoints.begin(); iter != ControlPoints.end(); iter++){
        glVertex2i((*iter)->x,(*iter)->y);
    }
    glEnd();
    glFlush();
    glPointSize(1.0);
}

void plotFinalControlPoints(){
    glPointSize(5.0);
    glBegin(GL_POINTS);
    for(vector<points*>::iterator iter = ControlPoints.begin(); iter != ControlPoints.end(); iter++){
        glVertex2i((*iter)->x,(*iter)->y);
    }
    glEnd();
    glFlush();
    glPointSize(1.0);
    //drawLoopAroundControlPoints();
}

void highlightSelectedControlPoint(int index){
    glPointSize(5.0);
    glColor3f(1.0,0,0);
    glBegin(GL_POINTS);
        glVertex2i(ControlPoints[index]->x, ControlPoints[index]->y);
    glEnd();
    glFlush();
    rgb_set(1);
    glPointSize(1.0);
}

void plotIndividualControlPoint(int x, int y){
    glPointSize(5.0);
    glBegin(GL_POINTS);
        glVertex2i(x,y);
    glEnd();
    glFlush();
    glPointSize(1.0);
}

void resetCanvas(){
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(1.0);
    rgb_set(1);
}

void renderBezier(){
    glPointSize(2.0);
    rgb(52.0, 152.0, 219.0);
    glBegin(GL_POINTS);
        for(int i = 0; i < BEZIER_QUALITY_FACTOR/3; i++)
            glVertex2i(round(BezierPoints[i]->x),round(BezierPoints[i]->y));
    glEnd();
    glFlush();
    rgb(155.0, 89.0, 182.0);
    glBegin(GL_POINTS);
        for(int i = BEZIER_QUALITY_FACTOR/3; i < (2*BEZIER_QUALITY_FACTOR)/3; i++)
            glVertex2i(round(BezierPoints[i]->x),round(BezierPoints[i]->y));
    glEnd();
    glFlush();
    rgb(26.0, 188.0, 156.0);
    glBegin(GL_POINTS);
        for(int i = (2*BEZIER_QUALITY_FACTOR)/3; i < BEZIER_QUALITY_FACTOR; i++)
            glVertex2i(round(BezierPoints[i]->x),round(BezierPoints[i]->y));
    glEnd();
    glFlush();
    glPointSize(1.0);
    plotFinalControlPoints();
}

// event handlers

void key(unsigned char key_t, int x, int y){
    if(key_t <= 57 && key_t >= 48){
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
    if(key_t == 'r')
        globalReset();
    if(key_t == 's')
        TranslateCP = 1;
     if(key_t == 'i')
        InsertCP = 1;
    if(key_t == 'o')
        RemoveCP = 1;
    if(key_t == 'd')
        controlPointDumpToFile();
    if(key_t == 'n'){
        magic = new animator();
        magic->constructiveRender();
    }
    if(key_t == 'm'){
        if(magic==NULL)
            magic = new animator();
        magic->animate(AnimationControl);
        AnimationControl = (AnimationControl+1)%BEZIER_ANIMATOR_OBJECT_COUNT;
    }
}


void drag_start(GLint x, GLint y){
    y = 480-y;
    if(TranslateCP==1){
        ControlPoints[SelectedIndex]->x = x;
        ControlPoints[SelectedIndex]->y = y;
        resetCanvas();
        plotFinalControlPoints();
        highlightSelectedControlPoint(SelectedIndex);
        bezierCurve();
        if(magic!=NULL)
            magic->constructiveRender();
    }
}

void drag_end(GLint x, GLint y){
    y = 480-y;

}


void mouse(int btn, int state, int x, int y){
    y = 480-y;
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN && InputControl==1)
    {
        ControlPointInput.push_back(new points(x, y));
        plotIndividualControlPoint(x,y);
    }
    if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN && InputControl==1)
    {
        InputControl = 0;
        assignControlPoint();
        plotFinalControlPoints();
        bezierCurve();
    }
    if((btn==GLUT_LEFT_BUTTON || btn == GLUT_MIDDLE_BUTTON) && state==GLUT_DOWN && TranslateCP==1 && InputControl==0)
    {
        selectTranslatableControlPoint(x, y);
    }
    if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN && TranslateCP==1 && InputControl==0)
    {
        TranslateCP=0;
        bezierCurve();
    }
    if(btn==GLUT_MIDDLE_BUTTON && state==GLUT_DOWN && TranslateCP==0 && InputControl==0)
    {
        TranslateCP = 1;
    }
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN && TranslateCP==0 && InputControl==0 && InsertCP == 1)
    {
        InsertCP = 0;
        /*ControlPoints.push_back(new points(x, y));
        plotFinalControlPoints();
        binomialCoeffs();
        bezierCurve();*/
    }
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN && TranslateCP==0 && InputControl==0 && RemoveCP == 1)
    {
        RemoveCP = 0;
        /*selectTranslatableControlPoint(x, y);
        ControlPoints.erase(ControlPoints.begin() + SelectedIndex);
        plotFinalControlPoints();
        binomialCoeffs();
        bezierCurve();*/
    }
}

// main


void world(){
    glPointSize(2);
    glColor3f(1,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    rgb_set();
    InputControl = 1;
}


int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(200,200);
    glutCreateWindow("Many Amaze Very GL WOW");
    glutDisplayFunc(world);
    glutMotionFunc(drag_start);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(drag_end);
    glutKeyboardFunc(key);
    init();
    glutMainLoop();
    return 0;
}

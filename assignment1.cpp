#include <iostream>
#include <fstream>
#include <climits>
#include <math.h>
#include <GL/freeglut.h>
#include "loadTGA.h"
using namespace std;

//--Globals ---------------------------------------------------------------
GLuint txId[6];                     //Texture ids
float *x, *y, *z;                   //vertex coordinates
int *nv, *t1, *t2, *t3, *t4;        //number of vertices and vertex indices of each face
int nvert, nface;                   //total number of vertices and faces
float camRotate = 0;
float rotateIllusion1 = 0;
float ball_y = 0;
float ball_x = -14.071;
float pause_time = 50;
bool ball_pause = false;
bool blue_shoot = true;
float rotnAroundBlue = 0;
float rotnAroundRed = 0;
float move_moire_x = 0;
bool moire_going_right = true;
float teacup_angle = 0;
float teapot_color = 0.0;
bool teapot_is_red = true;
float cx = 0;
float cz = 0;
float clx = sin(camRotate);
float clz = -cos(camRotate);
const int N = 50;  // Total number of vertices on the base curve
float vx_init[N] = { 0, 20, 19.5, 19, 18.5, 18, 17.5, 17, 16.5, 16, 15.5, 15, 14, 13, 12, 11, 10, 9, 8, 11,
                      14, 14.2, 14, 11, 8, 9, 10, 12, 13, 14, 15, 15.5, 16.5, 17, 17.5, 18, 18.5, 19, 19.5, 20};

float vy_init[N] = { 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                      21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40};

float nx_init[N] = { 0, 20, 19.5, 19, 18.5, 18, 17.5, 17, 16.5, 16, 15.5, 15, 14, 13, 12, 11, 10, 9, 8, 11,
                      14, 14.2, 14, 11, 8, 9, 10, 12, 13, 14, 15, 15.5, 16.5, 17, 17.5, 18, 18.5, 19, 19.5, 20};
float ny_init[N] = { 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                      21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40};

//---- A function to load textures in TGA format ----
void loadTextures()
{
    glGenTextures(6, txId);     // Create 3 texture ids

    glBindTexture(GL_TEXTURE_2D, txId[0]);
    loadTGA("moree.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, txId[1]);
    loadTGA("static1.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, txId[2]);
    loadTGA("static2.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, txId[3]);
    loadTGA("walls.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, txId[4]);
    loadTGA("door.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, txId[5]);
    loadTGA("stand.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

}

//-- Loads mesh data in OFF format    -------------------------------------
void loadMeshFile(const char* fname)
{
    ifstream fp_in;
    int ne;

    fp_in.open(fname, ios::in);
    if(!fp_in.is_open())
    {
        cout << "Error opening mesh file" << endl;
        exit(1);
    }

    fp_in.ignore(INT_MAX, '\n');                //ignore first line
    fp_in >> nvert >> nface >> ne;              // read number of vertices, polygons, edges (not used)

    x = new float[nvert];                        //create arrays
    y = new float[nvert];
    z = new float[nvert];

    nv = new int[nface];
    t1 = new int[nface];
    t2 = new int[nface];
    t3 = new int[nface];
    t4 = new int[nface];

    for(int i=0; i < nvert; i++)                         //read vertex list
        fp_in >> x[i] >> y[i] >> z[i];

    for(int i=0; i < nface; i++)                         //read polygon list
    {
        fp_in >> nv[i] >> t1[i] >> t2[i] >> t3[i];
        if (nv[i] == 4)
            fp_in >> t4[i];
        else
            t4[i] = -1;
    }

    fp_in.close();
    cout << " File successfully read." << endl;
}



//--Function to compute the normal vector of a triangle with index indx ----------
void normal(int indx)
{
    float x1 = x[t1[indx]], x2 = x[t2[indx]], x3 = x[t3[indx]];
    float y1 = y[t1[indx]], y2 = y[t2[indx]], y3 = y[t3[indx]];
    float z1 = z[t1[indx]], z2 = z[t2[indx]], z3 = z[t3[indx]];
    float nx, ny, nz;
    nx = y1*(z2-z3) + y2*(z3-z1) + y3*(z1-z2);
    ny = z1*(x2-x3) + z2*(x3-x1) + z3*(x1-x2);
    nz = x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2);
    glNormal3f(nx, ny, nz);
}


void drawFloor()
{
    bool flag = false;

    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    for(int x = -500; x <= 500; x += 20)
    {
        for(int z = -500; z <= 500; z += 20)
        {
            if(flag) glColor3f(0.6, 0, 1);
            else glColor3f(1.0, 1.0, 0);
            glVertex3f(x, -0.1, z);
            glVertex3f(x, -0.1, z+20);
            glVertex3f(x+20, -0.1, z+20);
            glVertex3f(x+20, -0.1, z);
            flag = !flag;
        }
    }
    glEnd();
}

void drawWalls()
{
    glColor3f(0, 0, 0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[3]);
    glBegin(GL_QUAD_STRIP);
       glTexCoord2f(0., 0.);
       glVertex3f(-500, 0, 500);
       glTexCoord2f(0., 1.);
       glVertex3f(-500, 400, 500);
       glTexCoord2f(1., 0.);
       glVertex3f(-500, 0, -500);
       glTexCoord2f(1., 1.);
       glVertex3f(-500, 400, -500);
       glTexCoord2f(0., 0.);
       glVertex3f(500, 0, -500);
       glTexCoord2f(0., 1.);
       glVertex3f(500, 400, -500);
       glTexCoord2f(1., 0.);
       glVertex3f(500, 0, 500);
       glTexCoord2f(1., 1.);
       glVertex3f(500, 400, 500);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawDoor()
{
    glColor3f(0, 0, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[4]);
    glBegin(GL_QUADS);
       glTexCoord2f(0., 0.);
       glVertex3f(500, 0, 500);
       glTexCoord2f(1., 0.);
       glVertex3f(-500, 0, 500);
       glTexCoord2f(1., 1.);
       glVertex3f(-500, 400, 500);
       glTexCoord2f(0., 1.);
       glVertex3f(500, 400, 500);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawCanonPlay()
{
    GLUquadric* q = gluNewQuadric();

    // the white ball
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(ball_x, ball_y, 0);
    glutSolidSphere(1, 36, 15);
    glPopMatrix();


    // blue canon
    glColor3f(0.3, 0.3, 1);
    glPushMatrix();
    glTranslatef(-14.071, 0, 0);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(q,2, 1.5, 3, 360, 100);
    glPopMatrix();

    // the blue ball around red
    glPushMatrix();
    glTranslatef((sin(rotnAroundRed)*4)+14.071, 1, cos(rotnAroundRed)*4);
    glutSolidSphere(1,36, 15);
    glPopMatrix();

    // red canon
    glColor3f(1, 0.3, 0.3);
    glPushMatrix();
    glTranslatef(14.071, 0, 0);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(q,2, 1.5, 3, 360, 100);
    glPopMatrix();

    // the red ball around blue
    glPushMatrix();
    glTranslatef((sin(rotnAroundBlue)*4) -14.071, 1, cos(rotnAroundBlue)*4);
    glutSolidSphere(1,36, 15);
    glPopMatrix();


}

void drawMoire()
{
    glPushMatrix();
        glDisable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, txId[0]);
        glBegin(GL_QUADS);
            glTexCoord2f(0., 0.);
            glVertex3f(-150, 0, 0);
            glTexCoord2f(1., 0.);
            glVertex3f(150, 0, 0);
            glTexCoord2f(1., 1.);
            glVertex3f(150, 300, 0);
            glTexCoord2f(0., 1.);
            glVertex3f(-150, 300, 0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, txId[0]);
        glTranslatef(move_moire_x, 0, 0);
        glBegin(GL_QUADS);
            glTexCoord2f(0., 0.);
            glVertex3f(-150, 0, 0.1);
            glTexCoord2f(1., 0.);
            glVertex3f(150, 0, 0.1);
            glTexCoord2f(1., 1.);
            glVertex3f(150, 300, 0.1);
            glTexCoord2f(0., 1.);
            glVertex3f(-150, 300, 0.1);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
    glPopMatrix();


}

void drawStaticIllusions1()
{
    glPushMatrix();
        glDisable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, txId[1]);
        glBegin(GL_QUADS);
            glTexCoord2f(0., 0.);
            glVertex3f(-100, 0, 0);
            glTexCoord2f(1., 0.);
            glVertex3f(100, 0, 0);
            glTexCoord2f(1., 1.);
            glVertex3f(100, 200, 0);
            glTexCoord2f(0., 1.);
            glVertex3f(-100, 200, 0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawStaticIllusions2()
{
    glPushMatrix();
        glDisable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, txId[2]);
        glBegin(GL_QUADS);
            glTexCoord2f(0., 0.);
            glVertex3f(-100, 0, 0);
            glTexCoord2f(1., 0.);
            glVertex3f(100, 0, 0);
            glTexCoord2f(1., 1.);
            glVertex3f(100, 200, 0);
            glTexCoord2f(0., 1.);
            glVertex3f(-100, 200, 0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawVase()
{
    float toRadians = M_PI / 180.0;   //Conversion from degrees to radians
    float angStep = 10.0 * toRadians;  //Rotate base curve in 10 deg steps
    int nSlices = 36;                 //36 slices at 10 deg intervals

    float vx[N], vy[N], vz[N];   //vertex positions
    float wx[N], wy[N], wz[N];
    float nx[N], ny[N], nz[N];   //normal vectors
    float mx[N], my[N], mz[N];
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (int i = 0; i < N; i++)     //Initialize data everytime the frame is refreshed
    {
        vx[i] = vx_init[i];
        vy[i] = vy_init[i];
        vz[i] = 0;
        nx[i] = nx_init[i];
        ny[i] = ny_init[i];
        nz[i] = 0;
    }
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[5]);
    glColor3f(1, 0.75, 1);

    for (int j = 0; j < nSlices; j++)
    {
        for (int i = 0; i < N; i++)
        {
            wx[i] = cos(angStep) * vx[i] + sin(angStep) * vz[i];
            wy[i] = vy[i];
            wz[i] = -sin(angStep) * vx[i] + cos(angStep) * vz[i];
            mx[i] = cos(angStep) * nx[i] + sin(angStep) * nz[i];
            my[i] = ny[i];
            mz[i] = -sin(angStep) * nx[i] + cos(angStep) * nz[i];
        }

        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i < N; i++)
        {
            glNormal3f(nx[i], ny[i], nz[i]);
            glTexCoord2f((float)j/nSlices, (float)i/(N-1));
            glVertex3f(vx[i], vy[i], vz[i]);
            glNormal3f(mx[i], my[i], mz[i]);
            glTexCoord2f((float)(j+1)/nSlices, (float)i/(N-1));
            glVertex3f(wx[i], wy[i], wz[i]);
        }
        glEnd();

        for (int i = 0; i < N; i++)    //Update vertices
        {
            vx[i] = wx[i]; vy[i] = wy[i]; vz[i] = wz[i];
            nx[i] = mx[i]; ny[i] = my[i]; nz[i] = mz[i];
        }
    }
    glDisable(GL_TEXTURE_2D);
}

void drawTeapot()
{
    float light[4] = {15., 20., 0, 1};
    float shadowMat[16] = {light[1], 0, 0, 0, -light[0], 0, -light[2],-1, 0, 0, light[1], 0, 0, 0, 0, light[1]};
    glLightfv(GL_LIGHT1, GL_POSITION, light);
    glColor3f(1, teapot_color, 0.25);
    glEnable(GL_LIGHTING);
    glPushMatrix();
        glTranslatef(0, 6, 0);
        glRotatef(teacup_angle, 0, 1, 0);
        glutSolidTeapot(1);
    glPopMatrix();

    glDisable(GL_LIGHTING);
    glColor3f(0.2, 0.2, 0.2);
    glPushMatrix();
        glMultMatrixf(shadowMat);
        glTranslatef(0, 6, 0);
        glRotatef(teacup_angle, 0, 1, 0);
        glutSolidTeapot(1);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}


//--Display: ----------------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.
void display()
{
    float lpos[4] = {10., 0., 10., 1.0};           //light's position

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //GL_LINE = Wireframe;   GL_FILL = Solid
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cx, 200, cz, cx+clx, 200, cz+clz, 0, 1, 0);

    glLightfv(GL_LIGHT0, GL_POSITION, lpos);    //set light position

    glDisable(GL_LIGHTING);
    glColor3f(1., 1., 1.);
    glPushMatrix();
        glTranslatef(-350, 200, 0);
        glRotatef(rotateIllusion1, 0, 1, 0);
        glScalef(3, 3, 3);
        for(int indx = 0; indx < nface; indx++)     //draw each face
        {   if (indx >= 7) glColor3f(0, 1, 1);
            normal(indx);
            if (nv[indx] == 3)  glBegin(GL_TRIANGLES);
            else                glBegin(GL_QUADS);
                glVertex3d(x[t1[indx]], y[t1[indx]], z[t1[indx]]);
                glVertex3d(x[t2[indx]], y[t2[indx]], z[t2[indx]]);
                glVertex3d(x[t3[indx]], y[t3[indx]], z[t3[indx]]);
                if(nv[indx]==4)
                  glVertex3d(x[t4[indx]], y[t4[indx]], z[t4[indx]]);
            glEnd();
        }
    glPopMatrix();
    drawFloor();
    glEnable(GL_LIGHTING);
    drawWalls();
    drawDoor();
    glPushMatrix();
        glTranslatef(400, 0, 0);
        glRotatef(90, 0, 1, 0);
        glScalef(12, 12, 12);
        drawCanonPlay();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0, 20, -400);
        drawMoire();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-300, 100, -300);
        glRotatef(45, 0, 1, 0);
        drawStaticIllusions1();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(300, 100, -300);
        glRotatef(-45, 0, 1, 0);
        drawStaticIllusions2();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0, 0, 350);
        glScalef(30, 30, 30);
        drawTeapot();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0, 0, 350);
        glScalef(3, 3, 3);
        drawVase();
    glPopMatrix();

    glutSwapBuffers();
}

//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
    loadMeshFile("illusion1.off");            //Specify mesh fitle name here
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   //Background colour
    loadTextures();

    glEnable(GL_LIGHTING);                  //Enable OpenGL states
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glBlendFunc(GL_DST_COLOR, GL_ZERO);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100, 1, 10.0, 1000.0);   //Camera Frustum
}

//------------ Special key event callback ---------------------------------
// To enable the use of left and right arrow keys to rotate the scene
void special(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT) camRotate -= 0.1, clx = sin(camRotate), clz = -cos(camRotate);
    else if(key == GLUT_KEY_RIGHT) camRotate += 0.1, clx = sin(camRotate), clz = -cos(camRotate);
    else if (key == GLUT_KEY_UP) cx += clx * 4, cz += clz * 4;
    else if (key == GLUT_KEY_DOWN) cx -= clx * 4, cz -= clz * 4;
    glutPostRedisplay();
}

void keyPressed (unsigned char key, int x, int y) {
    if(key == '1') cx = 200, cz = 0, camRotate = -1.6, clx = sin(camRotate), clz = -cos(camRotate);
    else if (key == '2') cx = 0, cz = -100, camRotate = 0, clx = sin(camRotate), clz = -cos(camRotate);
    else if (key == '3') cx = 100, cz = 0, camRotate = 1.6, clx = sin(camRotate), clz = -cos(camRotate);
    else if (key == '0') cx = 0, cz = 490, camRotate = 0, clx = sin(camRotate), clz = -cos(camRotate);
    glutPostRedisplay();
}

void myTimer(int value)
{
    if (ball_pause) {
        pause_time --;
        if (pause_time < 0) {
            ball_pause = !ball_pause;
        }
    } else {
        if (blue_shoot) {
            ball_x = ball_x + 0.5;
            if (ball_x > (sqrt(-1 * (3 - 25))*3)) {
                ball_x = (sqrt(-1 * (3 - 25))*3);
                pause_time = 50;
                ball_pause = true;
                blue_shoot = !blue_shoot;
            }
        } else {
            ball_x = ball_x - 0.5;
            if (ball_x < -(sqrt(-1 * (3 - 25))*3)) {
                ball_x = -(sqrt(-1 * (3 - 25))*3);
                pause_time = 50;
                ball_pause = true;
                blue_shoot = !blue_shoot;
            }
        }
        ball_y = -1 * pow(ball_x/3, 2) + 25;
    }

    if (moire_going_right) {
         move_moire_x += 0.5;
         if (move_moire_x > 50) {
             moire_going_right = false;
        }
    } else {
        move_moire_x -= 0.5;
        if (move_moire_x < -50) {
            moire_going_right = true;
        }
    }
    if (teapot_is_red) {
        teapot_color += 0.01;
    } else {
        teapot_color -= 0.01;
    }
    if (teapot_color <= 0 || teapot_color >= 1) {
        teapot_is_red = !teapot_is_red;
    }
    rotateIllusion1 += 2;
    rotnAroundBlue += 0.2;
    rotnAroundRed += 0.1;
    teacup_angle += .75;


    glutPostRedisplay();
    value ++;
    glutTimerFunc(50, myTimer, value);
}

//  ------- Main: Initialize glut window and register call backs -----------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize (600, 600);
    glutInitWindowPosition (10, 10);
    glutCreateWindow ("Model3D");
    glutTimerFunc(50, myTimer, 0);
    initialize();

    glutDisplayFunc(display);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyPressed);
    glutMainLoop();
    return 0;
}

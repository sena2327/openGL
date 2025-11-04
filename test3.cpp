#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <GL/glut.h>
#include <GL/glu.h>

#define WINDOW_X (500)
#define WINDOW_Y (500)
#define WINDOW_NAME "test3"

void init_GL(int argc, char *argv[]);
void init();
void set_callback_functions();

void glut_display();
void glut_keyboard(unsigned char key, int x, int y);
void glut_mouse(int button, int state, int x, int y);
void glut_motion(int x, int y);

void draw_pyramid();
void draw_cube(double width, double height, double depth);

// グローバル変数
double g_angle1 = 0.0;
double g_angle2 = 0.0;
double g_distance = 20.0;
bool g_isLeftButtonOn = false;
bool g_isRightButtonOn = false;

int main(int argc, char *argv[]){
    /* OpenGLの初期化 */
    init_GL(argc, argv);

    /* このプログラム特有の初期化 */
    init();

    /* コールバック関数の登録 */
    set_callback_functions();

    /* メインループ */
    glutMainLoop();

    return 0;
}

void init_GL(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_X, WINDOW_Y);
    glutCreateWindow(WINDOW_NAME);
}

void init(){
    glClearColor(0.0, 0.0, 0.0, 0.0); // 背景の塗りつぶし色
}

void set_callback_functions(){
    glutDisplayFunc(glut_display);
    glutKeyboardFunc(glut_keyboard);
    glutMouseFunc(glut_mouse);
    glutMotionFunc(glut_motion);
    glutPassiveMotionFunc(glut_motion);
}

void glut_keyboard(unsigned char key, int x, int y){
    switch(key){
        case 'q':
        case 'Q':
        case '\033': // ESC
            exit(0);
    }
    glutPostRedisplay();
}

void glut_mouse(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON){
        g_isLeftButtonOn = (state == GLUT_DOWN);
    }
    if(button == GLUT_RIGHT_BUTTON){
        g_isRightButtonOn = (state == GLUT_DOWN);
    }
}

void glut_motion(int x, int y){
    static int px = -1, py = -1;

    if(g_isLeftButtonOn){
        if(px >= 0 && py >= 0){
            g_angle1 += (double)-(x - px)/20.0;
            g_angle2 += (double)(y - py)/20.0;
        }
        px = x;
        py = y;
    }
    else if(g_isRightButtonOn){
        if(px >= 0 && py >= 0){
            g_distance += (double)(y - py)/20.0;
        }
        px = x;
        py = y;
    }
    else{
        px = -1;
        py = -1;
    }

    glutPostRedisplay();
}

void glut_display(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, 1.0, 0.1, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (cos(g_angle2) > 0){
        gluLookAt(g_distance * cos(g_angle2) * sin(g_angle1),
                  g_distance * sin(g_angle2),
                  g_distance * cos(g_angle2) * cos(g_angle1),
                  0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0);
    } else {
        gluLookAt(g_distance * cos(g_angle2) * sin(g_angle1),
                  g_distance * sin(g_angle2),
                  g_distance * cos(g_angle2) * cos(g_angle1),
                  0.0, 0.0, 0.0,
                  0.0, -1.0, 0.0);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // ティーポット
    glPushMatrix();
    glTranslatef(-1.0, 3.0, 0.0);
    //glRotatef(-30, 0.0, 0.0, 1.0);
    //glTranslatef(-1.0, 3.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glutWireTeapot(1.0);
    glPopMatrix();

    // ピラミッド
    glPushMatrix();
    glTranslatef(-1.5, -2.0, 0.0);
    draw_pyramid();
    glPopMatrix();

    //机
    glPushMatrix();
    glTranslatef(-1.5, 2.0, 0.0);
    draw_cube(5.0, 0.5, 5.0);
    glPopMatrix();

    // 足
    glPushMatrix();
    glTranslatef(-1.5, -2.0, 0.0);
    draw_cube(1.0, 4.0, 1.0);
    glPopMatrix(); 

    glFlush();
    glDisable(GL_DEPTH_TEST);
    glutSwapBuffers();
}

void draw_pyramid(){
    GLdouble pointO[] = {0.0, 1.0, 0.0};
    GLdouble pointA[] = {1.5, -1.0, 1.5};
    GLdouble pointB[] = {-1.5, -1.0, 1.5};
    GLdouble pointC[] = {-1.5, -1.0, -1.5};
    GLdouble pointD[] = {1.5, -1.0, -1.5};

    glColor3d(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
        glVertex3dv(pointO);
        glVertex3dv(pointA);
        glVertex3dv(pointB);
    glEnd();

    glColor3d(1.0, 1.0, 0.0);
    glBegin(GL_TRIANGLES);
        glVertex3dv(pointO);
        glVertex3dv(pointB);
        glVertex3dv(pointC);
    glEnd();

    glColor3d(0.0, 1.0, 1.0);
    glBegin(GL_TRIANGLES);
        glVertex3dv(pointO);
        glVertex3dv(pointC);
        glVertex3dv(pointD);
    glEnd();

    glColor3d(1.0, 0.0, 1.0);
    glBegin(GL_TRIANGLES);
        glVertex3dv(pointO);
        glVertex3dv(pointD);
        glVertex3dv(pointA);
    glEnd();

    glColor3d(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
        glVertex3dv(pointA);
        glVertex3dv(pointB);
        glVertex3dv(pointC);
        glVertex3dv(pointD);
    glEnd();
}

void draw_cube(double width, double height, double depth){
    double hx = width * 0.5;
    double hy0 = 0.0;        /* 底面 y */
    double hy1 = height;     /* 上面 y */
    double hz = depth * 0.5;

    GLdouble p[8][3] = {
        {-hx, hy0, -hz}, // 0
        { hx, hy0, -hz}, // 1
        { hx, hy1, -hz}, // 2
        {-hx, hy1, -hz}, // 3
        {-hx, hy0,  hz}, // 4
        { hx, hy0,  hz}, // 5
        { hx, hy1,  hz}, // 6
        {-hx, hy1,  hz}  // 7
    };

    // 背面 (z = -hz)
    glColor3d(1.0, 0.0, 0.0);
    glBegin(GL_QUADS);
        glVertex3dv(p[0]); glVertex3dv(p[1]); glVertex3dv(p[2]); glVertex3dv(p[3]);
    glEnd();

    // 前面 (z = +hz)
    glColor3d(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
        glVertex3dv(p[4]); glVertex3dv(p[5]); glVertex3dv(p[6]); glVertex3dv(p[7]);
    glEnd();

    // 左面 (x = -hx)
    glColor3d(0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
        glVertex3dv(p[0]); glVertex3dv(p[3]); glVertex3dv(p[7]); glVertex3dv(p[4]);
    glEnd();

    // 右面 (x = +hx)
    glColor3d(1.0, 1.0, 0.0);
    glBegin(GL_QUADS);
        glVertex3dv(p[1]); glVertex3dv(p[2]); glVertex3dv(p[6]); glVertex3dv(p[5]);
    glEnd();

    // 上面 (y = hy1)
    glColor3d(0.0, 1.0, 1.0);
    glBegin(GL_QUADS);
        glVertex3dv(p[3]); glVertex3dv(p[2]); glVertex3dv(p[6]); glVertex3dv(p[7]);
    glEnd();

    // 底面 (y = hy0)
    glColor3d(1.0, 0.0, 1.0);
    glBegin(GL_QUADS);
        glVertex3dv(p[0]); glVertex3dv(p[1]); glVertex3dv(p[5]); glVertex3dv(p[4]);
    glEnd();
}
// test4.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <GL/glut.h>
#include <GL/glu.h>

#define WINDOW_X (500)
#define WINDOW_Y (500)
#define WINDOW_NAME "test4"

void init_GL(int argc, char *argv[]);
void init();
void set_callback_functions();

void glut_display();
void glut_keyboard(unsigned char key, int x, int y);
void glut_mouse(int button, int state, int x, int y);
void glut_motion(int x, int y);
void glutSpecialFuncc(int key, int x, int y);

void draw_pyramid();
void draw_plane();

// グローバル変数
double g_angle1 = 0.0;
double g_angle2 = 0.0;
double g_angle3 = 0.0;
double g_angle4 = 3.141592 / 4.0;  // 光源高さ用
double g_distance = 20.0;
bool   g_isLeftButtonOn  = false;
bool   g_isRightButtonOn = false;

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
    glClearColor(0.0, 0.0, 0.0, 0.0); // 背景色
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    // スケール後も法線を自動正規化
    glEnable(GL_NORMALIZE);
}

void set_callback_functions(){
    glutDisplayFunc(glut_display);
    glutKeyboardFunc(glut_keyboard);
    glutMouseFunc(glut_mouse);
    glutMotionFunc(glut_motion);
    glutPassiveMotionFunc(glut_motion);
    glutSpecialFunc(glutSpecialFuncc);
}

void glut_keyboard(unsigned char key, int x, int y){
    (void)x; (void)y;
    switch(key){
        case 'q':
        case 'Q':
        case '\033': // ESC
            exit(0);
    }
    glutPostRedisplay();
}

void glut_mouse(int button, int state, int x, int y){
    (void)x; (void)y;
    if(button == GLUT_LEFT_BUTTON){
        g_isLeftButtonOn = (state == GLUT_DOWN);
    }else if(button == GLUT_RIGHT_BUTTON){
        g_isRightButtonOn = (state == GLUT_DOWN);
    }
}

void glut_motion(int x, int y){
    static int px = -1, py = -1;

    if(g_isLeftButtonOn){
        if(px >= 0 && py >= 0){
            g_angle1 += (double)-(x - px)/20.0;
            g_angle2 += (double) (y - py)/20.0;
        }
        px = x; py = y;
    }else if(g_isRightButtonOn){
        if(px >= 0 && py >= 0){
            g_angle3 += (double)(x - px)/20.0;  // 光源の水平回転
        }
        px = x; py = y;
    }else{
        px = -1; py = -1;
    }
    glutPostRedisplay();
}

void glutSpecialFuncc(int key, int x, int y){
    (void)x; (void)y;
    switch(key){
        case GLUT_KEY_UP:
            g_angle4 += 0.1; // 光源の高さ調整
            if(g_angle4 > M_PI / 2.0) g_angle4 = M_PI / 2.0;
            break;
        case GLUT_KEY_DOWN:
            g_angle4 -= 0.1;
            if(g_angle4 < -M_PI / 2.0) g_angle4 = -M_PI / 2.0;
            break;
        case GLUT_KEY_LEFT:
            g_distance += 0.5; // 視点距離調整
            break;
        case GLUT_KEY_RIGHT:
            g_distance -= 0.5;
            if(g_distance < 1.0) g_distance = 1.0; // めり込み防止
            break;
    }
    glutPostRedisplay();
}

void glut_display(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, 1.0, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (cos(g_angle2) > 0.0){
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

    // 画面クリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 光源（位置は毎フレーム設定）
    GLfloat lightpos[] = {
        5.0f * cosf((float)g_angle4) * sinf((float)g_angle3),
        5.0f * sinf((float)g_angle4),
        5.0f * cosf((float)g_angle4) * cosf((float)g_angle3),
        1.0f
    };
    GLfloat lightpos1[] = {-lightpos[0],lightpos[1],-lightpos[2],1.0f};
    GLfloat diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ambient[]  = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
    

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightpos1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  diffuse);
    glLightfv(GL_LIGHT1, GL_AMBIENT,  ambient);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);

    // 光源マーカー（小球）
    glPushMatrix();
    glTranslatef(lightpos[0], lightpos[1], lightpos[2]);
    glDisable(GL_LIGHTING);          // マーカーは発光っぽく
    glColor3f(1.0f, 1.0f, 0.6f);
    glutSolidSphere(0.2, 32, 32);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(lightpos1[0], lightpos1[1], lightpos1[2]);
    glDisable(GL_LIGHTING);          // マーカーは発光っぽく
    glColor3f(1.0f, 1.0f, 0.6f);
    glScalef(4.0f, 4.0f, 4.0f);
    glutSolidSphere(0.2, 32, 32);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    // 地面
    glPushMatrix();
    glTranslatef(0.0f, -2.0f, 0.0f);
    draw_plane();
    glPopMatrix();

    // ピラミッド（Y方向にスケール）
    glPushMatrix();
    glScalef(1.0f, 2.0f, 1.0f);
    draw_pyramid();
    glPopMatrix();

    glFlush();

    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);

    glutSwapBuffers();
}

void draw_pyramid(){
    GLdouble pointO[] = {0.0,  1.0,  0.0};
    GLdouble pointA[] = {1.5, -1.0,  1.5};
    GLdouble pointB[] = {-1.5, -1.0,  1.5};
    GLdouble pointC[] = {-1.5, -1.0, -1.5};
    GLdouble pointD[] = {1.5, -1.0, -1.5};

    GLfloat facecolor1[] = {1.0f, 0.0f, 1.0f, 0.8f};
    GLfloat facecolor2[] = {1.0f, 1.0f, 0.0f, 0.8f};
    GLfloat facecolor3[] = {0.0f, 1.0f, 1.0f, 0.8f};
    GLfloat facecolor4[] = {1.0f, 0.0f, 1.0f, 0.8f};
    GLfloat facecolor5[] = {1.0f, 1.0f, 1.0f, 1.0f};

    // 面1
    glMaterialfv(GL_FRONT, GL_DIFFUSE, facecolor1);
    glNormal3d(0.0, 0.6, 0.8);
    glBegin(GL_TRIANGLES);
        glVertex3dv(pointO); glVertex3dv(pointA); glVertex3dv(pointB);
    glEnd();

    // 面2
    glMaterialfv(GL_FRONT, GL_DIFFUSE, facecolor2);
    glNormal3d(-0.8, 0.6, 0.0);
    glBegin(GL_TRIANGLES);
        glVertex3dv(pointO); glVertex3dv(pointB); glVertex3dv(pointC);
    glEnd();

    // 面3
    glMaterialfv(GL_FRONT, GL_DIFFUSE, facecolor3);
    glNormal3d(0.0, 0.6, -0.8);
    glBegin(GL_TRIANGLES);
        glVertex3dv(pointO); glVertex3dv(pointC); glVertex3dv(pointD);
    glEnd();

    // 面4
    glMaterialfv(GL_FRONT, GL_DIFFUSE, facecolor4);
    glNormal3d(0.8, 0.6, 0.0);
    glBegin(GL_TRIANGLES);
        glVertex3dv(pointO); glVertex3dv(pointD); glVertex3dv(pointA);
    glEnd();

    // 底面
    glMaterialfv(GL_FRONT, GL_DIFFUSE, facecolor5);
    glNormal3d(0.0, -1.0, 0.0);
    glBegin(GL_POLYGON);
        glVertex3dv(pointA); glVertex3dv(pointB);
        glVertex3dv(pointC); glVertex3dv(pointD);
    glEnd();
}

void draw_plane(){
    GLfloat facecolor[] = {0.9f, 0.9f, 0.9f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, facecolor);
    glNormal3d(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
        glVertex3d( 4.0, 0.0,  4.0);
        glVertex3d( 4.0, 0.0, -4.0);
        glVertex3d(-4.0, 0.0, -4.0);
        glVertex3d(-4.0, 0.0,  4.0);
    glEnd();
}

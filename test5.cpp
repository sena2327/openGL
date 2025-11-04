// test5.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define WINDOW_X (500)
#define WINDOW_Y (500)
#define WINDOW_NAME "test5"
#define TEXTURE_HEIGHT (512)
#define TEXTURE_WIDTH  (512)

void init_GL(int argc, char *argv[]);
void init();
void set_callback_functions();

void glut_display();
void glut_keyboard(unsigned char key, int x, int y);
void glut_mouse(int button, int state, int x, int y);
void glut_motion(int x, int y);
void glut_idle();
void draw_pyramid();
void set_texture();

// グローバル変数
double g_angle1 = 0.0;
double g_angle2 = -3.141592 / 6.0;
double g_distance = 10.0;
bool g_isLeftButtonOn = false;
bool g_isRightButtonOn = false;
GLuint g_TextureHandles[3] = {0, 0, 0};

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
    glClearColor(0.2f, 0.2f, 0.2f, 0.2f);

    glGenTextures(3, g_TextureHandles);
    for(int i = 0; i < 3; i++){
        glBindTexture(GL_TEXTURE_2D, g_TextureHandles[i]);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                     TEXTURE_WIDTH, TEXTURE_HEIGHT, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    }

    set_texture();
}

void set_callback_functions(){
    glutDisplayFunc(glut_display);
    glutKeyboardFunc(glut_keyboard);
    glutMouseFunc(glut_mouse);
    glutMotionFunc(glut_motion);
    glutPassiveMotionFunc(glut_motion);
    glutIdleFunc(glut_idle);
}

void glut_keyboard(unsigned char key, int x, int y){
    (void)x; (void)y;
    switch(key){
        case 'q':
        case 'Q':
        case '\033':
            exit(0);
    }
    glutPostRedisplay();
}

void glut_mouse(int button, int state, int x, int y){
    (void)x; (void)y;
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
            g_angle2 += (double) (y - py)/20.0;
        }
        px = x; py = y;
    }else if(g_isRightButtonOn){
        if(px >= 0 && py >= 0){
            g_distance += (double)(y - py)/20.0;
        }
        px = x; py = y;
    }else{
        px = -1; py = -1;
    }
    glutPostRedisplay();
}

void glut_display(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, 1.0, 0.1, 100.0);

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

    draw_pyramid();

    glFlush();
    glDisable(GL_DEPTH_TEST);
    glutSwapBuffers();
}

void glut_idle(){
    static int counter = 0;

    if(counter == 0){
        glBindTexture(GL_TEXTURE_2D, g_TextureHandles[0]);
    }else if(counter == 100){
        glBindTexture(GL_TEXTURE_2D, g_TextureHandles[1]);
    }else if(counter == 200){
        glBindTexture(GL_TEXTURE_2D, g_TextureHandles[2]);
    }

    counter++;
    if(counter > 300) counter = 0;

    glutPostRedisplay();
}

void draw_pyramid(){
    GLdouble pointO[] = {0.0,  1.0,  0.0};
    GLdouble pointA[] = {1.5, -1.0,  1.5};
    GLdouble pointB[] = {-1.5, -1.0,  1.5};
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

    // 底面をテクスチャ表示
    glColor3d(1.0, 1.0, 1.0);   // MODULATE 前提なので白に
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_POLYGON);
        glTexCoord2d(1.0, 0.0); glVertex3dv(pointA);
        glTexCoord2d(0.0, 0.0); glVertex3dv(pointB);
        glTexCoord2d(0.0, 1.0); glVertex3dv(pointC);
        glTexCoord2d(1.0, 1.0); glVertex3dv(pointD);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

static void upload_centered_rgb_texture(GLuint tex, const cv::Mat& rgb)
{
    glBindTexture(GL_TEXTURE_2D, tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    int ox = (TEXTURE_WIDTH  - rgb.cols) / 2;
    int oy = (TEXTURE_HEIGHT - rgb.rows) / 2;
    if(ox < 0 || oy < 0){
        // 画像が大きすぎる場合はリサイズ
        cv::Mat resized;
        double sx = (double)TEXTURE_WIDTH  / rgb.cols;
        double sy = (double)TEXTURE_HEIGHT / rgb.rows;
        double s  = (sx < sy) ? sx : sy;
        cv::resize(rgb, resized, cv::Size(), s, s, cv::INTER_AREA);
        ox = (TEXTURE_WIDTH  - resized.cols) / 2;
        oy = (TEXTURE_HEIGHT - resized.rows) / 2;
        glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy,
                        resized.cols, resized.rows,
                        GL_RGB, GL_UNSIGNED_BYTE, resized.data);
    }else{
        glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy,
                        rgb.cols, rgb.rows,
                        GL_RGB, GL_UNSIGNED_BYTE, rgb.data);
    }
}

void set_texture(){
    const char *inputFileNames[3] = {"fruits.jpg", "lena.jpg", "baboon.jpg"};
    for(int i = 0; i < 3; i++){
        cv::Mat input = cv::imread(inputFileNames[i], 1);
        if(input.empty()){
            // 読み込み失敗時はチェックボードで埋める
            cv::Mat fallback(TEXTURE_HEIGHT, TEXTURE_WIDTH, CV_8UC3);
            for(int y=0; y<TEXTURE_HEIGHT; ++y){
                for(int x=0; x<TEXTURE_WIDTH; ++x){
                    bool c = ((x/32)+(y/32))%2;
                    fallback.at<cv::Vec3b>(y,x) = c ? cv::Vec3b(255,255,255) : cv::Vec3b(0,0,0);
                }
            }
            cv::cvtColor(fallback, fallback, cv::COLOR_BGR2RGB);
            glBindTexture(GL_TEXTURE_2D, g_TextureHandles[i]);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                            TEXTURE_WIDTH, TEXTURE_HEIGHT,
                            GL_RGB, GL_UNSIGNED_BYTE, fallback.data);
            continue;
        }

        // BGR → RGB
        cv::cvtColor(input, input, cv::COLOR_BGR2RGB);

        // 中央に貼り付け
        upload_centered_rgb_texture(g_TextureHandles[i], input);
    }
}

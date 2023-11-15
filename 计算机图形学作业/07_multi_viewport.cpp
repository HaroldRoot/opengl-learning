#include <iostream>
#include <vector>

#include "getAxis.h"
#include "getTetrahedron.h"

using namespace std;

GLuint objectID, axisID;              // 四面体对象ID、坐标轴ID
int winWidth = 400, winHeight = 200;  // 窗口的宽度和高度
static float angle = 45;              // angle绕坐标轴的旋转角
static GLfloat xRot = 0;              // 旋转参数
static GLfloat yRot = 0;              // 旋转参数
static GLfloat zRot = 0;              // 旋转参数
static GLsizei iMode = 1;             // 菜单值
static point3D s_eye(5, 5, 5);        // 观察点位置
static point3D s_at(0, 0, 0);         // 视点中心

static point3D A(2, 0, 0), B(2, 1, 0), C(0, 1, 0), D(1, 1, 1);  // 四面体

void initial();
void triangle(GLsizei mode);
void Key(unsigned char key, int x, int y);
void Display();
void ChangeSize(GLsizei w, GLsizei h);
void SpecialKeys(int key, int x, int y);
void ProcessMenu(int value);

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(winWidth, winHeight);

    glutCreateWindow("Multi-Viewport");  // 多视区

    int nGlutPolyMenu = glutCreateMenu(ProcessMenu);  // 创建菜单并定义菜单回调函数
    glutAddMenuEntry("front view XOZ(V)", 1);
    glutAddMenuEntry("side view XOY(H)", 2);
    glutAddMenuEntry("top view YOZ(W)", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);  // 主菜单与鼠标右键关联

    initial();

    glutDisplayFunc(Display);     // 指定窗口重绘响应函数
    glutReshapeFunc(ChangeSize);  // 指定窗口大小改变响应函数
    // glutKeyboardFunc(Key);         // 指定键盘响应函数
    glutSpecialFunc(SpecialKeys);  // 指定功能键响应函数

    glutMainLoop();

    return 0;
}

void initial() {
    glClearColor(1, 1, 1, 1);

    axisID = getAxis(5);                    // 创建坐标轴
    objectID = getTetrahedron(A, B, C, D);  // 创建四面体
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 绘制左视区（正交投影）
    glViewport(0, 0, winWidth / 2, winHeight);
    GLint vpLeft[4];
    glGetIntegerv(GL_VIEWPORT, vpLeft);  // 0, 0, 200, 200

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-10, 10, -10, 10, -10, 10);  // 定义左视区的三维正交观察可视体
    setLookAt(iMode, s_eye, s_at);       // 根据菜单选择，控制左视区的照相机的位置，使得左视区正交投影四面体的三视图

    glCallList(objectID);  // 绘制四面体
    glCallList(axisID);    // 绘制坐标轴

    // 绘制右视区（透视投影）
    glViewport(winWidth / 2, 0, winWidth / 2, winHeight);
    GLint vpRight[4];
    glGetIntegerv(GL_VIEWPORT, vpRight);  // 200, 0, 200, 200,

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45, 1, 0, 10);      // 定义右视区的三维透视观察可视体
    setLookAt(iMode, angle, s_eye, s_at);  // 通过给定函数，建立右视区的照相机的移动目坐标，使得右视区透视投影四面体。

    glCallList(objectID);  // 绘制四面体
    glCallList(axisID);    // 绘制坐标轴

    glFlush();
}

void ChangeSize(GLsizei w, GLsizei h) {
    if (h == 0)
        h = 1;
    glViewport(0, 0, w, h);

    winWidth = w;
    winHeight = h;
}

void SpecialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        angle -= 5;
    } else if (key == GLUT_KEY_DOWN) {
        angle += 5;
    }
    glutPostRedisplay();  // 重新绘制
}

void ProcessMenu(int value) {
    iMode = value;
    glutPostRedisplay();  // 重新绘制
}
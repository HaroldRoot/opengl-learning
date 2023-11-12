/*
g++ -fdiagnostics-color=always -g ./计算机图形学作业/05_liang_barsky.cpp ./计算机图形学作业/lineClipping.cpp -o ./计算机图形学作业/05_liang_barsky -lGL -lGLU -lglut
./计算机图形学作业/05_liang_barsky
*/

#include "lineClipping.h"

using namespace std;

int winWidth = 400;
int winHeight = 300;

rect winObj = Rectangle(0.0, 0.0, winWidth, winHeight);  // 标准矩形裁剪窗口对象
int iKeyPointNum = 0;                                    // 键盘'p'确定直线段端点的数目：0-无、1-起始点、2-终止点
int iMousePointNum = 0;                                  // 鼠标单击确定裁剪窗口点的数目：0-无、1-起始点、2-终止点

/*
利用键盘橡皮筋技术交互绘制要裁剪的直线段，键盘’p’确定直线段端点；
利用鼠标橡皮筋技术交互绘制裁剪窗口，鼠标左键单击确定裁剪窗口主对角线位置。
*/

int lineX[3], lineY[3];  // 记录用键盘'p'确定的直线段的两个端点的坐标值
bool keyFinished = false;
int rectX[3], rectY[3];  // 记录用鼠标单击确定裁剪窗口的对角线的两个端点的坐标值
vector<point> points;

void refreshPoints() {
    points.clear();
    points.push_back(point(lineX[1], lineY[1]));
    points.push_back(point(lineX[2], lineY[2]));
}

void setWinObj() {
    winObj.w_xmin = min(rectX[1], rectX[2]);
    winObj.w_xmax = max(rectX[1], rectX[2]);
    winObj.w_ymin = min(rectY[1], rectY[2]);
    winObj.w_ymax = max(rectY[1], rectY[2]);
}

// 初始化函数
void init() {
    // 设置背景颜色
    glClearColor(0.21875f, 0.046875f, 0.1640625f, 0.0f);
    // 设置绘制颜色为白色
    glColor3f(1.0, 1.0, 1.0);
}

void drawRect() {
    // 绘制矩形
    glBegin(GL_LINE_LOOP);
    glVertex2i(rectX[1], rectY[1]);
    glVertex2i(rectX[2], rectY[1]);
    glVertex2i(rectX[2], rectY[2]);
    glVertex2i(rectX[1], rectY[2]);
    glEnd();
}

// 显示函数
void display() {
    // 清除颜色缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (iKeyPointNum >= 1) {
        glBegin(GL_LINES);
        glVertex2i(points[0].x, points[0].y);
        glVertex2i(points[1].x, points[1].y);
        glEnd();
    }
    if (iMousePointNum >= 1) {
        drawRect();
    }
    glutSwapBuffers();
    glFlush();
}

// 窗口大小改变时的回调函数，设置视口和投影矩阵
void reshape(GLsizei w, GLsizei h) {
    // 设置视口大小和位置
    glViewport(0, 0, w, h);
    // 设置投影矩阵为正交投影
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, winWidth, 0.0, winHeight);
}

// 键盘事件处理函数
void keyboard(unsigned char key, int xMouse, int yMouse) {
    switch (key) {
        case 'p':
            switch (glutGetModifiers()) {
                case GLUT_DOWN:
                    if (iKeyPointNum == 0 || iKeyPointNum == 2) {
                        iKeyPointNum = 1;
                        lineX[1] = xMouse;
                        lineY[1] = winHeight - yMouse;
                        lineX[2] = lineX[1];
                        lineY[2] = lineY[1];
                        refreshPoints();
                    } else {
                        iKeyPointNum = 2;
                        lineX[2] = xMouse;
                        lineY[2] = winHeight - yMouse;
                        refreshPoints();
                    }
                    glutPostRedisplay();
                    break;
                case GLUT_UP:
                    // do nothing
                    break;
            }
            break;
        case 'e':
            keyFinished = true;
            refreshPoints();
            glutPostRedisplay();
            break;
        case 'c':
            if (iKeyPointNum == 2 && iMousePointNum == 2) {
                // 执行基于窗口的直线裁剪
                setWinObj();
                Line_Clipping(points, winObj);
                glutPostRedisplay();
            } else {
                cout << "Please set both line endpoints and window corners before clipping." << endl;
            }
            break;
        case 27:
            exit(0);
    }

    glutPostRedisplay();
}

void mouseMove(GLint xMouse, GLint yMouse)  // 处理鼠标移动的事件
{
    if (iKeyPointNum == 1)  // 判断当前是否已经确定了一个点
    {
        lineX[2] = xMouse;
        lineY[2] = winHeight - yMouse;
        refreshPoints();
        glutPostRedisplay();  // 通知系统需要重新绘制图形，这样就会实时地显示直线的变化，实现橡皮筋的效果
    }
    if (iMousePointNum == 1)  // 判断当前是否已经确定了一个点
    {
        rectX[2] = xMouse;
        rectY[2] = winHeight - yMouse;
        refreshPoints();
        glutPostRedisplay();
    }
}

void mousePlot(GLint button, GLint action, GLint xMouse, GLint yMouse) {
    if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN && keyFinished) {
        if (iMousePointNum == 0 || iMousePointNum == 2) {
            iMousePointNum = 1;
            rectX[1] = xMouse;
            rectY[1] = winHeight - yMouse;
            rectX[2] = rectX[1];
            rectY[2] = rectY[1];
        } else {
            iMousePointNum = 2;
            rectX[2] = xMouse;
            rectY[2] = winHeight - yMouse;
        }
    }
    if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN) {
        iMousePointNum = 0;
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Liang-Barsky");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouseMove);
    glutMouseFunc(mousePlot);

    init();
    glutMainLoop();
    return 0;
}
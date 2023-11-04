/*
g++ -fdiagnostics-color=always -g ./计算机图形学作业/03_ellipse.cpp ./计算机图形学作业/onMidPointEllipse.cpp -o ./计算机图形学作业/03_ellipse -lGL -lGLU -lglut
./计算机图形学作业/03_ellipse
*/

#include <GL/glut.h>
#include "onMidPointEllipse.h"

int pointNum = 0;
int x[3] = {0, 0, 0}, y[3] = {0, 0, 0};
int winWidth = 670, winHeight = 670;
static GLsizei iMode = 1;

void init(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
}

void changeSize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, winWidth, 0.0, winHeight);
}

void drawRectAndEllipse()
{
    // 计算矩形的宽和高
    int width = abs(x[2] - x[1]);
    int height = abs(y[2] - y[1]);

    // 计算矩形的中心坐标
    int cx = (x[1] + x[2]) / 2;
    int cy = (y[1] + y[2]) / 2;

    // 计算椭圆的长轴和短轴
    int a = width / 2;
    int b = height / 2;

    // 绘制矩形
    glBegin(GL_LINE_LOOP);
    glVertex2i(x[1], y[1]);
    glVertex2i(x[2], y[1]);
    glVertex2i(x[2], y[2]);
    glVertex2i(x[1], y[2]);
    glEnd();

    // 平移坐标系，使矩形中心对应原点
    glTranslated(cx, cy, 0);

    // 调用椭圆的中点 Bresenham 算法，绘制椭圆
    onMidPointEllipse(a, b);

    // 反平移坐标系，恢复原来的位置
    glTranslated(-cx, -cy, 0);
}

void drawEllipse()
{
    // 计算矩形的宽和高
    int width = abs(x[2] - x[1]);
    int height = abs(y[2] - y[1]);

    // 计算矩形的中心坐标
    int cx = (x[1] + x[2]) / 2;
    int cy = (y[1] + y[2]) / 2;

    // 计算椭圆的长轴和短轴
    int a = width / 2;
    int b = height / 2;

    // 平移坐标系，使矩形中心对应原点
    glTranslated(cx, cy, 0);

    // 调用椭圆的中点 Bresenham 算法，绘制椭圆
    onMidPointEllipse(a, b);

    // 反平移坐标系，恢复原来的位置
    glTranslated(-cx, -cy, 0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    switch (iMode)
    {
    case 1:
        glClear(GL_COLOR_BUFFER_BIT);
        if (pointNum == 1)
        {
            drawRectAndEllipse();
        }
        if (pointNum == 2)
        {
            drawEllipse();
        }
        glutSwapBuffers();
        break;
    case 2:
        if (pointNum >= 1)
        {
            glBegin(GL_LINES);
            glVertex2i(x[1], y[1]);
            glVertex2i(x[2], y[2]);
            glEnd();
        }
        glutSwapBuffers();
        break;
    default:
        break;
    }
    glFlush();
}

void processMenu(int value)
{
    iMode = value;
    glutPostRedisplay();
}

void mousePlot(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
    if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
    {
        if (pointNum == 0 || pointNum == 2)
        {
            pointNum = 1;
            x[1] = xMouse;
            y[1] = winHeight - yMouse;
        }
        else
        {
            pointNum = 2;
            x[2] = xMouse;
            y[2] = winHeight - yMouse;
            // 增加以下代码，用于检测SHIFT键的状态
            int modifier = glutGetModifiers();
            if (modifier == GLUT_ACTIVE_SHIFT) // 如果按下SHIFT键
            {
                // 使x[2]和y[2]的距离相等，形成正方形
                int dx = abs(x[2] - x[1]);
                int dy = abs(y[2] - y[1]);
                int d = min(dx, dy); // 取最小的距离
                if (x[2] > x[1])     // 根据x[2]和x[1]的相对位置，调整x[2]的值
                {
                    x[2] = x[1] + d;
                }
                else
                {
                    x[2] = x[1] - d;
                }
                if (y[2] > y[1]) // 根据y[2]和y[1]的相对位置，调整y[2]的值
                {
                    y[2] = y[1] + d;
                }
                else
                {
                    y[2] = y[1] - d;
                }
            }
            glutPostRedisplay();
        }
    }
    if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN)
    {
        pointNum = 0;
        glutPostRedisplay();
    }
}

void mouseMove(GLint xMouse, GLint yMouse) // 处理鼠标移动的事件
{
    if (pointNum == 1) // 判断当前是否已经确定了一个点
    {
        x[2] = xMouse;
        y[2] = winHeight - yMouse;
        glutPostRedisplay(); // 通知系统需要重新绘制图形，这样就会实时地显示直线的变化，实现橡皮筋的效果
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Rubber Ellipse");

    int nMainMenu = glutCreateMenu(processMenu); // 创建主菜单
    glutAddMenuEntry("Ellipse", 1);
    glutAddMenuEntry("Line", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutDisplayFunc(display);
    glutReshapeFunc(changeSize);
    glutMouseFunc(mousePlot);
    glutPassiveMotionFunc(mouseMove);

    init();
    glutMainLoop();
    return 0;
}
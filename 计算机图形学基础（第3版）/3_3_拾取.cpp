/* OpenGL实现拾取操作的例子，演示拾取操作的过程，其中拾取窗口的宽度和高度都指定为10 */
#include <GL/glut.h>
#include "stdio.h"

const GLint pickSize = 32;
int winWidth = 400, winHeight = 300;

void Initial(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void DrawRect(GLenum mode) // 绘制三个矩形图元
// 接受一个参数 mode，表示绘制模式，可以是 GL_RENDER 或 GL_SELECT
// 如果是 GL_RENDER，则正常绘制图元
// 如果是 GL_SELECT，则在绘制每个图元之前，用 glPushName 函数将一个唯一的名字压入名字堆栈，这个名字用于标识图元，方便后续的拾取操作
{
    if (mode == GL_SELECT)
        glPushName(1); // 压入堆栈
    glColor3f(1.0f, 0.0f, 0.0f);
    glRectf(60.0f, 50.0f, 150.0f, 150.0f);

    if (mode == GL_SELECT)
        glPushName(2); // 压入堆栈
    glColor3f(0.0f, 1.0f, 0.0f);
    glRectf(230.0f, 50.0f, 330.0f, 150.0f);

    if (mode == GL_SELECT)
        glPushName(3); // 压入堆栈
    glColor3f(0.0f, 0.0f, 1.0f);
    glRectf(140.0f, 140.0f, 240.0f, 240.0f);
}

void ProcessPicks(GLint nPicks, GLuint pickBuffer[]) // 处理拾取结果
// 接受两个参数，nPicks 表示选中的图元的个数，pickBuffer 表示选择缓冲区，用于存储拾取信息
// 这个函数遍历选择缓冲区，根据名字堆栈的位置和图元的名字，输出用户选择了哪些图元
{
    GLint i;
    GLuint name, *ptr;
    printf("选中的数目为%d个\n", nPicks);
    ptr = pickBuffer;
    for (i = 0; i < nPicks; i++)
    {
        name = *ptr;     // 选中图元在堆栈中的位置
        ptr += 3;        // 跳过名字和深度信息
        ptr += name - 1; // 根据位置信息获得选中的图元名字
        if (*ptr == 1)
            printf("你选择了红色图元\n");
        if (*ptr == 2)
            printf("你选择了绿色图元\n");
        if (*ptr == 3)
            printf("你选择了蓝色图元\n");
        ptr++;
    }
    printf("\n\n");
}

void ChangeSize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, winWidth, 0.0, winHeight);
}

void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    DrawRect(GL_RENDER);
    glFlush();
}

void MousePlot(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
    GLuint pickBuffer[pickSize];
    GLint nPicks, vp[4];

    if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
    {
        glSelectBuffer(pickSize, pickBuffer);   // 设置选择缓冲区的大小和地址
        glRenderMode(GL_SELECT);                // 激活选择模式，这个模式下，OpenGL不会真正绘制图元，而是将拾取信息存储在选择缓冲区中
        glInitNames();                          // 初始化名字堆栈，这个堆栈用于存储图元的名字
        glMatrixMode(GL_PROJECTION);            // 切换到投影矩阵模式
        glPushMatrix();                         // 将当前的投影矩阵压入堆栈，以便后续恢复
        glLoadIdentity();                       // 将投影矩阵设置为单位矩阵
        glGetIntegerv(GL_VIEWPORT, vp);         // 获取当前的视口大小，存储在 vp 数组中
        // 定义一个10×10的选择区域，这个区域以鼠标的位置为中心，用于限制拾取操作的范围，只有在这个区域内的图元才会被选中
        gluPickMatrix(GLdouble(xMouse), GLdouble(vp[3] - yMouse), 10.0, 10.0, vp);
        gluOrtho2D(0.0, winWidth, 0.0, winHeight);  // 设置一个正交投影，这个投影的范围与窗口的大小一致，用于将图元映射到屏幕坐标系
        DrawRect(GL_SELECT);            // 绘制三个矩形图元，这个过程中，OpenGL会将图元的名字和深度信息存储在选择缓冲区中
        // 恢复投影变换
        glMatrixMode(GL_PROJECTION);    // 切换到投影矩阵模式
        glPopMatrix();                  // 之前压入堆栈的投影矩阵弹出，恢复原来的投影变换
        glFlush();                      // 将图形命令送往图形管线

        // 获得选择集并输出
        nPicks = glRenderMode(GL_RENDER);   // 恢复正常的绘制模式，这个函数的返回值就是选中的图元的个数，存储在 nPicks 变量中
        ProcessPicks(nPicks, pickBuffer);   // 处理拾取结果，输出用户选择了哪些图元
        glutPostRedisplay();                // 通知OpenGL重新绘制场景
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Pick");
    glutDisplayFunc(Display);
    glutReshapeFunc(ChangeSize);
    glutMouseFunc(MousePlot);
    Initial();
    glutMainLoop();
    return 0;
}
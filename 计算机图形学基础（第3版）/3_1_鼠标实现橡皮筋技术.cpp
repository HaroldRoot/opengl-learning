/* 程序3-1 OpenGL中利用鼠标实现橡皮筋技术的例子 */
#include <GL/glut.h>
int iPointNum = 0;
// 表示当前已经确定的点的个数
// 初始值为0，当用户点击鼠标左键时，该值会加1
// 当用户点击鼠标右键时，该值会重置为0
int x1 = 0, x2 = 0, y1 = 0, y2 = 0; /* y1 是一个内建函数，对某些编译器而言可能会导致冲突或混淆 */
// 分别表示直线的两个端点的横纵坐标
// 当用户点击鼠标左键时，会根据鼠标的位置更新这些值
int winWidth = 400, winHeight = 300;
// 分别表示窗口的宽度和高度
// 当用户改变窗口的大小时，会根据窗口的新大小更新这些值

void Initial(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    /*
    与“简单矩形”相比，glMatrixMode 和 gluOrtho2D
    被移到了 ChangeSize 函数中
    */
}

void ChangeSize(int w, int h) // 处理窗口大小变化的事件
{
    winWidth = w; // 更新全局变量 winWidth 和 winHeight 的值
    winHeight = h; // 参数 w 和 h 分别表示窗口的新宽度和高度，单位是像素
    glViewport(0, 0, w, h); // 设置视口的位置和大小
    // 参数分别表示视口的左下角的横纵坐标和视口的宽度和高度，单位是像素
    // 视口是窗口中用于显示图形的区域，一般和窗口的大小相同
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // 将当前矩阵重置为单位矩阵，也就是没有任何变换的矩阵
    gluOrtho2D(0.0, winWidth, 0.0, winHeight);
}

void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);
    if (iPointNum >= 1) // 判断当前是否已经确定了至少一个点
    {
        glBegin(GL_LINES); // 开始绘制一个图元，参数表示绘制的图元的类型，这里是直线
        glVertex2i(x1, y1); // 指定直线的两个端点的坐标
        glVertex2i(x2, y2); // 参数分别表示横纵坐标，单位是像素
        glEnd(); // 结束绘制图元，表示已经指定了所有的顶点
    }
    glutSwapBuffers(); // 交换颜色缓冲区，也就是将绘制好的图形显示在窗口中
    // 这里使用了双缓冲技术，可以避免图形的闪烁和撕裂现象
}

void MousePlot(GLint button, GLint action, GLint xMouse, GLint yMouse) // 处理鼠标点击的事件
{
    if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) //判断是否是鼠标左键按下的事件
    {
        if (iPointNum == 0 || iPointNum == 2) //判断当前是否没有点或者已经有两个点
        {
            iPointNum = 1; // 将点的个数设置为 1，表示开始绘制一条新的直线
            x1 = xMouse; // 将第一个点的坐标设置为鼠标的位置
            y1 = winHeight - yMouse; // OpenGL 的坐标系和窗口的坐标系的 y 轴方向是相反的
        }
        else
        {
            iPointNum = 2; // 将点的个数设置为2，表示已经确定了两个点
            x2 = xMouse; // 将第二个点的坐标设置为鼠标的位置
            y2 = winHeight - yMouse; // 同样需要注意y轴的方向
            glutPostRedisplay(); // 通知系统需要重新绘制图形，这样就会调用 Display 函数来显示直线
        }
    }
    if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN) // 判断是否是鼠标右键按下的事件
    {
        iPointNum = 0; // 将点的个数设置为0，表示取消绘制直线
        glutPostRedisplay(); // 通知系统需要重新绘制图形，这样就会将窗口的背景颜色恢复为白色，擦除直线
    }
}

void PassiveMouseMove(GLint xMouse, GLint yMouse) // 处理鼠标移动的事件
{
    if (iPointNum == 1) // 判断当前是否已经确定了一个点
    {
        x2 = xMouse;
        y2 = winHeight - yMouse;
        glutPostRedisplay(); // 通知系统需要重新绘制图形，这样就会实时地显示直线的变化，实现橡皮筋的效果
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Rubber Band Tech");
    glutDisplayFunc(Display);

    glutReshapeFunc(ChangeSize); // 注册一个回调函数，当窗口的大小发生变化时，就会调用这个函数
    glutMouseFunc(MousePlot); // 注册一个回调函数，当鼠标发生点击事件时，就会调用这个函数
    glutPassiveMotionFunc(PassiveMouseMove); // 注册一个回调函数，当鼠标发生移动事件时，就会调用这个函数
    
    Initial();
    glutMainLoop();
    return 0;
}
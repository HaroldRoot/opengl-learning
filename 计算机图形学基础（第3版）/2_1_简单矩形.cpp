/* 程序2-1 OpenGL绘制矩形的简单例子 */
#include <GL/freeglut.h>
void Initial(void) // 初始化OpenGL的状态和参数
{   
    // 设置窗口的背景颜色为白色
    // 参数分别表示红、绿、蓝和透明度的值
    // 范围是0.0到1.0，其中1.0表示最大，0.0表示最小
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    // 设置当前矩阵模式为投影矩阵
    // 这个矩阵用于定义摄像机的视角和视野
    glMatrixMode(GL_PROJECTION);
    // 设置一个二维的正交投影
    // 参数分别表示左、右、下和上的坐标值
    // 这个函数会将指定的区域映射到窗口的整个区域
    gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}
void Display(void) // 绘制图形并显示在窗口中
{
    // 清除颜色缓冲区
    // 也就是将窗口的所有像素点都设置为背景颜色
    glClear(GL_COLOR_BUFFER_BIT);
    // 设置当前的颜色为红色
    // 参数分别表示红、绿、蓝的值
    // 范围是0.0到1.0，其中1.0表示最大，0.0表示最小
    glColor3f(1.0f, 0.0f, 0.0f);
    // 绘制一个矩形
    // 参数分别表示矩形的左下角和右上角的坐标值，单位是像素
    glRectf(50.0f, 100.0f, 150.0f, 50.0f);
    // 将所有的绘图命令发送到图形硬件，并等待它们执行完成
    glFlush();
}
int main(int argc, char *argv[]) // 执行程序的主要逻辑
{
    // 初始化freeglut库
    // 参数分别表示命令行参数的个数和值的指针
    glutInit(&argc, argv);
    // 设置窗口的显示模式
    // 参数表示使用单缓冲区和RGB颜色模式
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    // 设置窗口的大小
    // 参数分别表示窗口的宽度和高度，单位是像素
    glutInitWindowSize(400, 300);
    // 设置窗口的位置
    // 参数分别表示窗口的左上角相对于屏幕的横纵坐标，单位是像素
    glutInitWindowPosition(100, 120);
    // 创建一个窗口
    // 并给它一个字符串作为标题
    glutCreateWindow("Rectangle");
    // 注册一个回调函数
    // 当窗口需要重新绘制时，就会调用这个函数
    glutDisplayFunc(Display);
    // 调用Initial函数
    // 初始化OpenGL的状态和参数
    Initial();
    // 进入主循环
    // 等待用户的输入和事件的发生
    glutMainLoop();
    return 0;
}
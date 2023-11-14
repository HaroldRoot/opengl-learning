#include <GL/glut.h>

void initial() {
    glClearColor(1.0, 1.0, 1.0, 1.0);      // 设置背景颜色为白色
    glMatrixMode(GL_PROJECTION);           // 设置投影影矩阵为正交投影
    glLoadIdentity();                      // 清空投影影矩阵
    gluOrtho2D(-10.0, 10.0, -10.0, 10.0);  // 设置二维裁剪窗口的范围为 [-10, 10] x [-10, 10]
}

void triangle(GLsizei mode) {
    if (mode == 1)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // 多边形模式为线框
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // 多边形模式为填充多边形
    glBegin(GL_TRIANGLES);                          // 开始绘制三角形
    glVertex2f(0.0, 5.0);                           // 绘制第一个顶点
    glVertex2f(5.0, -5.0);                          // 绘制第二个顶点
    glVertex2f(-5.0, -5.0);                         // 绘制第三个顶点
    glEnd();                                        // 结束绘制三角形
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);  // 清除颜色缓冲区，即将背景颜色覆盖到屏幕上

    glColor3f(1.0, 0.0, 0.0);    // 设置颜色为红色
    glViewport(0, 0, 200, 200);  // 设置视口大小为长宽均为200的的矩形区域，从左上角开始计算坐标系原点（x = 0，y = 0）
    triangle(1);                 // 绘制第一个多边形，模式为线框，颜色为红色

    glColor3f(0.0, 0.0, 1.0);      // 设置颜色为为蓝色
    glViewport(200, 0, 200, 200);  // 设置视口大小为长宽均为200的的矩形区域，从右上角开始计算坐标系原点（x = 200，y = 0）
    triangle(2);                   // 绘制第二个多边形，模式为填充多边形，颜色为为绿色

    glFlush();  // 刷新缓冲区，即将绘制的内容显示到屏幕上
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);                        // 初始化GLUT库，传入命令行参数argc和argv数组中的元素作为参数值
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  // 设置显示模式为单一窗口RGB模式，即只有一个窗口显示图像，并且使用RGB颜色深度模式
    glutInitWindowPosition(100, 100);             // 设置窗口位置在屏幕左上角（x =100，y =100）
    glutInitWindowSize(400, 200);                 // 设置窗口大小为长宽均为400和200的正方形区域
    glutCreateWindow("Multi-View Area");          // 创建一个名为“Multi-View Area”的窗口，并返回其句柄（handle）

    initial();  // 调用initial函数初始化GLUT库和设置背景颜色和投影影矩阵

    glutDisplayFunc(Display);  // 注册Display函数作为显示函数（display function），即当窗口需要更新时调用该函数绘制图像

    glutMainLoop();  // 进入主循环（main loop），即当窗口需要更新时，调用glutMainLoop函数，该函数会自动调用Display函数和其他相关函数，直到用户关闭窗口或按下特定的键

    return 0;
}
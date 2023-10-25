#include <windows.h>
// #include <gl/gl.h>
// #include <gl/glu.h>
// 若使用GLUT库函数实现窗口管理
// 则不需要包含gl.h和glu.h
// 因为GLUT保证了这两者被正确包含
#include <gl/glut.h>

void Initial(void) {
	// 利用OpenGL绘图
	// 指定窗口的背景颜色
	glClearColor(0.98f, 0.625f, 0.12f, 1.0f);
	// 当OpenGL混合参数被激活时，Alpha值为0.0表示对象是完全透明的
	// Alpha值为1.0则表示对象是完全不透明的

	// OpenGL处理图形的方式是三维的
	// 二维矩形要在一个三维坐标空间中创建
	glMatrixMode(GL_PROJECTION);
	// gluOrtho2D指定使用正投影
	// 将一个x坐标在0.0~200.0，
	// y坐标在0.0~150.0的矩形坐标区域投影到窗口内
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
	// 任何在这个区域内绘制的图形都可以显示在窗口中
	// 任何坐标范围外的图形都不能显示
}

void Display(void) {
	// 用当前背景色填充窗口
	glClear(GL_COLOR_BUFFER_BIT);
	// 设置当前的绘图颜色为红色
	glColor3f(1.0f, 0.0f, 0.0f);
	// 绘制一个左上角点在(50.0,100.0)
	// 右下角点在(150.0,50.0)的矩形
	glRectf(50.0f, 100.0f, 150.0f, 50.0f);
	// 清空OpenGL命令缓冲区
	// 强制执行命令缓冲区中的所有OpenGL()函数
	glFlush();
}

int main(int argc, char** argv) {
	// 要使用GLUT库首先需要进行初始化
	// 对命令行的参数进行处理
	glutInit(&argc, argv);

	// 使用glutInitDisplayMode函数设定窗口的显示模式
	// 包括缓存和颜色模型等
	// 下面的函数指令了窗口使用单缓存以及RGB颜色模型
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	// 使用glutInitWindowSize和glutInitWindowPosition函数指定显示窗口的大小和位置
	glutInitWindowSize(100, 300);
	glutInitWindowPosition(100, 120);

	glutCreateWindow("矩形");

	// 此时还需要定义窗口显示的内容。
	// 通常，利用OpenGL绘制图形的过程将显示内容定义在一个不带任何参数的函数内，
	// 如果这个函数的名字是Display，则通过函数
	glutDisplayFunc(Display);

	Initial();

	// 需要调用glutMainLoop函数使窗口框架运行起来
	// 使设置的显示回调函数开始工作，直到用户终止程序为止
	glutMainLoop();

	return 0;
}
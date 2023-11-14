#include <GL/glut.h>  // 引入GLUT库，用于创建图形用户界面

void Initial();                 // 定义一个函数Initial，用于初始化图形环境和设置背景颜色
void ChangeSize(int w, int h);  // 定义一个函数ChangeSize，用于改变窗口的大小
void Display();                 // 定义一个函数Display，用于绘制三个球体和控制时间间隔
void TimerFunc(int value);      // 定义一个函数TimerFunc，用于设置定时器函数和参数值

int main(int argc, char** argv) {                              // 定义主函数main，用于接收命令行参数和调用其他函数
    glutInit(&argc, argv);                                     // 初始化GLUT库，传入命令行参数argc和argv数组中的元素作为参数值
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  // 设置显示模式为双精度浮点数RGB深度模式，即使用高精度的颜色、深度和视角信息
    glutCreateWindow("Example of Molecular Animation");        // 创建一个名为“Example of Molecular Animation”的窗口，并返回其句柄（handle）

    glutReshapeFunc(ChangeSize);  // 注册ChangeSize函数作为窗口大小改变的回调函数（callback function），即当窗口大小改变时调用该函数重新设置视口大小
    glutDisplayFunc(Display);     // 注册Display函数作为显示函数（display function），即当窗口需要更新时调用该函数绘制图像

    glutTimerFunc(500, TimerFunc, 1);  // 设置定时器函数TimerFunc每隔500毫秒执行一次，并传入参数值1作为参数值，即每隔0.5秒执行一次TimerFunc函数

    Initial();  // 调用Initial函数初始化GLUT库和设置背景颜色和深度测试模式

    glutMainLoop();  // 进入主循环（main loop），即当窗口需要更新时，调用glutMainLoop函数，该函数会自动调用Display函数和其他相关函数，直到用户关闭窗口或按下特定的键

    return 0;  // 返回0，表示程序正常结束
}

void Initial() {               // 定义Initial函数初始化GLUT库和设置背景颜色和深度测试模式
    glEnable(GL_DEPTH_TEST);   // 开启深度测试模式，即在绘制图像时考虑到光线的反射和折射效果
    glClearColor(1, 1, 1, 1);  // 设置背景颜色为白色（alpha = 1）
}

void ChangeSize(int w, int h) {  // 定义ChangeSize函数改变窗口的大小，并传入参数w和h作为参数值，即w表示水平方向的宽度，h表示示垂直方向的高度
    if (h == 0) {                // 如果h等于0，则将h设为1，表示窗口不变宽度但变高度
        h = 1;
    }
    glViewport(0, 0, w, h);       // 设置视口大小为长宽均为w和h的的矩形区域，从左上角开始计算坐标系原点（x = 0，y = 0）
    glMatrixMode(GL_PROJECTION);  // 设置投影模式为正交投影模式（orthographic projection mode），即只显示平面上的几何形形状，并且不考虑透视效果
    glLoadIdentity();             // 清空投影影矩阵（projection matrix）

    GLfloat fAspect;                      // 定义一个浮点型变量fAspect来存存储水平方向与与垂直方向之比（aspect ratio）
    fAspect = (float)w / (float)h;        // 计算fAspect的值，并赋值给fAspect变量
    gluPerspective(45, fAspect, 1, 500);  // 设置正交投影（perspective projection mode），即根据fAspect的值，设置一个透视效果，使得远处的物体看起来更小，近处的物体看起来更大，并且保持水平方向和和垂直方向的比例不变

    glMatrixMode(GL_MODELVIEW);  // 设置模型视图模式为正交投影模式（orthographic projection mode）
    glLoadIdentity();            // 清空模型视图图矩阵（model view matrix）
}

void Display() {                                         // 定义Display函数绘制三个球体和控制时间间隔
    static float fElect1 = 0;                            // 定义一个静态变量fElect1来存存储当前旋转角度（rotation angle）
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 清除颜色缓冲区和深度缓冲区，即将背景颜色和深度信息覆盖到屏幕上
    glMatrixMode(GL_MODELVIEW);                          // 设置模型视图模式为正交投影模式（orthographic projection mode）
    glLoadIdentity();                                    // 清空模型视图图矩阵（model view matrix）

    glTranslatef(0, 0, -250);     // 将坐标系原点移动到(0, 0, -250)处，即向下移动250个单位长度
    glColor3f(1, 0, 0);           // 设置颜色为红色
    glutSolidSphere(12, 15, 15);  // 绘制一个半径为12，高度为15，直径为15的立方体（sphere）
    glColor3f(0, 0, 0);           // 设置颜色为黑色

    glPushMatrix();               // 开始保存当前的模型视图图矩阵（model view matrix）
    glRotatef(fElect1, 0, 1, 0);  // 将当前的旋转角度fElect1旋转到(0, 1, 0)方向上，即顺时针旋转90度
    glTranslatef(90, 0, 0);       // 将坐标系原点移动到(90, 0, 0)处，即向右移动90个单位长度
    glutSolidSphere(6, 15, 15);   // 绘制一个半径为6，高度为15，直径为15的立方体（sphere）
    glPopMatrix();                // 结束保存当前的模型视图图矩阵（model view matrix）

    glPushMatrix();               // 开始保存当前的模型视图图矩阵（model view matrix）
    glRotatef(45, 0, 0, 1);       // 将当前的旋转角度45旋转到(0, 0, 1)方向上，即顺时针旋转45度
    glRotatef(fElect1, 0, 1, 0);  // 将当前的旋转角度fElect1旋转到(1, 0)方向上，即顺时针旋转45度
    glTranslatef(-70, 0, 0);      // 将坐标系原点移动到(-70, 0, 0)处，即向左移动70个单位长度
    glutSolidSphere(6, 15, 15);   // 绘制一个半径为6，高度为15，直径为15的立方体（sphere）
    glPopMatrix();                // 结束保存当前的模型视图图图矩阵（model view matrix）
    
    glPushMatrix();               // 开始保存当前的模型视图图图矩阵（model view matrix）
    glRotatef(-45, 0, 0, 1);      // 将当前的旋转角度-45旋转到(0, 0, 1)方向上，即顺时针旋转-45度
    glRotatef(fElect1, 0, 1, 0);  // 将当前的旋转角度fElect1旋转到(1, 0)方向上，即顺时针旋转-45度
    glTranslatef(0, 0, 60);       // 将坐标系原点移动到(0, 0, 60)处，即向下移动60个单位长度
    glutSolidSphere(6, 15, 15);   // 绘制一个半径为6，高度为15，直径为15的立方体（sphere）
    glPopMatrix();                // 结束保存当前的模型视图图图图矩阵（model view matrix）

    fElect1 += 10;        // 将fElect1加上10，表示增加旋转角度10度
    if (fElect1 > 360) {  // 如果fElect1大于360，则将fElect1设为10，表示恢复旋转角度为10度
        fElect1 = 10;
    }
    glutSwapBuffers();  // 刷新缓冲区，即将绘制的内容显示到屏幕上
}

void TimerFunc(int value) {            // 定义TimerFunc函数设置定时器函数和参数值
    glutPostRedisplay();               // 将窗口重新显示到屏幕上
    glutTimerFunc(100, TimerFunc, 1);  // 设置定时器函数TimerFunc每隔100毫秒执行一次，并传入参数值1作为参数值，即每隔0.1秒执行一次TimerFunc函数
}

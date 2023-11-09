#include <GL/freeglut.h>
#include <math.h>
#include <vector>
#include <string.h>

using namespace std;

// 定义最大迭代次数
const int MAX_ITER = 6;

// 定义顶点结构体，包含 x 和 y 坐标
struct vertex {
    GLfloat x, y;
};

// 定义一个二维向量，存储每一次迭代的顶点集合
vector<vertex> vertices[MAX_ITER + 1];

// 定义当前的迭代次数，初始为 0
int iter = 0;

// 初始化函数，设置背景颜色和初始的线段
void init(void) {
    // 设置背景颜色为黑色
    glClearColor(0.21875f, 0.046875f, 0.1640625f, 0.0f);

    // 定义初始的线段的起点和终点
    vertex start, end;
    start.x = 0.0;
    start.y = 400.0;
    end.x = 800.0;
    end.y = 400.0;

    // 将初始的线段的起点和终点加入到第 0 次迭代的顶点集合中
    vertices[0].push_back(start);
    vertices[0].push_back(end);
}

// 窗口大小改变时的回调函数，设置视口和投影矩阵
void changeSize(GLsizei w, GLsizei h) {
    // 设置视口大小和位置
    glViewport(0, 0, w, h);

    // 设置投影矩阵为正交投影
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 800.0, 0.0, 800.0);
}

// 计算函数，根据当前的迭代次数，计算出下一次迭代的顶点集合
void compute() {
    // 静态变量，作用域是整个函数，但生命周期是整个程序
    // 只会在第一次调用函数时被初始化，之后每次调用函数时都会保持上一次的值
    static auto record = 0;
    if (record < iter) {
        auto len = vertices[iter - 1].size();
        auto& prev = vertices[iter - 1];
        auto& curr = vertices[iter];
        for (auto i = 0; i < len - 1; i++) {
            // 将当前线段的起点加入到下一次迭代的顶点集合中
            curr.push_back(prev[i]);
            // 定义一个临时的顶点变量，用于存储新生成的顶点
            vertex tmp;
            // 计算当前线段的第一个三等分点，并加入到下一次迭代的顶点集合中
            tmp.x = prev[i].x + (prev[i + 1].x - prev[i].x) / 3;
            tmp.y = prev[i].y + (prev[i + 1].y - prev[i].y) / 3;
            curr.push_back(tmp);
            // 计算当前线段的中点的正三角形的顶点，并加入到下一次迭代的顶点集合中
            tmp.x = (prev[i].x + prev[i + 1].x) / 2 + (prev[i].y - prev[i + 1].y) * sqrt(3.0) / 6;
            tmp.y = (prev[i].y + prev[i + 1].y) / 2 + (prev[i + 1].x - prev[i].x) * sqrt(3.0) / 6;
            curr.push_back(tmp);
            // 计算当前线段的第二个三等分点，并加入到下一次迭代的顶点集合中
            tmp.x = prev[i].x + 2 * (prev[i + 1].x - prev[i].x) / 3;
            tmp.y = prev[i].y + 2 * (prev[i + 1].y - prev[i].y) / 3;
            curr.push_back(tmp);
        }
        // 将当前线段的终点加入到下一次迭代的顶点集合中
        curr.push_back(prev[len - 1]);
        // 更新上一次的迭代次数
        record = iter;
    }
}

// 线段绘制函数
void drawLine(int bias) {
    // 开始绘制线段
    glBegin(GL_LINE_STRIP);

    // 遍历当前迭代的顶点集合，将每个顶点作为顶点着色器的输入
    for (int i = 0; i < vertices[iter].size(); i++) {
        glVertex2f(vertices[iter][i].x, vertices[iter][i].y + bias);
    }

    // 结束绘制线段
    glEnd();
}

// 显示函数，绘制当前的迭代结果
void display(void) {
    // 清除颜色缓冲区
    glClear(GL_COLOR_BUFFER_BIT);

    // 调用计算函数，计算出当前的迭代结果
    compute();

    // 设置绘制颜色为白色
    glColor3f(1.0, 1.0, 1.0);

    // 绘制线段
    drawLine(0);
    // drawLine(160);
    // drawLine(320);
    // drawLine(-160);
    // drawLine(-320);
    // drawLine(-480);

    // 交换前后缓冲区，显示绘制结果
    glutSwapBuffers();
}

// 特殊键盘事件处理函数，根据用户的按键，改变迭代次数或退出程序
void key(int key, int x, int y) {
    switch (key) {
        // 如果按下右箭头键，迭代次数加一，但不超过最大迭代次数
        case GLUT_KEY_RIGHT:
            if (iter < MAX_ITER) {
                iter++;
            }
            break;
        // 如果按下左箭头键，迭代次数减一，但不小于零
        case GLUT_KEY_LEFT:
            if (iter > 0) {
                iter--;
            }
            break;
        // 如果按下 Esc 键，退出程序
        case 27:
            exit(0);
    }
    // 重新绘制窗口
    glutPostRedisplay();
}

// 主函数，初始化 GLUT 库，创建窗口，注册回调函数，进入主循环
int main(int argc, char** argv) {
    // 初始化 GLUT 库
    glutInit(&argc, argv);

    // 设置显示模式为双缓冲和 RGBA 颜色模式
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    // 设置窗口大小为 800 x 800 像素
    glutInitWindowSize(800, 800);

    // 设置窗口位置为屏幕中央
    glutInitWindowPosition(100, 100);

    // 创建窗口，标题为 Koch Curve
    glutCreateWindow("Koch Curve");

    // 调用初始化函数
    init();

    // 注册窗口大小改变时的回调函数
    glutReshapeFunc(changeSize);

    // 注册键盘事件的回调函数
    glutSpecialFunc(key);

    // 注册显示函数
    glutDisplayFunc(display);

    // 进入主循环
    glutMainLoop();

    return 0;
}

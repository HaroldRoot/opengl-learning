/*
g++ -g ./计算机图形学作业/08_bezier.cpp -o ./计算机图形学作业/08_bezier -lGL -lGLU -lglut
./计算机图形学作业/08_bezier
*/

#include "graphicType.h"
#include "lineClipping.h"

int iPointNum = 0;                   // 鼠标点击次数
int x1 = 0, x2 = 0, y2 = 0, y3 = 0;  // 鼠标两次点击的坐标
int mode = 0;                        // 确定模式
int winWidth = 40, winHeight = 300;
int xKey1 = 0, yKey1 = 0, xKey2 = 0, yKey2 = 0;  // 裁剪直线的两端点
int x3 = 0, y4 = 0;                              // 按一次p时直线另一端点的坐标
int iKeyPointNum = 0;                            // 按p的次数
int flag = 0, flag1 = 0;                         // 确定是否按了e，c键
vector<point> v1 = {0};                          // 存放各折线的端点
vector<point> v2 = {0};                          // 存放裁剪后的各折线端点
vector<point> pointVertex;                       // 控制多边形顶点序列
bool isEnd = false;                              // 标识控制多边形是否确定
const GLint ControlN = 4;                        // 由4个控制点定义三次bezier曲线段
color redColor(1, 0, 0), blackColor(0, 0, 0);    // 红色、黑色

void drawPolygonalLine(vector<point>& points, color& c) {  // 根据点序列向量数组points，绘制折线
    glColor3f(c.r, c.g, c.b);
    glEnable(GL_LINE_STIPPLE);  // 画折线
    glLineStipple(1, 0xFE);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < points.size(); i++) {
        glVertex2i(points[i].x, points[i].y);
    }
    glEnd();
}
// 绘制折线
void drawPolygonalLine(vector<point>& points, point& p, color& c) {  // 根据点序列向量数组points，绘制折线
    glColor3f(c.r, c.g, c.b);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0xFE);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < points.size(); i++) {
        glVertex2i(points[i].x, points[i].y);
    }
    glVertex2i(p.x, p.y);
    glEnd();
}
// 如果控制点数n<4，绘制一段n-1次Bezier曲线段，否则绘制一段三次Bezier曲线
void drawBezierCurve(vector<Point>& points, color& c) {
    GLfloat ControlP[4][3];  // 定义一个4*3的数组存放控制点的坐标信息，即xyz
    int iPointNum = points.size();
    int i;
    if (iPointNum < 4) {  // 控制点数<4，绘制一段n-1次Bezier曲线段
        for (i = 0; i < iPointNum; i++) {
            ControlP[i][0] = points[i].x;
            ControlP[i][1] = points[i].y;
            ControlP[i][2] = 0.0;
        }
    } else {
        for (i = 0; i < 4; i++) {  // 控制点数>=4，绘制一段三次Bezier曲线段
            ControlP[i][0] = points[i].x;
            ControlP[i][1] = points[i].y;
            ControlP[i][2] = 0.0;
        }
    }
    glColor3f(c.r, c.g, c.b);
    glPointSize(2);
    if (iPointNum > 4)
        glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, *ControlP);  // 定义一维取值器
    else
        glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, iPointNum, *ControlP);  // 定义一维取值器
    glEnable(GL_MAP1_VERTEX_3);
    glMapGrid1f(100, 0.0, 1.0);    // 生成均匀分布的一维网格参数值
    glEvalMesh1(GL_LINE, 0, 100);  // 绘制Bezier曲线
}

// 通过（m×3+1）控制点，绘制m段n-1次Bezier曲线段
void drawBezierCurve(vector<Point>& points, int n, Color& c) {
    vector<point> v;
    for (int i = 0, j = 0; i < n; i++, j++) {
        v.push_back(points[i]);
        if ((j + 1) % 3 == 0 && (points.size() - i - 1) > 1) {
            v.push_back(point((points[i].x + points[i + 1].x) / 2, (points[i].y + points[i + 1].y) / 2));
            j++;
        }
    }
    int m = (v.size() - 1) / 3;
    vector<point> v1(4);
    for (int i = 0, j = 0; j < m; j++, i++) {
        v1[0] = v[i * 3];
        v1[1] = v[i * 3 + 1];
        v1[2] = v[i * 3 + 2];
        v1[3] = v[i * 3 + 3];
        drawBezierCurve(v1, blackColor);
    }
    if (int k = (v.size() - 1) % 3 != 0) {
        for (int i = 0; i < k + 2; i++) {
            v1[i] = v[v.size() - k - i];
        }
        drawBezierCurve(v1, blackColor);
    }
}

int Clip_Top(float p, float q, float& umax, float& umin) {
    float r = 0.0;
    if (p < 0.0)  // 线段从裁剪窗口外部延伸到内部，取最大值r并更新umax
    {
        r = q / p;
        if (r > umin)
            return 0;  // umax>umin的情况，弃之
        else if (r > umax)
            umax = r;
    } else if (p > 0.0)  // 线段从裁剪窗口内部延伸到外部,取最小值r并更新umin
    {
        r = q / p;
        if (r < umax)
            return 0;  // umax>umin的情况，弃之
        else if (r < umin)
            umin = r;
    } else  // p=0时，线段平行于裁剪窗口
        if (q < 0.0)
            return 0;
    return 1;
}

void Line_Clipping(vector<point>& points, rect& winRect) {
    // 比较左、右边界，获得最大的umax
    point &p1 = points[0], &p2 = points[1];
    float dx = p2.x - p1.x, dy = p2.y - p1.y, umax = 0.0, umin = 1.0;
    point p = p1;
    if (Clip_Top(-dx, p1.x - winRect.w_xmin, umax, umin))     // 左边界
        if (Clip_Top(dx, winRect.w_xmax - p1.x, umax, umin))  // 右边界
            // 比较下、上边界，获得最小的umin
            if (Clip_Top(-dy, p1.y - winRect.w_ymin, umax, umin))     // 下边界
                if (Clip_Top(dy, winRect.w_ymin - p1.y, umax, umin))  // 上边界
                {                                                     // 求裁剪后直线新端点
                    p1.x = (int)(p.x + umax * dx);
                    p1.y = (int)(p.y + umax * dy);
                    p2.x = (int)(p.x + umin * dx);
                    p2.y = (int)(p.y + umin * dy);
                    return;
                }
    p1.x = 0;
    p1.y = 0;
    p2.x = 0;
    p2.y = 0;
}

void onMidPointEllispe(int a, int b) {
    int x, y;
    float d1, d2;
    glBegin(GL_POINTS);
    x = 0;
    y = b;
    d1 = b * b + a * a * (-b + 0.25);
    glVertex2i(x, y);
    glVertex2i(-x, -y);
    glVertex2i(-x, y);
    glVertex2i(x, -y);
    while (b * b * (x + 1) < a * a * (y - 0.5)) {
        if (d1 <= 0) {
            d1 += b * b * (2 * x + 3);
            x++;
        } else {
            d1 += b * b * (2 * x + 3) + a * a * (-2 * y + 2);
            x++;
            y--;
        }
        glVertex2f(x, y);
        glVertex2f(-x, -y);
        glVertex2f(-x, y);
        glVertex2f(x, -y);
    } /*while上半部分*/
    d2 = b * b * (x + 0.5) * (x + 0.5) + a * a * (y - 1) * (y - 1) - a * a * b * b;
    while (y > 0) {
        if (d2 <= 0) {
            d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3);
            x++;
            y--;
        } else {
            d2 += a * a * (-2 * y + 3);
            y--;
        }
        glVertex2f(x, y);
        glVertex2f(-x, -y);
        glVertex2f(-x, y);
        glVertex2f(x, -y);
    }
    glEnd();
}

void Initial(void) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void ChangeSize(int w, int h) {
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, winWidth, 0.0, winHeight);
}
void Display(void) {  // 显示空界面
    glClear(GL_COLOR_BUFFER_BIT);
}
void Displayoval(void) {  // 画椭圆
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);
    if (iPointNum == 1) {  // 显示辅助矩形
        glBegin(GL_LINE_STRIP);
        glVertex2i(x1, y2);
        glVertex2i(x2, y2);
        glVertex2i(x2, y3);
        glVertex2i(x1, y3);
        glVertex2i(x1, y2);
        glEnd();
    }
    glTranslated(x1 + (x2 - x1) / 2, y2 + (y3 - y2) / 2, 0.0);  // 以视窗的左下角为原点，将所画椭圆的中心平移到矩阵中心
    if (iPointNum >= 1) {                                       // 显示椭圆
        glBegin(GL_POINTS);
        onMidPointEllispe(abs(x2 - x1) / 2, abs(y3 - y2) / 2);
        glEnd();
    }
    glTranslated(-(x1 + (x2 - x1) / 2), -(y2 + (y3 - y2) / 2), 0.0);
    glutSwapBuffers();
}

void Displaylineclipping(void) {  // 完成裁剪直线
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    if (flag == 0 && flag1 == 1) {  // 没按e的时候
        glVertex2i((v1.back()).x, (v1.back()).y);
        glVertex2i(x3, y4);  // 实现各直线的橡皮筋效果
    }
    glEnd();
    if (flag == 0 || flag == 1) {  // 没按c键的时候
        glBegin(GL_LINE_STRIP);
        for (int i = 1; i < v1.size() - 1; i++) {  // 画出各折线
            glVertex2i(v1[i].x, v1[i].y);
            glVertex2i(v1[i + 1].x, v1[i + 1].y);
        }
        glEnd();
    } else if (flag == 2) {  // 按了c键，显示裁剪后的折线
        glBegin(GL_LINE_STRIP);
        for (int i = 1; i < v2.size() - 1; i += 2) {
            glVertex2i(v2[i].x, v2[i].y);
            glVertex2i(v2[i + 1].x, v2[i + 1].y);
        }
        glEnd();
    }
    glBegin(GL_LINE_STRIP);  // 画裁剪矩形
    glVertex2i(x1, y2);
    glVertex2i(x2, y2);
    glVertex2i(x2, y3);
    glVertex2i(x1, y3);
    glVertex2i(x1, y2);
    glEnd();
    glutSwapBuffers();
}

void Displaycircle(void) {  // 画圆
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);
    int length = abs(x2 - x1);
    if (iPointNum == 1) {  // 显示辅助正方形
        glBegin(GL_LINE_STRIP);
        if (x1 > x2 && y2 < y3) {
            glVertex2i(x1, y2);
            glVertex2i(x1 - length, y2);
            glVertex2i(x1 - length, y2 + length);
            glVertex2i(x1, y2 + length);
            glVertex2i(x1, y2);
        } else if (x1 < x2 && y2 < y3) {
            glVertex2i(x1, y2);
            glVertex2i(x1 + length, y2);
            glVertex2i(x1 + length, y2 + length);
            glVertex2i(x1, y2 + length);
            glVertex2i(x1, y2);
        } else if (x1 > x2 && y2 > y3) {
            glVertex2i(x1, y2);
            glVertex2i(x1 - length, y2);
            glVertex2i(x1 - length, y2 - length);
            glVertex2i(x1, y2 - length);
            glVertex2i(x1, y2);
        } else {
            glVertex2i(x1, y2);
            glVertex2i(x1 + length, y2);
            glVertex2i(x1 + length, y2 - length);
            glVertex2i(x1, y2 - length);
            glVertex2i(x1, y2);
        }
        glEnd();
    }
    if (y3 > y2) {
        glTranslated(x1 + (x2 - x1) / 2, y2 + length / 2, 0.0);
    } else {
        glTranslated(x1 + (x2 - x1) / 2, y2 - length / 2, 0.0);
    }
    // 以视窗的左下角为原点，将所画椭圆的中心平移到矩阵中心
    if (iPointNum >= 1) {  // 显示圆
        glBegin(GL_POINTS);
        onMidPointEllispe(abs(x2 - x1) / 2, abs(x2 - x1) / 2);
        glEnd();
    }
    if (y3 > y2) {
        glTranslated(-(x1 + (x2 - x1) / 2), -(y2 + length / 2), 0.0);
    } else {
        glTranslated(-(x1 + (x2 - x1) / 2), -(y2 - length / 2), 0.0);
    }
    glutSwapBuffers();
}

void DisplayBezier(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    if (!isEnd) {  // 没按e的时候
        point p(x3, y4);
        drawPolygonalLine(pointVertex, p, blackColor);
    }
    if (isEnd) {  // 按e后
        drawPolygonalLine(pointVertex, blackColor);
        drawBezierCurve(pointVertex, pointVertex.size(), blackColor);
    }
    glutSwapBuffers();
}

void ProcessMenu(int value) {
    if (value == 1) {  // 选椭圆
        mode = 1;
        glutDisplayFunc(Display);
    } else if (value == 2) {  // 选圆
        mode = 2;
        glutDisplayFunc(Display);  // 先将界面置为空
    } else if (value == 3) {       // 选裁剪直线
        mode = 3;
        glutDisplayFunc(Displaylineclipping);
    } else if (value == 4) {
        mode = 4;
        glutDisplayFunc(DisplayBezier);
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (mode == 3) {
        switch (key) {
            case 'p': {
                if (iKeyPointNum % 2 == 0) {  // 按奇数下p时此时x，y为直线的始点
                    flag = 0;
                    xKey1 = x;
                    yKey1 = winHeight - y;
                    v1.push_back(point(xKey1, yKey1));
                    flag1 = 1;
                } else {  // 偶数下p时为直线的末点
                    flag = 0;
                    xKey2 = x;
                    yKey2 = winHeight - y;
                    v1.push_back(point(xKey2, yKey2));
                    glutPostRedisplay();
                }
                iKeyPointNum++;
                break;
            }
            case 'e': {
                flag = 1;
                glutPostRedisplay();
                break;
            }
            case 'c': {
                flag = 2;
                rect r = rect(x1 > x2 ? x2 : x1, y2 > y3 ? y3 : y2, x1 > x2 ? x1 : x2, y2 > y3 ? y2 : y3);
                vector<point> v3(2);  // 存放各直线裁剪后的端点
                for (int i = 1; i < v1.size() - 1; i++) {
                    v3[0] = v1[i];
                    v3[1] = v1[i + 1];
                    Line_Clipping(v3, r);  // 计算裁剪后的直线的首尾点并存入v中
                    v2.push_back(point(v3[0].x, v3[0].y));
                    v2.push_back(point(v3[1].x, v3[1].y));
                }
                glutPostRedisplay();  // 重绘裁剪后的直线
                break;
            }
            case 'r': {  // 清空界面功能
                v1.clear();
                v2.clear();
                v1.push_back(0);
                v2.push_back(0);
                x1 = x2 = y2 = y3 = 0;
                glutPostRedisplay();
            }
            default:
                break;
        }
    }
    if (mode == 4) {
        switch (key) {
            case 'p': {
                xKey1 = x;
                yKey1 = winHeight - y;
                pointVertex.push_back(point(xKey1, yKey1));
                glutPostRedisplay();
                break;
            }
            case 'e': {
                isEnd = true;
                glutPostRedisplay();
                break;
            }
            case 'c': {
                glutPostRedisplay();  // 重绘裁剪后的直线
                break;
            }
            case 'r': {  // 清空界面功能
                pointVertex.clear();
                isEnd = false;
                glutPostRedisplay();
            }
            default:
                break;
        }
    }
}

void MousePlot(GLint button, GLint action, GLint xMouse, GLint yMouse) {
    if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
        if (iPointNum == 0 || iPointNum == 2) {
            iPointNum = 1;
            x1 = xMouse;
            y2 = winHeight - yMouse;                                     // 确定直线段的第一个端点
            if (glutGetModifiers() == GLUT_ACTIVE_SHIFT && mode == 2) {  // 同时按下shift并且为圆菜单时显示圆
                glutDisplayFunc(Displaycircle);
            } else if (glutGetModifiers() != GLUT_ACTIVE_SHIFT && mode == 2) {
                glutDisplayFunc(Display);
            } else if (mode == 1) {  // 显示椭圆
                glutDisplayFunc(Displayoval);
            }
        } else {
            iPointNum = 2;
            x2 = xMouse;
            y3 = winHeight - yMouse;  // 确定直线段的第二个端点
            glutPostRedisplay();      // 窗口重新绘制
        }
    }
    if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN) {
        iPointNum = 0;
        glutPostRedisplay();
    }
}

void PassiveMouseMove(GLint xMouse, GLint yMouse) {
    if (iPointNum == 1) {  // 实现按一次鼠标的橡皮筋效果
        x2 = xMouse;
        y3 = winHeight - yMouse;  // 将当前鼠标位置制定为直线的未固定端点。
        glutPostRedisplay();
    }
    if (flag == 0) {  // 按奇数次p时实现橡皮筋效果
        x3 = xMouse;
        y4 = winHeight - yMouse;
        glutPostRedisplay();
    }
    if (!isEnd) {
        x3 = xMouse;
        y4 = winHeight - yMouse;
        // glutPostRedisplay();
    }
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bezier");
    glutDisplayFunc(Display);
    glutReshapeFunc(ChangeSize);
    glutMouseFunc(MousePlot);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(PassiveMouseMove);
    glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("oval", 1);
    glutAddMenuEntry("circle", 2);
    glutAddMenuEntry("lineClipping", 3);
    glutAddMenuEntry("drawBezier", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    Initial();
    glutMainLoop();
    return 0;
}

/*
g++ -fdiagnostics-color=always -g ./计算机图形学作业/06_liang_barsky_extended.cpp ./计算机图形学作业/lineClipping.cpp -o ./计算机图形学作业/06_liang_barsky_extended -lGL -lGLU -lglut
./计算机图形学作业/06_liang_barsky_extended
*/

#include "lineClipping.h"

using namespace std;

int winWidth = 400;
int winHeight = 300;

int iKeyPointNum = 0;
int lineX[3], lineY[3];
vector<point> points;          // 存储当前直线的端点
vector<vector<point> > lines;  // 存储所有直线
bool keyFinished = false;
bool cut = false;

void refreshPoints() {
    points.clear();
    points.push_back(point(lineX[1], lineY[1]));
    points.push_back(point(lineX[2], lineY[2]));
}

int iMousePointNum = 0;
int rectX[3], rectY[3];
rect winObj = Rectangle(0.0, 0.0, winWidth, winHeight);

void setWinObj() {
    winObj.w_xmin = min(rectX[1], rectX[2]);
    winObj.w_xmax = max(rectX[1], rectX[2]);
    winObj.w_ymin = min(rectY[1], rectY[2]);
    winObj.w_ymax = max(rectY[1], rectY[2]);
}

void init() {
    glClearColor(0.21875f, 0.046875f, 0.1640625f, 0.0f);
    glColor3f(1.0, 1.0, 1.0);
}

void drawRect() {
    glBegin(GL_LINE_LOOP);
    glVertex2i(rectX[1], rectY[1]);
    glVertex2i(rectX[2], rectY[1]);
    glVertex2i(rectX[2], rectY[2]);
    glVertex2i(rectX[1], rectY[2]);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 绘制所有已绘制的直线
    for (const auto& line : lines) {
        glBegin(GL_LINES);
        glVertex2i(line[0].x, line[0].y);
        glVertex2i(line[1].x, line[1].y);
        glEnd();
    }

    // 绘制当前直线
    if (iKeyPointNum >= 1 && !cut && !keyFinished) {
        glBegin(GL_LINES);
        glVertex2i(points[0].x, points[0].y);
        glVertex2i(points[1].x, points[1].y);
        glEnd();
    }

    // 绘制矩形
    if (iMousePointNum >= 1) {
        drawRect();
    }

    glutSwapBuffers();
    glFlush();
}

void reshape(GLsizei w, GLsizei h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, winWidth, 0.0, winHeight);
}

void keyboard(unsigned char key, int xMouse, int yMouse) {
    switch (key) {
        case 'p':
            switch (glutGetModifiers()) {
                case GLUT_DOWN:
                    if (iKeyPointNum == 0) {
                        iKeyPointNum++;
                        lineX[1] = xMouse;
                        lineY[1] = winHeight - yMouse;
                        lineX[2] = lineX[1];
                        lineY[2] = lineY[1];
                        refreshPoints();
                    } else {
                        iKeyPointNum++;
                        lines.push_back(points);
                        lineX[1] = lineX[2];
                        lineY[1] = lineY[2];
                        refreshPoints();
                    }
                    glutPostRedisplay();
                    break;
                case GLUT_UP:
                    break;
            }
            break;
        case 'e':
            keyFinished = true;
            points.clear();
            glutPostRedisplay();
            break;
        case 'c':
            points.clear();
            cut = true;
            if (iKeyPointNum >= 2 && iMousePointNum == 2) {
                setWinObj();
                for (auto& line : lines) {
                    Line_Clipping(line, winObj);
                }
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

void mouseMove(GLint xMouse, GLint yMouse) {
    if (iKeyPointNum >= 1 && !keyFinished) {
        lineX[2] = xMouse;
        lineY[2] = winHeight - yMouse;
        refreshPoints();
        glutPostRedisplay();
    }
    if (iMousePointNum == 1) {
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
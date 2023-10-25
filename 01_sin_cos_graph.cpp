#include <GL/freeglut.h>
#include <cmath>

#define PI 3.14159265358979323846

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);

    //绘制正弦曲线
    //glPointSize(2.0);
    //glBegin(GL_POINTS);
    //for (float x = -PI; x <= PI; x += 0.01) {
    //    glVertex2f(x, sin(x));
    //}
    //glEnd();

    glBegin(GL_LINE_STRIP);
    for (float x = -PI; x <= PI; x += 0.01) {
        glVertex2f(x, sin(x));
    }
    glEnd();

    // 绘制余弦曲线
    glColor3f(0.0, 1.0, 0.0);

    //glBegin(GL_POINTS);
    //for (float x = -PI; x <= PI; x += 0.01) {
    //    glVertex2f(x, cos(x));
    //}
    //glEnd();

    glBegin(GL_LINE_STRIP);
    for (float x = -PI; x <= PI; x += 0.01) {
        glVertex2f(x, cos(x));
    }
    glEnd();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("折线模式");

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-PI, PI, -1.5, 1.5);

    glutDisplayFunc(display);
    glutMainLoop();
}
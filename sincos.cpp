#include <GL/glut.h>
#include <cmath>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_POINTS); // 点模式
    for (float x = -M_PI; x <= M_PI; x += 0.01) {
        glVertex2f(x, sin(x));
    }
    glEnd();

    glBegin(GL_LINE_STRIP); // 折线模式
    for (float x = -M_PI; x <= M_PI; x += 0.01) {
        glVertex2f(x, cos(x));
    }
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Sine and Cosine Curves");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

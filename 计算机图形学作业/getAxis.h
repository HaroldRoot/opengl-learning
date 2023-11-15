#include <GL/glut.h>
#include <string.h>

GLuint getAxis(float axisLength) {  // 根据轴长axisLength定义XYZ三维坐标系
    GLuint axisID = glGenLists(1);  // 获得一个有效的无符号整数作为列表ID

    static GLuint lists;

    glListBase(lists);              // 设置显示列表ID的起始位置
    glNewList(axisID, GL_COMPILE);  // 通过显示列表定义坐标系

    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);  // x轴，设置笔的颜色为red
    glBegin(GL_LINES);
    glVertex3f(-axisLength, 0.0, 0.0);
    glVertex3f(axisLength, 0.0, 0.0);
    glEnd();
    glRasterPos3f(axisLength, 0, 0);                  // 设置字符串"x"的起始位置
    glCallLists(strlen("x"), GL_UNSIGNED_BYTE, "x");  // 一次调用多个显示列表，显示字符串"x"

    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);  // y轴，设置笔的颜色为green
    glBegin(GL_LINES);
    glVertex3f(0.0, -axisLength, 0.0);
    glVertex3f(0.0, axisLength, 0.0);
    glEnd();
    glRasterPos3f(0, axisLength, 0);                  // 设置字符串"y"的起始位置
    glCallLists(strlen("y"), GL_UNSIGNED_BYTE, "y");  // 一次调用多个显示列表，显示字符串"y"

    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);  // z轴，设置笔的颜色为blue
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, -axisLength);
    glVertex3f(0.0, 0.0, axisLength);
    glEnd();
    glRasterPos3f(0, 0, axisLength);                  // 设置字符串"z"的起始位置
    glCallLists(strlen("z"), GL_UNSIGNED_BYTE, "z");  // 一次调用多个显示列表，显示字符串"z"

    glEndList();

    return axisID;
}
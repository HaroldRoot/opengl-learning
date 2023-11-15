#include <GL/glut.h>
#include <math.h>

/*
定义一个名为point3D的结构体，用来表示三维空间中的一个点。
它有三个成员变量x, y, z，分别表示点的x, y, z坐标。
它还有一个构造函数，用来初始化点的坐标为给定的参数a, b, c。
*/
typedef struct Point3D {
    double x, y, z;
    Point3D(double a = 0.0, double b = 0.0, double c = 0.0) {
        x = a;
        y = b;
        z = c;
    }
} point3D;

float PI = 3.14159f;

/*
第一个函数setLookAt根据menuValue的值调用不同的gluLookAt函数。
gluLookAt是OpenGL库中提供的一个函数，用来设置视图方向和投影影矩阵。
void gluLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, 
               GLdouble centerX, GLdouble centerY, GLdouble centerZ, 
               GLdouble upX, GLdouble upY, GLdouble upZ)
*/
void setLookAt(int menuValue, point3D eye, point3D at) {
    switch (menuValue) {
        case 1:
            gluLookAt(0, eye.y, 0, at.x, at.y, at.z, 0, 0, 1);
            break;
        case 2:
            gluLookAt(0, 0, eye.z, at.x, at.y, at.z, 0, -1, 0);
            break;
        case 3:
            gluLookAt(eye.x, 0, 0, at.x, at.y, at.z, 0, 0, 1);
            break;
    }
}

void setLookAt(int menuValue, float angle, point3D eye, point3D at) {
    GLfloat squareRootOf2 = 1.4142;
    GLdouble dcos, dsin, radius;
    float Rot = float(PI * angle / 180.0f);
    dcos = cos(Rot);
    dsin = sin(Rot);
    switch (menuValue) {
        case 1:
            radius = sqrt((eye.x * eye.x + eye.z * eye.z));
            gluLookAt(dcos * radius, eye.y, dsin * radius, at.x, at.y, at.z, 0, 1, 0);
            break;
        case 2:
            radius = sqrt((eye.x * eye.x + eye.y * eye.y));
            gluLookAt(dsin * radius, dcos * radius, eye.z, at.x, at.y, at.z, 0, 0, 1);
            break;
        case 3:
            radius = sqrt((eye.y * eye.y + eye.z * eye.z));
            gluLookAt(eye.x, dsin * radius, dcos * radius, at.x, at.y, at.z, 1, 0, 0);
            break;
    }
}

void setLookAt(const GLfloat x_angle, const GLfloat y_angle, const GLfloat z_angle, point3D eye, point3D at) {
    GLdouble dxc, dxs, dyc, dys, dzc, dzs;
    dxc = cos(PI * x_angle / 180);
    dxs = sin(PI * x_angle / 180);
    dyc = cos(PI * y_angle / 180);
    dys = sin(PI * y_angle / 180);
    dzc = cos(PI * z_angle / 180);
    dzs = sin(PI * z_angle / 180);
    GLdouble ux, uy, uz, vx, vy, vz, nx, ny, nz;
    ux = dyc * dzc;
    uy = dyc * dzs;
    uz = -dys;
    vx = dxs * dys * dzc - dxc * dzs;
    vy = dxs * dys * dzs + dxc * dzc;
    vz = dxs * dyc;
    nx = dxc * dys * dzc + dxs * dzs;
    ny = dxc * dys * dzs - dxs * dzc;
    nz = dxc * dyc;
    point3D camera;
    camera.x = eye.x * ux + eye.y * vx + eye.z * nx;
    camera.y = eye.x * uy + eye.y * vy + eye.z * ny;
    camera.z = eye.x * uz + eye.y * vz + eye.z * nz;
    gluLookAt(camera.x, camera.y, camera.z, at.x, at.y, at.z, vx, vy, vz);
}
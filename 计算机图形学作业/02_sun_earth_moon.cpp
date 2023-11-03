// 使用 freeglut 库
#include <GL/freeglut.h>
#include <math.h>

// 定义太阳、地球和月亮的半径
#define SUN_RADIUS 1
#define EARTH_RADIUS 0.2
#define MOON_RADIUS 0.06

// 定义地球和月亮的轨道半径
#define EARTH_ORBIT_RADIUS 1.5
#define MOON_ORBIT_RADIUS 0.3

// 定义地球和月亮的轨道速度
#define EARTH_ORBIT_SPEED 1
#define MOON_ORBIT_SPEED 3

// 定义地球和月亮的旋转速度
#define EARTH_ROTATE_SPEED 10
#define MOON_ROTATE_SPEED 5

// 定义地球和月亮的旋转角度和轨道角度
float earth_rotate_angle = 0;
float earth_orbit_angle = 0;
float moon_rotate_angle = 0;
float moon_orbit_angle = 0;

// 初始化 OpenGL 环境
void init()
{
    // 设置背景颜色为黑色
    glClearColor(0, 0, 0, 1);
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 启用光照
    glEnable(GL_LIGHTING);
    // 启用光源 0
    glEnable(GL_LIGHT0);
}

// 绘制太阳、地球和月亮的函数
void draw()
{
    // 清除颜色缓冲区和深度缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 设置投影矩阵为透视投影
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 100);
    // 设置视图矩阵为观察者在 (0, 0, 5) 的位置，看向原点，y 轴向上
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

    // 设置光源位置为 (2, 2, -2)
    GLfloat light_position[] = { 2, 2, -2, 1 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // 绘制太阳
    glPushMatrix();
    // 设置太阳的材质颜色为黄色，发光效果为白色
    GLfloat sun_diffuse[] = { 1, 1, 0 };            // 漫反射，表示光线照射到该材质上，经过漫反射后形成的光线强度（颜色）
    GLfloat sun_emission[] = { 0.9, 0.8, 0.5 };     // 由四个值组成，表示一种颜色。
    //OpenGL认为该材质本身就微微的向外发射光线，以至于眼睛感觉到它有这样的颜色，但这光线又比较微弱，以至于不会影响到其它物体的颜色。
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, sun_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, sun_emission);
    // 绘制一个半径为 SUN_RADIUS 的球体
    glutSolidSphere(SUN_RADIUS, 80, 80);
    glPopMatrix();

    // 绘制地球
    glPushMatrix();
    // 设置地球的材质颜色为蓝色，不发光
    GLfloat earth_diffuse[] = { 0, 0.5, 1 };
    GLfloat earth_emission[] = { 0, 0, 0.8 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, earth_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, earth_emission);
    // 沿 y 轴旋转 earth_orbit_angle 度，模拟地球绕太阳公转
    glRotatef(earth_orbit_angle, 0, 1, 0);
    // 沿 x 轴平移 EARTH_ORBIT_RADIUS 的距离，到达地球的轨道位置
    glTranslatef(EARTH_ORBIT_RADIUS, 0, 0);
    // 沿 y 轴旋转 earth_rotate_angle 度，模拟地球自转
    glRotatef(earth_rotate_angle, 0, 1, 0);
    // 绘制一个半径为 EARTH_RADIUS 的球体
    glutSolidSphere(EARTH_RADIUS, 40, 40);

    // 绘制月亮
    glPushMatrix();
    // 设置月亮的材质颜色为灰色，不发光
    GLfloat moon_diffuse[] = { 0, 0.5, 1 };
    GLfloat moon_emission[] = { 0.5, 0.5, 0.5 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, moon_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, moon_emission);
    // 沿 y 轴旋转 moon_orbit_angle 度，模拟月亮绕地球公转
    glRotatef(moon_orbit_angle, 0, 1, 0);
    // 沿 x 轴平移 MOON_ORBIT_RADIUS 的距离，到达月亮的轨道位置
    glTranslatef(MOON_ORBIT_RADIUS, 0, 0);
    // 沿 y 轴旋转 moon_rotate_angle 度，模拟月亮自转
    glRotatef(moon_rotate_angle, 0, 1, 0);
    // 绘制一个半径为 MOON_RADIUS 的球体
    glutSolidSphere(MOON_RADIUS, 20, 20);
    glPopMatrix();
    glPopMatrix();

    // 更新地球和月亮的旋转角度和轨道角度
    earth_rotate_angle += EARTH_ROTATE_SPEED;
    earth_orbit_angle += EARTH_ORBIT_SPEED;
    moon_rotate_angle += MOON_ROTATE_SPEED;
    moon_orbit_angle += MOON_ORBIT_SPEED;

    // 刷新窗口
    glutSwapBuffers();
}

// 主函数
int main(int argc, char** argv)
{
    // 初始化 GLUT 环境
    glutInit(&argc, argv);
    // 设置显示模式为双缓冲、RGB 颜色、带深度缓冲区
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    // 设置窗口大小和位置
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    // 创建窗口
    glutCreateWindow("Model of the motion of the Sun, Earth and Moon");
    // 初始化 OpenGL 环境
    init();
    // 注册绘图函数
    glutDisplayFunc(draw);
    // 注册空闲函数，使得窗口在没有事件时也会刷新，实现动画效果
    glutIdleFunc(draw);
    // 进入主循环
    glutMainLoop();
    return 0;
}
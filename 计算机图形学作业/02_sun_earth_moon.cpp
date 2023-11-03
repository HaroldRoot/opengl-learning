// ʹ�� freeglut ��
#include <GL/freeglut.h>
#include <math.h>

// ����̫��������������İ뾶
#define SUN_RADIUS 1
#define EARTH_RADIUS 0.2
#define MOON_RADIUS 0.06

// �������������Ĺ���뾶
#define EARTH_ORBIT_RADIUS 1.5
#define MOON_ORBIT_RADIUS 0.3

// �������������Ĺ���ٶ�
#define EARTH_ORBIT_SPEED 1
#define MOON_ORBIT_SPEED 3

// ����������������ת�ٶ�
#define EARTH_ROTATE_SPEED 10
#define MOON_ROTATE_SPEED 5

// ����������������ת�ǶȺ͹���Ƕ�
float earth_rotate_angle = 0;
float earth_orbit_angle = 0;
float moon_rotate_angle = 0;
float moon_orbit_angle = 0;

// ��ʼ�� OpenGL ����
void init()
{
    // ���ñ�����ɫΪ��ɫ
    glClearColor(0, 0, 0, 1);
    // ������Ȳ���
    glEnable(GL_DEPTH_TEST);
    // ���ù���
    glEnable(GL_LIGHTING);
    // ���ù�Դ 0
    glEnable(GL_LIGHT0);
}

// ����̫��������������ĺ���
void draw()
{
    // �����ɫ����������Ȼ�����
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // ����ͶӰ����Ϊ͸��ͶӰ
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 100);
    // ������ͼ����Ϊ�۲����� (0, 0, 5) ��λ�ã�����ԭ�㣬y ������
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

    // ���ù�Դλ��Ϊ (2, 2, -2)
    GLfloat light_position[] = { 2, 2, -2, 1 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // ����̫��
    glPushMatrix();
    // ����̫���Ĳ�����ɫΪ��ɫ������Ч��Ϊ��ɫ
    GLfloat sun_diffuse[] = { 1, 1, 0 };            // �����䣬��ʾ�������䵽�ò����ϣ�������������γɵĹ���ǿ�ȣ���ɫ��
    GLfloat sun_emission[] = { 0.9, 0.8, 0.5 };     // ���ĸ�ֵ��ɣ���ʾһ����ɫ��
    //OpenGL��Ϊ�ò��ʱ����΢΢�����ⷢ����ߣ��������۾��о���������������ɫ����������ֱȽ�΢���������ڲ���Ӱ�쵽�����������ɫ��
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, sun_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, sun_emission);
    // ����һ���뾶Ϊ SUN_RADIUS ������
    glutSolidSphere(SUN_RADIUS, 80, 80);
    glPopMatrix();

    // ���Ƶ���
    glPushMatrix();
    // ���õ���Ĳ�����ɫΪ��ɫ��������
    GLfloat earth_diffuse[] = { 0, 0.5, 1 };
    GLfloat earth_emission[] = { 0, 0, 0.8 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, earth_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, earth_emission);
    // �� y ����ת earth_orbit_angle �ȣ�ģ�������̫����ת
    glRotatef(earth_orbit_angle, 0, 1, 0);
    // �� x ��ƽ�� EARTH_ORBIT_RADIUS �ľ��룬�������Ĺ��λ��
    glTranslatef(EARTH_ORBIT_RADIUS, 0, 0);
    // �� y ����ת earth_rotate_angle �ȣ�ģ�������ת
    glRotatef(earth_rotate_angle, 0, 1, 0);
    // ����һ���뾶Ϊ EARTH_RADIUS ������
    glutSolidSphere(EARTH_RADIUS, 40, 40);

    // ��������
    glPushMatrix();
    // ���������Ĳ�����ɫΪ��ɫ��������
    GLfloat moon_diffuse[] = { 0, 0.5, 1 };
    GLfloat moon_emission[] = { 0.5, 0.5, 0.5 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, moon_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, moon_emission);
    // �� y ����ת moon_orbit_angle �ȣ�ģ�������Ƶ���ת
    glRotatef(moon_orbit_angle, 0, 1, 0);
    // �� x ��ƽ�� MOON_ORBIT_RADIUS �ľ��룬���������Ĺ��λ��
    glTranslatef(MOON_ORBIT_RADIUS, 0, 0);
    // �� y ����ת moon_rotate_angle �ȣ�ģ��������ת
    glRotatef(moon_rotate_angle, 0, 1, 0);
    // ����һ���뾶Ϊ MOON_RADIUS ������
    glutSolidSphere(MOON_RADIUS, 20, 20);
    glPopMatrix();
    glPopMatrix();

    // ���µ������������ת�ǶȺ͹���Ƕ�
    earth_rotate_angle += EARTH_ROTATE_SPEED;
    earth_orbit_angle += EARTH_ORBIT_SPEED;
    moon_rotate_angle += MOON_ROTATE_SPEED;
    moon_orbit_angle += MOON_ORBIT_SPEED;

    // ˢ�´���
    glutSwapBuffers();
}

// ������
int main(int argc, char** argv)
{
    // ��ʼ�� GLUT ����
    glutInit(&argc, argv);
    // ������ʾģʽΪ˫���塢RGB ��ɫ������Ȼ�����
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    // ���ô��ڴ�С��λ��
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    // ��������
    glutCreateWindow("Model of the motion of the Sun, Earth and Moon");
    // ��ʼ�� OpenGL ����
    init();
    // ע���ͼ����
    glutDisplayFunc(draw);
    // ע����к�����ʹ�ô�����û���¼�ʱҲ��ˢ�£�ʵ�ֶ���Ч��
    glutIdleFunc(draw);
    // ������ѭ��
    glutMainLoop();
    return 0;
}
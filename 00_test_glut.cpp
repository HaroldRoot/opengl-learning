#include <windows.h>
// #include <gl/gl.h>
// #include <gl/glu.h>
// ��ʹ��GLUT�⺯��ʵ�ִ��ڹ���
// ����Ҫ����gl.h��glu.h
// ��ΪGLUT��֤�������߱���ȷ����
#include <gl/glut.h>

void Initial(void) {
	// ����OpenGL��ͼ
	// ָ�����ڵı�����ɫ
	glClearColor(0.98f, 0.625f, 0.12f, 1.0f);
	// ��OpenGL��ϲ���������ʱ��AlphaֵΪ0.0��ʾ��������ȫ͸����
	// AlphaֵΪ1.0���ʾ��������ȫ��͸����

	// OpenGL����ͼ�εķ�ʽ����ά��
	// ��ά����Ҫ��һ����ά����ռ��д���
	glMatrixMode(GL_PROJECTION);
	// gluOrtho2Dָ��ʹ����ͶӰ
	// ��һ��x������0.0~200.0��
	// y������0.0~150.0�ľ�����������ͶӰ��������
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
	// �κ�����������ڻ��Ƶ�ͼ�ζ�������ʾ�ڴ�����
	// �κ����귶Χ���ͼ�ζ�������ʾ
}

void Display(void) {
	// �õ�ǰ����ɫ��䴰��
	glClear(GL_COLOR_BUFFER_BIT);
	// ���õ�ǰ�Ļ�ͼ��ɫΪ��ɫ
	glColor3f(1.0f, 0.0f, 0.0f);
	// ����һ�����Ͻǵ���(50.0,100.0)
	// ���½ǵ���(150.0,50.0)�ľ���
	glRectf(50.0f, 100.0f, 150.0f, 50.0f);
	// ���OpenGL�������
	// ǿ��ִ����������е�����OpenGL()����
	glFlush();
}

int main(int argc, char** argv) {
	// Ҫʹ��GLUT��������Ҫ���г�ʼ��
	// �������еĲ������д���
	glutInit(&argc, argv);

	// ʹ��glutInitDisplayMode�����趨���ڵ���ʾģʽ
	// �����������ɫģ�͵�
	// ����ĺ���ָ���˴���ʹ�õ������Լ�RGB��ɫģ��
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	// ʹ��glutInitWindowSize��glutInitWindowPosition����ָ����ʾ���ڵĴ�С��λ��
	glutInitWindowSize(100, 300);
	glutInitWindowPosition(100, 120);

	glutCreateWindow("����");

	// ��ʱ����Ҫ���崰����ʾ�����ݡ�
	// ͨ��������OpenGL����ͼ�εĹ��̽���ʾ���ݶ�����һ�������κβ����ĺ����ڣ�
	// ������������������Display����ͨ������
	glutDisplayFunc(Display);

	Initial();

	// ��Ҫ����glutMainLoop����ʹ���ڿ����������
	// ʹ���õ���ʾ�ص�������ʼ������ֱ���û���ֹ����Ϊֹ
	glutMainLoop();

	return 0;
}
#include <GL/glut.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include "OBJImpoter.cpp"

using namespace std;

float mynear = 0;//��׶�������
float myfar = 100.0;//��׶��Զ����
int winWidth, winHeight;//���ڿ��
int mouseX = 0;//���λ��x
int mouseY = 0;//���λ��y
float rotX = 0.0f;//��x����ת�Ƕ�
float rotY = 0.0f;//��y����ת�Ƕ�
float tx = 0.0f, ty = 0.0f, tz = 0.0f;//ƽ��
float scale = 1.0f;//����


void myReshape(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-5.0, 5.0, -5.0, 5.0, mynear, myfar);
	tz = -5.0f;
	glMatrixMode(GL_MODELVIEW);
}
void triangle(OBJImporter& obj, Face& f)
{
	auto& vertexList = obj.vertexList;//���������б�
	auto& normalList = obj.normalList;//�������б�
	auto& materialTable = obj.materialTable;//�����б�
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialTable[f.mtl_name
	].ambient);//���ò��ʵĻ�����
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialTable[f.mtl_name
	].diffuse);//���ò��ʵ��������
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialTable[f.mtl_name].specular);//���ò��ʵľ����
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialTable[f.mtl_name].shininess);//���ò��ʵľ���߹��

	glBegin(GL_TRIANGLES);//��ʼ����������

	
	glNormal3f(normalList[f.a].x, normalList[f.a].y, normalList[f.a].z);//������
	glVertex3f(vertexList[f.a].x, vertexList[f.a].y, vertexList[f.a].z);//��������


	glNormal3f(normalList[f.b].x, normalList[f.b].y, normalList[f.b].z);//������
	glVertex3f(vertexList[f.b].x, vertexList[f.b].y, vertexList[f.b].z);//��������


	glNormal3f(normalList[f.c].x, normalList[f.c].y, normalList[f.c].z);//������
	glVertex3f(vertexList[f.c].x, vertexList[f.c].y, vertexList[f.c].z);//��������

	glEnd();//��������������

}

void draw()
{

	static OBJImporter teapot("./newell_teaset/teapot.obj");//��ȡobj�ļ�
	glPushMatrix();//���浱ǰ����
	glTranslatef(-1.0f, -2.0f, 0.0f);//ƽ������ϵ

	for (auto& f : teapot.faceList) {
		triangle(teapot, f);
	}//���Ʋ��
	glPopMatrix();//�ָ���ǰ����

	static OBJImporter teacup("./newell_teaset/teacup.obj");//��ȡobj�ļ�
	glPushMatrix();//���浱ǰ����
	glTranslatef(2.0f, -2.0f, 0.0f);//ƽ������ϵ

	for (auto& f : teacup.faceList) {
		triangle(teacup, f);
	}//���Ʋ豭
	glPopMatrix();//�ָ���ǰ����
}

void scalefunction(float factor)
{
	scale *= factor;//
	glutPostRedisplay();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����ɫ����������Ȼ�����
	glLoadIdentity();//���õ�ǰ����
	glTranslatef(tx,ty,tz);//ƽ������ϵ
	glRotatef(rotX, 1.0f, 0.0f, 0.0f);//��x����ת
	glRotatef(rotY, 0.0f, 1.0f, 0.0f);//��y����ת
	glScalef(scale, scale, scale);//����
	// ������Դ
	glEnable(GL_LIGHTING);//���ù���
	glEnable(GL_LIGHT0);//���ù�Դ0
	glEnable(GL_LIGHT1);//���ù�Դ1
	glEnable(GL_NORMALIZE);//���÷�������һ��

	GLfloat light_position[] = { 5.0f, 5.0f, 5.0f, 1.0f };//���ù�Դλ��
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };//���û�������ɫ
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };//�������������ɫ
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };//���þ������ɫ

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);//�Թ���0���ò���

	GLfloat light_position1[] = { -5.0f, -5.0f, -5.0f, 1.0f };//���ù�Դλ��
	GLfloat light_ambient1[] = { 0.2f, 0.2f, 0.2f, 1.0f };//���û�������ɫ
	GLfloat light_diffuse1[] = { 1.0f, 1.0f, 1.0f, 1.0f };//�������������ɫ
	GLfloat light_specular1[] = { 1.0f, 1.0f, 1.0f, 1.0f };//���þ������ɫ

	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);//�Թ���1���ò���

	draw();//��������

	glutSwapBuffers();//˫����
}


void init(void)
{

	glEnable(GL_DEPTH_TEST);
}
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mouseX = x;
		mouseY = y;
	}
}

void motion(int x, int y)
{
	rotX += (y - mouseY) * 0.1f;
	rotY += (x - mouseX) * 0.1f;
	mouseX = x;
	mouseY = y;
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
		tz -= 0.1f;
		break;
	case 'a':
		tx -= 0.1f;
		break;
	case 's':
		tz+= 0.1f;
		break;
	case 'd':
		tx += 0.1f;
		break;
	case 'q':
		ty += 0.1f;
		break;
	case 'e':
		ty -= 0.1f;
		break;
	case'+':
		scalefunction(1.1f);
		break;
	case '-':
		scalefunction(0.9f);
		break;
	}
	glutPostRedisplay();
}
	
int main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Experiment 1");
	init();
	
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutMouseFunc(mouse); 
	glutMotionFunc(motion); 
	glutKeyboardFunc(keyboard); 
	glutMainLoop();
	return 0;
}

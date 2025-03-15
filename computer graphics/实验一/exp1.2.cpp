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

float mynear = 0;//视锥体近端面
float myfar = 100.0;//视锥体远端面
int winWidth, winHeight;//窗口宽高
int mouseX = 0;//鼠标位置x
int mouseY = 0;//鼠标位置y
float rotX = 0.0f;//绕x轴旋转角度
float rotY = 0.0f;//绕y轴旋转角度
float tx = 0.0f, ty = 0.0f, tz = 0.0f;//平移
float scale = 1.0f;//缩放


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
	auto& vertexList = obj.vertexList;//顶点坐标列表
	auto& normalList = obj.normalList;//法向量列表
	auto& materialTable = obj.materialTable;//材质列表
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialTable[f.mtl_name
	].ambient);//设置材质的环境光
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialTable[f.mtl_name
	].diffuse);//设置材质的漫反射光
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialTable[f.mtl_name].specular);//设置材质的镜面光
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialTable[f.mtl_name].shininess);//设置材质的镜面高光度

	glBegin(GL_TRIANGLES);//开始绘制三角形

	
	glNormal3f(normalList[f.a].x, normalList[f.a].y, normalList[f.a].z);//法向量
	glVertex3f(vertexList[f.a].x, vertexList[f.a].y, vertexList[f.a].z);//顶点坐标


	glNormal3f(normalList[f.b].x, normalList[f.b].y, normalList[f.b].z);//法向量
	glVertex3f(vertexList[f.b].x, vertexList[f.b].y, vertexList[f.b].z);//顶点坐标


	glNormal3f(normalList[f.c].x, normalList[f.c].y, normalList[f.c].z);//法向量
	glVertex3f(vertexList[f.c].x, vertexList[f.c].y, vertexList[f.c].z);//顶点坐标

	glEnd();//结束绘制三角形

}

void draw()
{

	static OBJImporter teapot("./newell_teaset/teapot.obj");//读取obj文件
	glPushMatrix();//保存当前矩阵
	glTranslatef(-1.0f, -2.0f, 0.0f);//平移坐标系

	for (auto& f : teapot.faceList) {
		triangle(teapot, f);
	}//绘制茶壶
	glPopMatrix();//恢复当前矩阵

	static OBJImporter teacup("./newell_teaset/teacup.obj");//读取obj文件
	glPushMatrix();//保存当前矩阵
	glTranslatef(2.0f, -2.0f, 0.0f);//平移坐标系

	for (auto& f : teacup.faceList) {
		triangle(teacup, f);
	}//绘制茶杯
	glPopMatrix();//恢复当前矩阵
}

void scalefunction(float factor)
{
	scale *= factor;//
	glutPostRedisplay();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色缓冲区和深度缓冲区
	glLoadIdentity();//重置当前矩阵
	glTranslatef(tx,ty,tz);//平移坐标系
	glRotatef(rotX, 1.0f, 0.0f, 0.0f);//绕x轴旋转
	glRotatef(rotY, 0.0f, 1.0f, 0.0f);//绕y轴旋转
	glScalef(scale, scale, scale);//缩放
	// 调整光源
	glEnable(GL_LIGHTING);//启用光照
	glEnable(GL_LIGHT0);//启用光源0
	glEnable(GL_LIGHT1);//启用光源1
	glEnable(GL_NORMALIZE);//启用法向量归一化

	GLfloat light_position[] = { 5.0f, 5.0f, 5.0f, 1.0f };//设置光源位置
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };//设置环境光颜色
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };//设置漫反射光颜色
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };//设置镜面光颜色

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);//对光照0设置参数

	GLfloat light_position1[] = { -5.0f, -5.0f, -5.0f, 1.0f };//设置光源位置
	GLfloat light_ambient1[] = { 0.2f, 0.2f, 0.2f, 1.0f };//设置环境光颜色
	GLfloat light_diffuse1[] = { 1.0f, 1.0f, 1.0f, 1.0f };//设置漫反射光颜色
	GLfloat light_specular1[] = { 1.0f, 1.0f, 1.0f, 1.0f };//设置镜面光颜色

	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);//对光照1设置参数

	draw();//绘制物体

	glutSwapBuffers();//双缓冲
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

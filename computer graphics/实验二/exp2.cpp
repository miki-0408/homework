#include <iostream>
#include <GL/glut.h>
#include <math.h>
using namespace std;
GLsizei winWidth = 800, winHeight = 600;// ���ڵĿ�Ⱥ͸߶�
GLfloat xComplexMin=-2.00, xComplexMax=0.50, yComplexMin=-1.25, yComplexMax=1.25;// ������ķ�Χ
GLfloat complexWidth=xComplexMax-xComplexMin, complexHeight=yComplexMax-yComplexMin;// ������Ŀ�Ⱥ͸߶�
GLint newX = 0, newY = 0;// �������λ��
const int timeInterval = 200;// ˢ��Ƶ��
const int numViews = 4;// ��ͼ����
GLint maxItermax[numViews] = { 50, 100, 150, 200 };// ����������
GLint maxIter[numViews] = { 0,0,0,0 };// ��ǰ��ͼ������������
int viewports[numViews][2] = { {0, winHeight / 2},{winWidth / 2, winHeight / 2},{0, 0},{winWidth / 2, 0} };// ��ͼ��λ��
class complexNum {
public:
	GLfloat x, y;
};
struct color {
	GLfloat r, g, b;
};
void init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0); // Set background color to black.
}
void plotPoint(complexNum z)
{
	glBegin(GL_POINTS);
		glVertex2f(z.x, z.y);
	glEnd();
}
complexNum complexSquare(complexNum z)
{
	complexNum zSquare;// ����ƽ��
	zSquare.x = z.x * z.x - z.y * z.y;// ʵ��ƽ��
	zSquare.y = 2 * z.x * z.y;// �鲿ƽ��
	return zSquare;// ���ظ���ƽ��
}
GLint mandelSqtransf(complexNum z0, GLint maxIter)
{
	complexNum z = z0;// ������ĸ��� z0 ��ֵ�� z��z �ǵ�ǰ�����ĸ���
	GLint count = 0;// ��ʼ����������������
	while (z.x * z.x + z.y * z.y <=4.0 && (count < maxIter))// ��������
	{
		z = complexSquare(z);//����ƽ��
		z.x += z0.x;// �� z0 ��ʵ���ӵ���ǰ���� z ��ʵ����
		z.y += z0.y;// �� z0 ���鲿�ӵ���ǰ���� z ���鲿��
		count++;// ���ӵ�������
	}
	return count;// ���ص�������
}
void mandelbrot(GLint nx, GLint ny, GLint maxIter)
{
	complexNum z, zIncr;// z �ǵ�ǰ�����ĸ���,zIncr ��ÿ�����ض�Ӧ�ĸ������������ڱ�������ƽ�档
	color ptColor;// ���ڴ洢��ǰ�����ɫ
	GLint itercount; //�ǵ�ǰ��ĵ���������������

	zIncr.x = complexWidth / GLfloat(nx);
	zIncr.y = complexHeight / GLfloat(ny);//������x��y������ÿ�����ض�Ӧ�ĸ���������

	for (z.x = xComplexMin; z.x < xComplexMax; z.x += zIncr.x)
		for (z.y = yComplexMin; z.y < yComplexMax; z.y += zIncr.y)//���ѭ������������ʵ�� z.x���ڲ�ѭ�������������鲿 z.y��
		{
			itercount = mandelSqtransf(z, maxIter);//����ÿ������ z������ mandelSqtransf ��������������������ø����ڵ���������û�з�ɢ�Ĵ�����
			if (itercount >= maxIter)//Ȼ�󣬸��ݵ����������������ɫ��
				ptColor.r = ptColor.g = ptColor.b = 0.0;
			else if (itercount > (maxIter / 8))
			{
				ptColor.r = 1.0;
				ptColor.g = 0.5;
				ptColor.b = 0.0;
			}
			else if (itercount > (maxIter / 10))
			{
				ptColor.r = 1.0;
				ptColor.g = 0.0;
				ptColor.b = 0.0;
			}
			else if (itercount > (maxIter / 20)) 
			{
				ptColor.b = 0.5;
				ptColor.g = 0.0;
				ptColor.r = 0.0;
			}
			else if (itercount > (maxIter / 40)) 
			{
				ptColor.r = ptColor.g = 1.0;
				ptColor.b = 0.0;
			}
			else if (itercount > (maxIter / 100)) 
			{
				ptColor.r = ptColor.b = 0.0;
				ptColor.g = 0.3;
			}
			else {
				ptColor.r = 0.0;
				ptColor.g = ptColor.b = 1.0;
			}
			glColor3f(ptColor.r, ptColor.g, ptColor.b);//���õ�ǰ�����ɫ��
			plotPoint(z);//������ǰ�㡣
		}
}

void displayFcn()
{

	 GLint nx = winWidth, ny = winHeight;// ���ڵĿ�Ⱥ͸߶�
	
	 glClear(GL_COLOR_BUFFER_BIT);
	 for (int i = 0; i < numViews; ++i) {
		 // // ���㵱ǰ�ӿڵ�λ�úʹ�С
		 glViewport(viewports[i][0], viewports[i][1], nx / 2, ny / 2);// �����ӿڵ�λ�úʹ�С
		 glMatrixMode(GL_PROJECTION);
		 glLoadIdentity();
		 gluOrtho2D(xComplexMin, xComplexMax, yComplexMin, yComplexMax);// ����ͶӰ����
		 glMatrixMode(GL_MODELVIEW);
		 mandelbrot(nx / 2, ny / 2, maxIter[i]);// ���Ƶ�ǰ�ӿڵ�ͼ��
	 }
	 glFlush();
}
void keyboard(unsigned char key, int x, int y) {
	switch (key) 
	{ 
		case 'i':
			printf("�������µĵ�������: ");
			for (int i = 0; i < numViews; i++) {
				printf("��ͼ%d: ", i + 1);
				cin >> maxItermax[i];
			}
		break;
	}
	glutPostRedisplay();
}
 
void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(xComplexMin, xComplexMax, yComplexMin, yComplexMax);
	glClear(GL_COLOR_BUFFER_BIT);
}
void update(int value) 
{
	for (int i = 0; i < numViews; i++) {// ����ÿ����ͼ
		if (maxIter[i] < maxItermax[i])// �����ǰ��������С�������������������ӵ�������
			maxIter[i] += 10;// ÿ������ 10
	}
	glutPostRedisplay(); // ˢ�´����Ի�����ͼ��
	glutTimerFunc(timeInterval, update, 0); // ����������һ����ʱ��
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);// ���ô�����ʾģʽΪ��ɫRGB
	glutInitWindowPosition(50,50);// ���ô���λ��
	glutInitWindowSize(winWidth, winHeight);// ���ô��ڴ�С
	glutCreateWindow("Experiment 2");// ��������
	init();// ��ʼ��
	glutTimerFunc(timeInterval, update, 0);// ���ö�ʱ�����Ը�����ʾ
	glutReshapeFunc(winReshapeFcn);// ���ô��ڴ�С�仯�Ļص�����
	glutDisplayFunc(displayFcn);// ������ʾ�ص�����
	glutKeyboardFunc(keyboard);// ���ü��̻ص�����
	glutMainLoop();// ������ѭ��
	return 0;
}
#include <iostream>
#include <GL/glut.h>
#include <math.h>
using namespace std;
GLsizei winWidth = 800, winHeight = 600;// 窗口的宽度和高度
GLfloat xComplexMin=-2.00, xComplexMax=0.50, yComplexMin=-1.25, yComplexMax=1.25;// 复数域的范围
GLfloat complexWidth=xComplexMax-xComplexMin, complexHeight=yComplexMax-yComplexMin;// 复数域的宽度和高度
GLint newX = 0, newY = 0;// 鼠标点击的位置
const int timeInterval = 200;// 刷新频率
const int numViews = 4;// 视图数量
GLint maxItermax[numViews] = { 50, 100, 150, 200 };// 最大迭代次数
GLint maxIter[numViews] = { 0,0,0,0 };// 当前视图的最大迭代次数
int viewports[numViews][2] = { {0, winHeight / 2},{winWidth / 2, winHeight / 2},{0, 0},{winWidth / 2, 0} };// 视图的位置
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
	complexNum zSquare;// 复数平方
	zSquare.x = z.x * z.x - z.y * z.y;// 实部平方
	zSquare.y = 2 * z.x * z.y;// 虚部平方
	return zSquare;// 返回复数平方
}
GLint mandelSqtransf(complexNum z0, GLint maxIter)
{
	complexNum z = z0;// 将输入的复数 z0 赋值给 z，z 是当前迭代的复数
	GLint count = 0;// 初始化迭代次数计数器
	while (z.x * z.x + z.y * z.y <=4.0 && (count < maxIter))// 迭代条件
	{
		z = complexSquare(z);//进行平方
		z.x += z0.x;// 将 z0 的实部加到当前复数 z 的实部上
		z.y += z0.y;// 将 z0 的虚部加到当前复数 z 的虚部上
		count++;// 增加迭代次数
	}
	return count;// 返回迭代次数
}
void mandelbrot(GLint nx, GLint ny, GLint maxIter)
{
	complexNum z, zIncr;// z 是当前迭代的复数,zIncr 是每个像素对应的复数增量，用于遍历复数平面。
	color ptColor;// 用于存储当前点的颜色
	GLint itercount; //是当前点的迭代次数计数器。

	zIncr.x = complexWidth / GLfloat(nx);
	zIncr.y = complexHeight / GLfloat(ny);//计算在x，y方向上每个像素对应的复数增量。

	for (z.x = xComplexMin; z.x < xComplexMax; z.x += zIncr.x)
		for (z.y = yComplexMin; z.y < yComplexMax; z.y += zIncr.y)//外层循环遍历复数的实部 z.x，内层循环遍历复数的虚部 z.y。
		{
			itercount = mandelSqtransf(z, maxIter);//对于每个复数 z，调用 mandelSqtransf 函数计算迭代次数，即该复数在迭代过程中没有发散的次数。
			if (itercount >= maxIter)//然后，根据迭代次数决定点的颜色：
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
			glColor3f(ptColor.r, ptColor.g, ptColor.b);//设置当前点的颜色。
			plotPoint(z);//画出当前点。
		}
}

void displayFcn()
{

	 GLint nx = winWidth, ny = winHeight;// 窗口的宽度和高度
	
	 glClear(GL_COLOR_BUFFER_BIT);
	 for (int i = 0; i < numViews; ++i) {
		 // // 计算当前视口的位置和大小
		 glViewport(viewports[i][0], viewports[i][1], nx / 2, ny / 2);// 设置视口的位置和大小
		 glMatrixMode(GL_PROJECTION);
		 glLoadIdentity();
		 gluOrtho2D(xComplexMin, xComplexMax, yComplexMin, yComplexMax);// 设置投影矩阵
		 glMatrixMode(GL_MODELVIEW);
		 mandelbrot(nx / 2, ny / 2, maxIter[i]);// 绘制当前视口的图像
	 }
	 glFlush();
}
void keyboard(unsigned char key, int x, int y) {
	switch (key) 
	{ 
		case 'i':
			printf("请输入新的迭代次数: ");
			for (int i = 0; i < numViews; i++) {
				printf("视图%d: ", i + 1);
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
	for (int i = 0; i < numViews; i++) {// 对于每个视图
		if (maxIter[i] < maxItermax[i])// 如果当前迭代次数小于最大迭代次数，则增加迭代次数
			maxIter[i] += 10;// 每次增加 10
	}
	glutPostRedisplay(); // 刷新窗口以绘制新图形
	glutTimerFunc(timeInterval, update, 0); // 继续设置下一个定时器
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);// 设置窗口显示模式为单色RGB
	glutInitWindowPosition(50,50);// 设置窗口位置
	glutInitWindowSize(winWidth, winHeight);// 设置窗口大小
	glutCreateWindow("Experiment 2");// 创建窗口
	init();// 初始化
	glutTimerFunc(timeInterval, update, 0);// 设置定时器，以更新显示
	glutReshapeFunc(winReshapeFcn);// 设置窗口大小变化的回调函数
	glutDisplayFunc(displayFcn);// 设置显示回调函数
	glutKeyboardFunc(keyboard);// 设置键盘回调函数
	glutMainLoop();// 进入主循环
	return 0;
}
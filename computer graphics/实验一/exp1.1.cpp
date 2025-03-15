#include<stdio.h>
#include <GL/glut.h>


GLfloat vertices[8][3] = {
{-1.0,-1.0,-1.0}, {1.0,-1.0,-1.0}, {1.0,1.0,-1.0}, {-1.0,1.0,-1.0},
{-1.0,-1.0,1.0}, {1.0,-1.0,1.0}, {1.0,1.0,1.0}, {-1.0,1.0,1.0}
};
GLfloat vertices1[8][3];

GLfloat colors[8][3] = {
 {0.0,0.0,0.0}, {1.0,0.0,0.0}, {1.0,1.0,0.0}, {0.0,1.0,0.0},
 {0.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0}
};

float mynear = -50; 
float myfar =1000;
float viewxform_z = -5.0;

void init(void)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 3; j++)
			vertices1[i][j] = vertices[i][j] + 7;
	}
	glEnable(GL_DEPTH_TEST);
}


void polygon(int a, int b, int c, int d)
{

	glBegin(GL_POLYGON);

	glColor3fv(colors[a]);
	glVertex3fv(vertices[a]);

	glColor3fv(colors[b]);
	glVertex3fv(vertices[b]);

	glColor3fv(colors[c]);
	glVertex3fv(vertices[c]);

	glColor3fv(colors[d]);
	glVertex3fv(vertices[d]);

	glEnd();
}
void polygon1(int a, int b, int c, int d)
{
	glBegin(GL_POLYGON);
	glColor3fv(colors[a]);
	glVertex3fv(vertices1[a]);

	glColor3fv(colors[b]);
	glVertex3fv(vertices1[b]);

	glColor3fv(colors[c]);
	glVertex3fv(vertices1[c]);

	glColor3fv(colors[d]);
	glVertex3fv(vertices1[d]);
	glEnd();
}
void draw(void)
{
	polygon(1, 0, 3, 2);
	polygon(3, 7, 6, 2);
	polygon(7, 3, 0, 4);
	polygon(2, 6, 5, 1);
	polygon(4, 5, 6, 7);
	polygon(5, 4, 0, 1);
	polygon1(1, 0, 3, 2);
	polygon1(3, 7, 6, 2);
	polygon1(7, 3, 0, 4);
	polygon1(2, 6, 5, 1);
	polygon1(4, 5, 6, 7);
	polygon1(5, 4, 0, 1);

}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(2,3,3, 0, 0, 0, 0, 1, 0);
	draw();
	glutSwapBuffers();
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-5.0, 5.0, -5.0, 5.0, mynear, myfar);
	glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("Experiement 1");
	init();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutMainLoop();
}

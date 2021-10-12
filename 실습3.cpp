#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <Windows.h>
using namespace std;
GLvoid drawscene(GLvoid);
GLvoid reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void resize(int width, int height);
void mousemove(int x, int y);
void random2();
GLboolean Count = true;
GLboolean check = false;
float a = 0.0;
float b = 0.0;
float c = 0.0;
float d = 0.0;
float e = 0.0;
float f = 0.0;
bool left_button;
float x3 ;
float y3 ;
float x2 ;
float y2 ;
float rsize = 25;
float xstep = 0.05;
float ystep = 0.05;
int windowWidth;
int windowHeight;

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);
	glutCreateWindow("example1");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cerr << "Unable to initialize GLEW" << endl;
		exit(EXIT_FAILURE);
	}
	else {
		cout << "GLEW initialized" << endl;
	}
	glutDisplayFunc(drawscene);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutMotionFunc(mousemove);
	glutMainLoop();
}
GLvoid drawscene() {
	if (Count) {
		glClearColor(0.5, 0.5, 0.5, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	
		Count = false;		
	}
	glutSwapBuffers();
}
GLvoid reshape(int w, int h) {
	glViewport(0, 0, 800, 600);
}
void random2() {
	random_device rd;
	default_random_engine dre(rd());
	uniform_real_distribution<>uid(0.0, 1.0);
	float a1 = uid(dre);
	float b1 = uid(dre);
	float c1 = uid(dre);
	d = a1;
	e = b1;
	f = c1;
}
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		x2 = (((float)(x - (windowWidth / 2)) / (windowWidth / 2)) - 0.05);
		x3 = (((float)(x - (windowWidth / 2)) / (windowWidth / 2)) + 0.05);
		y2 = ((((float)(windowHeight / 2) - y) / (windowHeight / 2)) - 0.05);
		y3 = ((((float)(windowHeight / 2) - y) / (windowHeight / 2)) + 0.05);
		random2();
	}
	glClearColor(0.5, 0.5, 0.5, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(d, e, f);
	glRectf(x2, y2, x3, y3);
	glutPostRedisplay();	
}
void mousemove(int x,int y) {
	x2 = (((float)(x - (windowWidth / 2)) / (windowWidth / 2)) - 0.05);
	x3 = (((float)(x - (windowWidth / 2)) / (windowWidth / 2)) + 0.05);
	y2 = ((((float)(windowHeight / 2) - y) / (windowHeight / 2)) - 0.05);
	y3 = ((((float)(windowHeight / 2) - y) / (windowHeight / 2)) + 0.05);
	glClearColor(0.5, 0.5, 0.5, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(d, e, f);
	glRectf(x2, y2, x3, y3);
	glutPostRedisplay();
}
void resize(int width, int height) {	
	windowWidth= width  ;
	windowHeight = height;
	glViewport(0, 0, width, height);	
}

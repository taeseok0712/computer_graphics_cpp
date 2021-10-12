#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <Windows.h>
using namespace std;
GLvoid drawscene(GLvoid);
GLvoid reshape(int w, int h);
void mouse(int button,int state,int x,int y);
void resize(int width, int height);
void random();
void random2();
void glrectf();
GLboolean Count = true;
GLboolean check = true;
float a = 0.0;
float b = 0.0;
float c = 0.0;
float d = 0.0;
float e = 0.0;
float f = 0.0;
bool left_button;

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
	glutMouseFunc(mouse);
	glutReshapeFunc(resize);
	glutMainLoop();
}

GLvoid drawscene() {

	glClearColor(a, b, c, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (Count) {
		random2();
		Count = false;
	}
	glColor3f(d, e, f);
	glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
	glFlush();
	glutSwapBuffers();
}
GLvoid reshape(int w, int h) {
	glViewport(0, 0, 500, 500);
}
void random() {
	random_device rd;
	default_random_engine dre(rd());
	uniform_real_distribution<>uid(0.0, 1.0);
	float a1 = uid(dre);
	float b1 = uid(dre);
	float c1 = uid(dre);
	a = a1;
	b = b1;
	c = c1;
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
		if (x >= 200 && x <= 600 && y >= 150 && y <= 450) {
			random2();
			glColor3f(d, e, f);
			glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
			cout << "Change color in Rectangle" << endl;
		}
		else {
			random();

			
			cout << "Change colot out Rectangle" << endl;
		}
	}
	glFlush();
}
void resize(int width, int height) {
	glutReshapeWindow(800, 600);
}
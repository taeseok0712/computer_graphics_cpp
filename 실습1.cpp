#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <Windows.h>
using namespace std;
GLvoid drawscene(GLvoid);
GLvoid reshape(int w, int h);
GLvoid keyboard(unsigned char key, int x, int y);
void random();
GLboolean Count = true;
GLboolean check = true;
float a = 0.0;
float b = 0.0;
float c = 0.0;
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0,0);
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
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}

GLvoid drawscene() {
	if (Count) {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		Count = false;
	}
	glutSwapBuffers();
}
GLvoid reshape(int w, int h) {
	glViewport(0, 0, w, h);
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
void timer(int value) {

	if (check) {
		random();
		glClearColor(a, b, c, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glutPostRedisplay();
		glutTimerFunc(1000, timer, 1);
	}
	else {
		random();
		glClearColor(a, b, c, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}
GLvoid keyboard(unsigned char key, int x, int y) {
	
	switch (key)
	{
	case 'r':
		glClearColor(1.0, 0.0, 0.0, 1.0);
		cout << "Red" << endl;
		glClear(GL_COLOR_BUFFER_BIT);		
		break;
	case 'g':
		glClearColor(0.0, 1.0, 0.0, 1.0);
		cout << "Green" << endl;
		glClear(GL_COLOR_BUFFER_BIT);	
		break;

	case 'b':
		glClearColor(0.0, 0.0, 1.0, 1.0);
		cout << "Blue" << endl;
		glClear(GL_COLOR_BUFFER_BIT);		
		break;
	case 'a':	
		random();
		glClearColor(a, b, c, 1.0);
		cout << "Random color" << endl;
		glClear(GL_COLOR_BUFFER_BIT);
		break;
	case 'w':
		glClearColor(1.0, 1.0, 1.0, 1.0);
		cout << "White" << endl;
		glClear(GL_COLOR_BUFFER_BIT);
		break;
	case 'k':
		glClearColor(0.0, 0.0, 0.0, 0.0);
		cout << "Black" << endl;
		glClear(GL_COLOR_BUFFER_BIT);
		break;
	case 't':
		cout << "timer on" << endl;
		check = true;
		glutTimerFunc(1000,timer,1);
		break;
	case 's':
		cout << "timer off" << endl;
		check = false;
		glutTimerFunc(1000, timer, 0);
		break;
	case 'q':
		cout << "exit" << endl;
		exit(0);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
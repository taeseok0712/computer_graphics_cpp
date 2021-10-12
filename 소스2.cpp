#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <Windows.h>
#include <fstream>
#include <sstream>
using namespace std;
GLvoid drawscene(GLvoid);
GLvoid reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void resize(int width, int height);
void random();
void random2();
void timer(int value);
GLvoid keyboard(unsigned char key, int x, int y);
GLboolean Count = true;
GLboolean check = false;
float a = 0.0;
float b = 0.0;
float c = 0.0;
float d = 0.0;
float e = 0.0;
float f = 0.0;
bool left_button;
float x3;
float y3;
float x2;
float y2;
float x4;
float y4;
float xstep = 0.05;
float ystep = 0.05;
int windowWidth;
int windowHeight;

struct tri {
	float x1, y1, z1, x2, y2, z2, x3, y3, z3;
};
tri t[][3] = {
	{
		0.3f, 0.3f, 0.0f,
		0.7f, 0.3f, 0.0f,
		0.5f,  0.7f, 0.0f,
	},
   {
	   -0.7f, 0.3f, 0.0f,
	   -0.3f, 0.3f, 0.0f,
	   -0.5f,  0.7f, 0.0f,
   },
   {
	   -0.7f, -0.7f, 0.0f,
	   -0.3f, -0.7f, 0.0f,
	   -0.5f,  -0.3f, 0.0f,
   },
   {
	   0.3f, -0.7f, 0.0f,
	   0.7f, -0.7f, 0.0f,
	   0.5f, -0.3f, 0.0f,
   }
};

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
	glutKeyboardFunc(keyboard);
	glutTimerFunc(3, timer, 1);
	glutMainLoop();
}
GLuint loadshader(const char* vertex_file_path,const char* fragment_file_path) {
	GLuint vertexshaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentshaderID = glCreateShader(GL_FRAGMENT_SHADER);

	string vertexshadercode;
	ifstream vertexshaderstream(vertex_file_path, ios::in);
	if (vertexshaderstream.is_open()) {
		stringstream sstr;
		sstr << vertexshaderstream.rdbuf();
		vertexshadercode = sstr.str();
		vertexshaderstream.close();
	}
	else {
		cout << "file" << vertex_file_path << "is not read" << endl;
		getchar();
		return 0;
	}
	string fragmentshadercode;
	ifstream fragmentshaderstream(fragment_file_path, ios::in);
	if (fragmentshaderstream.is_open()) {
		stringstream sstr;
		sstr << fragmentshaderstream.rdbuf();
		fragmentshadercode = sstr.str();
		fragmentshaderstream.close();
	}

	GLuint result = GL_FALSE;
	int infologlength;

	cout << "compiling shader:" << vertex_file_path << endl;
	char const* vertexsourcepointer = vertexshadercode.c_str();
	glShaderSource(vertexshaderID, 1, &vertexsourcepointer, NULL);
	glCompileShader(vertexshaderID);

	glGetShaderiv(vertexshaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexshaderID, GL_INFO_LOG_LENGTH, &infologlength);

	if (infologlength > 0) {
		vector<char> vertexshadererrormessage(infologlength + 1);
		glGetShaderInfoLog(vertexshaderID, infologlength, NULL, &vertexshadererrormessage[0]);
		cout << &vertexshadererrormessage[0] << endl;
	}
	cout << "compiling chader:" << fragment_file_path << endl;
	char const* fragmentsourcepointer = fragmentshadercode.c_str();
	glShaderSource(fragmentshaderID, 1, &fragmentsourcepointer, NULL);
	glCompileShader(fragmentshaderID);

	glGetShaderiv(fragmentshaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentshaderID, GL_INFO_LOG_LENGTH, &infologlength);
	if (infologlength > 0) {
		vector<char>fragmentshadererrormessage(infologlength + 1);
		glGetShaderInfoLog(fragmentshaderID, infologlength, NULL, &fragmentshadererrormessage[0]);
		cout << &fragmentshadererrormessage[0] << endl;
	}

	cout << "linking program" << endl;
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexshaderID);
	glAttachShader(programID, fragmentshaderID);
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infologlength);
	if (infologlength > 0) {
		vector<char>programerrormessage(infologlength + 1);
		glGetProgramInfoLog(programID, infologlength, NULL, &programerrormessage[0]);
		cout << &programerrormessage[0] << endl;
	}

	glDetachShader(programID, vertexshaderID);
	glDetachShader(programID, fragmentshaderID);

	glDeleteShader(vertexshaderID);
	glDeleteShader(fragmentshaderID);
	
	return programID;
}


GLvoid drawscene() {
	if (Count) {
		glClearColor(0.0, 0.0, 0.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		Count = false;





	}
	glutSwapBuffers();
}
GLvoid reshape(int w, int h) {
	glViewport(0, 0, 800, 600);
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
void resize(int width, int height) {
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, width, height);
}

GLvoid keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'a':
		check = true;
		cout << "1" << endl;
		break;
	case 's':
		check = false;
		cout << "2" << endl;
		break;
	case 'q':
		exit(0);
		break;
	default:
		break;
	}
}
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include <math.h>
using namespace std;

void initbuffer();
void make_vertexShaders();
void make_fragmentShaders();
void random_color();
void make_random_small_rectangel();
void mousemove(int x, int y);
void timer(int value);
void resize(int width, int height);
void mouse(int button, int state, int x, int y);
char* filetobuf(const string name);

GLuint make_shaderProgram();
GLvoid reshape(int w, int h);
GLvoid drawscene(GLvoid);
GLvoid keyboard(unsigned char key, int x, int y);

GLboolean Count = true;
GLboolean check = false;
bool left_button;
bool onesave = true;
bool changemode = true;
bool make_rectangle = true;
bool mouse_mode = false;
bool timer_mode = false;
bool StartClick = false;


float ystep = 0.005;
float a, b, c;
float x2, x3, y2, y3;
int windowWidth = 800, windowHeight = 600;

int rectangle_count = 0;
float r = 0.05;
const GLchar* vertexShaderSource;
const GLchar* fragmentShaderSource;


GLuint shaderID;
GLuint vertexshader;
GLuint fragmentshader;
GLuint triangleshaderProgram;

vector<float>pointindex;
float eraser_rectangle[36];

GLuint VAO[2], VBO[4];

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
	pointindex.resize(0);
	make_vertexShaders();
	make_fragmentShaders();
	shaderID = make_shaderProgram();
	make_random_small_rectangel();
	initbuffer();
	glutDisplayFunc(drawscene);
	glutMouseFunc(mouse);
	glutMotionFunc(mousemove);
	glutTimerFunc(100, timer, 1);
	glutReshapeFunc(reshape);
	glutMainLoop();
}
void make_random_small_rectangel() {
	random_device rd;
	default_random_engine dre(rd());
	uniform_real_distribution<>uid(-1.0, 1.0);
	uniform_real_distribution<>makecolor(0.0, 1.0);
	float a1 = makecolor(dre);
	float b1 = makecolor(dre);
	float c1 = makecolor(dre);
	float rect_x1 = uid(dre);
	float rect_y1 = uid(dre);

	float rect_x2 = rect_x1 + 0.035;
	float rect_y2 = rect_y1 - 0.035;
	
	pointindex.push_back(rect_x1);
	pointindex.push_back(rect_y1);
	pointindex.push_back(0);
	pointindex.push_back(a1);
	pointindex.push_back(b1);
	pointindex.push_back(c1);

	pointindex.push_back(rect_x1);
	pointindex.push_back(rect_y2);
	pointindex.push_back(0);
	pointindex.push_back(a1);
	pointindex.push_back(b1);
	pointindex.push_back(c1);

	pointindex.push_back(rect_x2);
	pointindex.push_back(rect_y2);
	pointindex.push_back(0);
	pointindex.push_back(a1);
	pointindex.push_back(b1);
	pointindex.push_back(c1);

	pointindex.push_back(rect_x1);
	pointindex.push_back(rect_y1);
	pointindex.push_back(0);
	pointindex.push_back(a1);
	pointindex.push_back(b1);
	pointindex.push_back(c1);

	pointindex.push_back(rect_x2);
	pointindex.push_back(rect_y1);
	pointindex.push_back(0);
	pointindex.push_back(a1);
	pointindex.push_back(b1);
	pointindex.push_back(c1);

	pointindex.push_back(rect_x2);
	pointindex.push_back(rect_y2);
	pointindex.push_back(0);
	pointindex.push_back(a1);
	pointindex.push_back(b1);
	pointindex.push_back(c1);
	
	rectangle_count++;
}
void random_color() {
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
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {		
		mouse_mode = true;
		x2 = (((float)(x - (windowWidth / 2)) / (windowWidth / 2)) - 0.035);
		x3 = (((float)(x - (windowWidth / 2)) / (windowWidth / 2)) + 0.035);
		y2 = ((((float)(windowHeight / 2) - y) / (windowHeight / 2)) - 0.035);
		y3 = ((((float)(windowHeight / 2) - y) / (windowHeight / 2)) + 0.035);
		eraser_rectangle[0] = x2;
		eraser_rectangle[1] = y3;
		eraser_rectangle[6] = x2;
		eraser_rectangle[7] = y2;
		eraser_rectangle[12] = x3;
		eraser_rectangle[13] = y2;

		eraser_rectangle[18] = x2;
		eraser_rectangle[19] = y3;
		eraser_rectangle[24] = x3;
		eraser_rectangle[25] = y3;
		eraser_rectangle[30] = x3;
		eraser_rectangle[31] = y2;

		random_color();
		for (int i = 0; i < 36; i++) {
			if (i % 6 == 2) {
				eraser_rectangle[i] = 0;
			}
			else if (i % 6 == 3) {
				eraser_rectangle[i] = 0;
			}
			else if (i % 6 == 4) {
				eraser_rectangle[i] = 0;
			}
			else if (i % 6 == 5) {
				eraser_rectangle[i] = 0;
			}
		}
	}
	else {
		mouse_mode = false;
	}
	glutPostRedisplay();
}
void mousemove(int x, int y) {

	x2 = (((float)(x - (windowWidth / 2)) / (windowWidth / 2)) - 0.035);
	x3 = (((float)(x - (windowWidth / 2)) / (windowWidth / 2)) + 0.035);
	y2 = ((((float)(windowHeight / 2) - y) / (windowHeight / 2)) - 0.035);
	y3 = ((((float)(windowHeight / 2) - y) / (windowHeight / 2)) + 0.035);
	eraser_rectangle[0] = x2;
	eraser_rectangle[1] = y3;
	eraser_rectangle[6] = x2;
	eraser_rectangle[7] = y2;
	eraser_rectangle[12] = x3;
	eraser_rectangle[13] = y2;

	eraser_rectangle[18] = x2;
	eraser_rectangle[19] = y3;
	eraser_rectangle[24] = x3;
	eraser_rectangle[25] = y3;
	eraser_rectangle[30] = x3;
	eraser_rectangle[31] = y2;
	
	for (int i = 0; i < pointindex.size() / 36; i++) {
		bool erase_small_rect = false;
		if ((x2 <= pointindex[36 * i] && pointindex[36 * i] <= x3)
			|| (x2 <= pointindex[36 * i + 12] && pointindex[36 * i + 12] <= x3)) {
			if ((y2 <= pointindex[36 * i + 1] && pointindex[36 * i + 1] <= y3)
				|| (y2 <= pointindex[36 * i + 13] && pointindex[36 * i + 13] <= y3)) {
				erase_small_rect = true;
			}
		}

		if (erase_small_rect) {
			for (int j = 0; j < 36; j++) {
				swap(pointindex[36 * i + j], pointindex[pointindex.size() - 36 + j]);
			}
			for (int k = 0; k < 36; k++) {
				pointindex.pop_back();
			}
			rectangle_count--;
		}
	}
	glutPostRedisplay();
}
void make_vertexShaders() {
	GLchar* vertexsource;

	vertexsource = filetobuf("vertexshader.glsl");

	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexshader, 1, &vertexsource, NULL);
	glCompileShader(vertexshader);

	GLint result;
	GLchar errorlog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(vertexshader, 512, NULL, errorlog);
		cerr << "Error:vertexshader 컴파일 실패" << errorlog << endl;
	}
}
void make_fragmentShaders() {
	GLchar* fragmentsource;
	fragmentsource = filetobuf("fragmentshader.glsl");

	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentshader, 1, &fragmentsource, NULL);
	glCompileShader(fragmentshader);

	GLint result;
	GLchar errorlog[512];
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorlog);
		cerr << "Error:fragmentshader 컴파일 실패" << errorlog << endl;
	}
}
char* filetobuf(const string name)
{
	vector<char> tempFile;
	ifstream in(name, ios::binary);
	char temp;
	while (true) {
		noskipws(in);
		in >> temp;
		if (in.eof()) {
			tempFile.push_back(0);
			break;
		}
		else
			tempFile.push_back(temp);
	}
	char* addr = new char[tempFile.size()];
	for (int i = 0; i < tempFile.size(); i++) {
		addr[i] = tempFile[i];
	}
	return addr;
}
GLuint make_shaderProgram() {

	GLuint shaderprogramID = glCreateProgram();

	glAttachShader(shaderprogramID, vertexshader);
	glAttachShader(shaderprogramID, fragmentshader);

	glLinkProgram(shaderprogramID);

	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);

	GLint result;
	GLchar errorlog[512];

	glGetProgramiv(shaderprogramID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(triangleshaderProgram, 512, NULL, errorlog);
		cerr << "Error: shader program 연결 실패" << errorlog << endl;
	}
	glUseProgram(shaderprogramID);
	return shaderprogramID;
}
GLvoid drawscene() {
	glClearColor(0.5, 0.5, 0.5, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	initbuffer();
	glUseProgram(shaderID);
	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_TRIANGLES, 0, pointindex.size() / 6);
	if (mouse_mode) {
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, pointindex.size() / 6);
	}
	glutSwapBuffers();
}
GLvoid reshape(int w, int h) {
	glViewport(0, 0, 800, 600);
}
void initbuffer() {

	glGenVertexArrays(2, VAO);

	glBindVertexArray(VAO[0]);
	glGenBuffers(2, &VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pointindex.size(), &pointindex[0], GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pointindex.size(), &pointindex[0], GL_STREAM_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	if (mouse_mode) {
		glBindVertexArray(VAO[1]);
		glGenBuffers(2, &VBO[2]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(eraser_rectangle), eraser_rectangle, GL_STREAM_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(eraser_rectangle), eraser_rectangle, GL_STREAM_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}


}
void timer(int value) {
	if (rectangle_count < 100) {
		make_random_small_rectangel();
	}
	glutPostRedisplay();
	glutTimerFunc(500, timer, 1);
}

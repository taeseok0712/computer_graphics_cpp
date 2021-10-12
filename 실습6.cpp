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
struct trio {
	float data[18];
	float sinx, cosx;
	float xmove, ymove;
	float vectorx, vectory;
	float center_x, center_y;
};
queue<trio> tri;
void random2();
void moverandom();
void initbuffer();
void make_vertexShaders();
void make_fragmentShaders();
void timer(int value);
void rotatetri(trio *temp);
void resize(int width, int height);
void mouse(int button, int state, int x, int y);
char* filetobuf(const string name);
GLuint make_shaderProgram();
GLvoid reshape(int w, int h);
GLvoid drawscene(GLvoid);
GLvoid keyboard(unsigned char key, int x, int y);
GLboolean Count = true;
GLboolean check = true;
float x2, x3, x4, y2, y3;
float d = 0.0;
float e = 0.0;
float f = 0.0;
bool left_button;
bool changemode = true;

float random_xmove, random_ymove;
float xscale = 1.0;
float yscale = 1.0;
float xstep = 0.007;
float ystep = 0.007;
float xfscale = 0.003;
float yfscale = 0.003;
int windowWidth, windowHeight;
const GLchar* vertexShaderSource;
const GLchar* fragmentShaderSource;
GLuint VAO[4], VBO[8];
GLuint shaderID;
GLuint vertexshader;
GLuint fragmentshader;
GLuint triangleshaderProgram;


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
	make_vertexShaders();
	make_fragmentShaders();
	shaderID = make_shaderProgram();
	initbuffer();
	glutDisplayFunc(drawscene);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(1, timer, 1);
	glutMainLoop();
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
	if (Count == false) {
		int size = tri.size();
		initbuffer();
		glUseProgram(shaderID);
		for (int i = 0; i < size; i++) {
			glBindVertexArray(VAO[i]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
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
		int makecount = tri.size();
		if (makecount == 4) {
			tri.pop();
		}
		x2 = (((float)(x - (windowWidth / 2)) / (windowWidth / 2)) - 0.15);
		x3 = (((float)(x - (windowWidth / 2)) / (windowWidth / 2)) + 0.15);
		y2 = ((((float)(windowHeight / 2) - y) / (windowHeight / 2)) - 0.15);
		y3 = ((((float)(windowHeight / 2) - y) / (windowHeight / 2)) + 0.15);
		x4 = ((float)(x - (windowWidth / 2)) / (windowWidth / 2));
		
		
		trio temp;
		for (int i = 0; i < 18; i++) {
			if (i % 6 == 0) {
				if (i / 6 == 0) {
					temp.data[i] = x2 ;
				}
				else if (i / 6 == 1) {
					temp.data[i] = x4;
				}
				else {
					temp.data[i] = x3;
				}
			}
			else if (i % 6 == 1) {
				if (i / 6 == 0) {
					temp.data[i] = y2;
				}
				else if (i / 6 == 1) {
					temp.data[i] = y3 ;
				}
				else {
					temp.data[i] = y2;
				}
			}
			else if (i % 6 == 2) {
				temp.data[i] = 0;
			}
			else if (i % 6 == 3) {
				random2();
				temp.data[i] = d;
			}
			else if (i % 6 == 4) {
				random2();
				temp.data[i] = e;
			}
			else if (i % 6 == 5) {
				random2();
				temp.data[i] = f;
			}
		}
		temp.center_x = ((temp.data[0] + temp.data[6] + temp.data[12]) / 3);
		temp.center_y = ((temp.data[1] + temp.data[7] + temp.data[13]) / 3);
		temp.cosx = 0;
		temp.sinx = 0;
		moverandom();
		temp.xmove=random_xmove;
		temp.ymove=random_ymove;
		temp.vectorx = 1.0;
		temp.vectory = 1.0;
		tri.push(temp);
		Count = false;
		glutPostRedisplay();
	}
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
		check = false;
		cout << "triangle polygon mode" << endl;
		break;
	case 'b':
		check = true;
		cout << "triangle line mode" << endl;
		break;
	case 'q':
		exit(0);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
void moverandom() {
	random_device rd;
	default_random_engine dre(rd());
	uniform_real_distribution<>uid(0.02, 0.07);
	float a1 = uid(dre);
	float b1 = uid(dre);
	random_xmove = a1;
	random_ymove = b1;
}
void initbuffer() {
	int size = tri.size();
	glGenVertexArrays(4, VAO);
	for (int i = 0; i < size; i++) {
		trio temp = tri.front();
		glBindVertexArray(VAO[i]);
		glGenBuffers(2, &VBO[2 * i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2 * i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(temp), &temp, GL_STREAM_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2 * i + 1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(temp), &temp, GL_STREAM_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		tri.pop();
		tri.push(temp);
	}
}
void timer(int value) {
	int sizet = tri.size();
	
	if (check) {
		for (int i = 0; i < sizet; i++) {
			trio temp = tri.front();
			if (temp.data[0] >= 1.0 || temp.data[6] >= 1.0 || temp.data[12] >= 1.0) {
				rotatetri(&temp);
				temp.vectorx = -1.0;
			}
			else if (temp.data[0] <= -1.0 || temp.data[6] <= -1.0 || temp.data[12] <= -1.0) {
				rotatetri(&temp);
				temp.vectorx = 1.0;
			}
			if (temp.data[1] >= 1.0 || temp.data[7] >= 1.0 || temp.data[13] >= 1.0) {
				rotatetri(&temp);
				temp.vectory = -1.0;
			}
			else if (temp.data[1] <= -1.0 || temp.data[7] <= -1.0 || temp.data[13] <= -1.0) {
				rotatetri(&temp);
				temp.vectory = 1.0;
			}

			temp.data[0] += temp.vectorx * temp.xmove;
			temp.data[1] += temp.vectory * temp.ymove;
			temp.data[6] += temp.vectorx * temp.xmove;
			temp.data[7] += temp.vectory * temp.ymove;
			temp.data[12] += temp.vectorx * temp.xmove;
			temp.data[13] += temp.vectory * temp.ymove;
			temp.center_x = ((temp.data[0] + temp.data[6] + temp.data[12]) / 3);
			temp.center_y = ((temp.data[1] + temp.data[7] + temp.data[13]) / 3);
			tri.pop();
			tri.push(temp);
		}
	}
	glutPostRedisplay();
	glutTimerFunc(60, timer, 1);
}
void rotatetri(trio* temp) {

	trio rotate = *temp;
	trio temp2;
	rotate.data[0] = rotate.data[0] - rotate.center_x;
	rotate.data[6] = rotate.data[6] - rotate.center_x;
	rotate.data[12] = rotate.data[12] - rotate.center_x;

	rotate.data[1] = rotate.data[1] - rotate.center_y;
	rotate.data[7] = rotate.data[7] - rotate.center_y;
	rotate.data[13] = rotate.data[13] - rotate.center_y;

	temp2.data[0] = 0 * rotate.data[0] - 1 * rotate.data[1];
	temp2.data[6] = 0 * rotate.data[6] - 1 * rotate.data[7];
	temp2.data[12] = 0 * rotate.data[12] - 1 * rotate.data[13];

	temp2.data[1] = 1 * rotate.data[0] + 0 * rotate.data[1];
	temp2.data[7] = 1 * rotate.data[6] + 0 * rotate.data[7];
	temp2.data[13] = 1 * rotate.data[12] + 0 * rotate.data[13];

	temp->data[0] = temp2.data[0] + rotate.center_x;
	temp->data[6] = temp2.data[6] + rotate.center_x;
	temp->data[12] = temp2.data[12] + rotate.center_x;
		
	temp->data[1] = temp2.data[1] + rotate.center_y;
	temp->data[7] = temp2.data[7] + rotate.center_y;
	temp->data[13] = temp2.data[13] + rotate.center_y;
	
}
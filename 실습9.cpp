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
bool mouse_mode = true;
bool timer_mode = false;
bool StartClick = false;
float ystep = 0.005;
float a, b, c;
float x2 = 0;
float y2 = 0;
int windowWidth = 800, windowHeight = 600;
int theta = 180;
int round_count = 0;
float r = 0.05;
const GLchar* vertexShaderSource;
const GLchar* fragmentShaderSource;


GLuint shaderID;
GLuint vertexshader;
GLuint fragmentshader;
GLuint triangleshaderProgram;

vector<float>pointindex;

GLuint VAO, VBO[2];


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
	initbuffer();
	glutDisplayFunc(drawscene);
	glutMouseFunc(mouse);
	glutTimerFunc(1, timer, 1);
	glutReshapeFunc(reshape);
	glutMainLoop();
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
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		pointindex.resize(0);
		x2 = (((float)(x - (windowWidth / 2)) / (windowWidth / 2)));
		y2 = ((((float)(windowHeight / 2) - y) / (windowHeight / 2)));
		random();
		pointindex.push_back(x2);
		pointindex.push_back(y2);
		pointindex.push_back(0);
		pointindex.push_back(a);
		pointindex.push_back(b);
		pointindex.push_back(c);
		mouse_mode = !mouse_mode;
		check = true;
		timer_mode = true;
		round_count = 0;
		r = 0.05;
		if (!mouse_mode) {
			theta = 180;
		}
		else {
			theta = 0;
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

	glBindVertexArray(VAO);
	glPointSize(3.0f);
	glDrawArrays(GL_POINTS, 0, pointindex.size()/6);

	glutSwapBuffers();
}
GLvoid reshape(int w, int h) {
	glViewport(0, 0, 800, 600);
}

void initbuffer() {
	if (check) {
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(2, &VBO[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pointindex.size(), &pointindex[0], GL_STREAM_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); 
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pointindex.size(), &pointindex[0], GL_STREAM_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
}
void timer(int value) {
	if (timer_mode) {
		if (!mouse_mode) {
			if (round_count < 4) {
				float x;
				float y;
				if (round_count % 2 == 0) {
					x = (r * cos(theta * 3.14 / 180) + x2 + r);
					y = (r * sin(theta * 3.14 / 180) + y2);
				}
				else if (round_count % 2 == 1) {
					x = (r * cos(theta * 3.14 / 180) + x2 - r);
					y = (r * sin(theta * 3.14 / 180) + y2);
				}
				pointindex.push_back(x);
				pointindex.push_back(y);
				pointindex.push_back(0);
				pointindex.push_back(a);
				pointindex.push_back(b);
				pointindex.push_back(c);

				theta -= 1;
				if ((theta % 180) == 0 || (theta % 180) == 180) {
					r *= 1.5;
					round_count++;
					x2 = x;
					y2 = y;
					if (round_count==4) {
						theta += 180;
						r /= 1.5;
					}
				}

			}
			else if (round_count < 8 && round_count >= 4) {
				float x;
				float y;
				if (round_count % 2 == 0) {
					x = (r * cos(theta * 3.14 / 180) + x2 - r);
					y = (r * sin(theta * 3.14 / 180) + y2);
				}
				else if (round_count % 2 == 1) {
					x = (r * cos(theta * 3.14 / 180) + x2 + r);
					y = (r * sin(theta * 3.14 / 180) + y2);
				}
				pointindex.push_back(x);
				pointindex.push_back(y);
				pointindex.push_back(0);
				pointindex.push_back(a);
				pointindex.push_back(b);
				pointindex.push_back(c);

				theta += 1;
				if ((theta % 180) == 0 || (theta % 180) == 180) {
					r /= 1.5;
					round_count++;
					x2 = x;
					y2 = y;
				}
			}
		}
		else {
			if (round_count < 4) {
				float x;
				float y;
				if (round_count % 2 == 0) {
					x = (r * cos(theta * 3.14 / 180) + x2 - r);
					y = (r * sin(theta * 3.14 / 180) + y2);
				}
				else if (round_count % 2 == 1) {
					x = (r * cos(theta * 3.14 / 180) + x2 + r);
					y = (r * sin(theta * 3.14 / 180) + y2);
				}
				pointindex.push_back(x);
				pointindex.push_back(y);
				pointindex.push_back(0);
				pointindex.push_back(a);
				pointindex.push_back(b);
				pointindex.push_back(c);

				theta += 1;
				if ((theta % 180) == 0 || (theta % 180) == 180) {
					r *= 1.5;
					round_count++;
					x2 = x;
					y2 = y;
					if (round_count == 4) {
						theta -= 180;
						r /= 1.5;
					}
				}
			}
			else if (round_count < 8 && round_count >= 4) {				
				float x;
				float y;
				if (round_count % 2 == 0) {
					x = (r * cos(theta * 3.14 / 180) + x2 + r);
					y = (r * sin(theta * 3.14 / 180) + y2);
				}
				else if (round_count % 2 == 1) {
					x = (r * cos(theta * 3.14 / 180) + x2 - r);
					y = (r * sin(theta * 3.14 / 180) + y2);
				}
				pointindex.push_back(x);
				pointindex.push_back(y);
				pointindex.push_back(0);
				pointindex.push_back(a);
				pointindex.push_back(b);
				pointindex.push_back(c);

				theta -= 1;
				if ((theta % 180) == 0 || (theta % 180) == 180) {
					r /= 1.5;
					round_count++;
					x2 = x;
					y2 = y;
					
				}
			}
		}
	}
	glutPostRedisplay();
	glutTimerFunc(1, timer, 1);
}

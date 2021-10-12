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
using namespace std;
void mouse(int button, int state, int x, int y);
void resize(int width, int height);
void random2();
void initbuffer();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const string name);
GLuint make_shaderProgram();
GLvoid reshape(int w, int h);
GLvoid drawscene(GLvoid);
GLvoid keyboard(unsigned char key, int x, int y);
GLboolean Count = true;
GLboolean check = false;
float d = 0.0;
float e = 0.0;
float f = 0.0;
bool left_button;
bool changemode = true;
bool onesave = true;
float x3, y3, x2, y2, x4, y4;
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
const float triangle[] = {
	0.3,0.3,0,1.0,0.0,0.0,
	0.45,0.6,0,1.0,0.0,0.0,
	0.6,0.3,0,1.0,0.0,0.0,

	-0.3,0.3,0,0.0,1.0,0.0,
	-0.45,0.6,0,0.0,1.0,0.0,
	-0.6,0.3,0,0.0,1.0,0.0,

	-0.3,-0.6,0,0.0,0.0,1.0,
	-0.45,-0.3,0,0.0,0.0,1.0,
	-0.6,-0.6,0,0.0,0.0,1.0,

	0.3,-0.6,0,1.0,1.0,1.0,
	0.45,-0.3,0,1.0,1.0,1.0,
	0.6,-0.6,0,1.0,1.0,1.0
};
struct trio {
	float data[18];
};

queue<trio> tri;

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
	//glutTimerFunc(3, timer, 1);
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
	if (changemode == true) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	glClearColor(0.5, 0.5, 0.5, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	initbuffer();
	glUseProgram(shaderID);
	for (int i = 0; i < 4; i++) {
		glBindVertexArray(VAO[i]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
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
		random2();
		
		

		if (((x3+ xfscale)-(x2- xfscale))<0.1||((x3+ xfscale)-(x2- xfscale))>0.8) {
			xscale *= -1.0;			
		}
		if (((y3+ yfscale)-(y2- yfscale)) < 0.1 || ((y3+ yfscale)-(y2- yfscale)) > 0.8) {
			yscale *= -1.0;
		}
		xfscale += (xscale * xstep);
		yfscale += (yscale * ystep);

		trio temp;
		for (int i = 0; i < 18; i++) {
			if (i % 6 == 0) {
				if (i / 6 == 0) {
					temp.data[i] = x2 - xfscale;
				}
				else if (i / 6 == 1) {
					temp.data[i] = x4;
				}
				else {
					temp.data[i] = x3 + xfscale;
				}
			}
			else if (i % 6 == 1) {
				if (i / 6 == 0) {
					temp.data[i] = y2 - yfscale;
				}							
				else if (i / 6 == 1) {		
					temp.data[i] = y3 + yfscale;
				}							
				else {						
					temp.data[i] = y2 - yfscale;
				}
			}
			else if (i % 6 == 2) {
				temp.data[i] = 0;
			}
			else if (i % 6 == 3) {
				temp.data[i] = d;
			}
			else if (i % 6 == 4) {
				temp.data[i] = e;
			}
			else if (i % 6 == 5) {
				temp.data[i] = f;
			}
		}
		tri.push(temp);

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
		changemode = true;
		cout << "triangle polygon mode" << endl;
		break;
	case 'b':
		changemode = false;
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

void save() {

	for (int i = 0; i < 4; i++) {
		trio temp;
		for (int j = 0; j < 18; j++) {						
			temp.data[j] = triangle[j + (i * 18)];							
		}
		tri.push(temp);
	}
}
void initbuffer() {	
	if (onesave) {
		save();
		onesave = false;
	}
	glGenVertexArrays(4, VAO);
	for (int i = 0; i < 4; i++) {
		trio temp = tri.front();
		glBindVertexArray(VAO[i]);
		glGenBuffers(2, &VBO[2*i]);		
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2*i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(temp), &temp, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2 * i+1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(temp), &temp, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);		
		tri.pop();
		tri.push(temp);

	}
}
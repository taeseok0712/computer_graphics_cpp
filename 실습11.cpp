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
void mousemove(int x, int y);
void resize(int width, int height);
void random2();
void initbuffer();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const string name);
GLuint make_shaderProgram();
GLvoid reshape(int w, int h);
GLvoid drawscene(GLvoid);
GLboolean Count = true;
GLboolean check = false;
float d = 0.0;
float e = 0.0;
float f = 0.0;
bool left_button;
bool changemode = true;
int click_mode = 0;
float x3, y3, x2, y2;
float x4 = 0;
float y4 = 0;
float center_x, center_y;
int windowWidth, windowHeight;
const GLchar* vertexShaderSource;
const GLchar* fragmentShaderSource;
GLuint VAO[4], VBO[8];
GLuint shaderID;
GLuint vertexshader;
GLuint fragmentshader;
GLuint triangleshaderProgram;
float rectagle[] = {
	-0.5,0.5,0,1,0,0,
	-0.5,-0.5,0,0,1,0,
	0.5,-0.5,0,0,0,1,
	0.5,0.5,0,0,1,0
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
	make_vertexShaders();
	make_fragmentShaders();
	shaderID = make_shaderProgram();
	initbuffer();
	glutDisplayFunc(drawscene);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutMotionFunc(mousemove);
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
	initbuffer();
	glUseProgram(shaderID);
	glLineWidth(5.0f);
	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
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
void pointfind() {
	float find_maxy = 0;
	float find_miny = 0;
	float find_maxx = 0;
	float find_minx = 0;
	int count = 0;
	for (int i = 0; i < 4; i++) {
		if (rectagle[6 * i+1] >= find_maxy) {
			find_maxy = rectagle[6 * i + 1];
		}
		if (rectagle[6 * i + 1] <= find_miny) {
			find_miny = rectagle[6 * i + 1];
		}
	}
	for (int i = 0; i < 4; i++) {
		if (rectagle[6 * i] >= find_maxx) {
			find_maxx = rectagle[6 * i];
		}
		if (rectagle[6 * i] <= find_minx) {
			find_minx = rectagle[6 * i];
		}
	}
	if (find_miny <= center_y && center_y <= find_maxy &&
		find_minx <= center_x && center_x <= find_maxx) {		
		if (rectagle[1] != rectagle[7]) {
			if (rectagle[0] != rectagle[6]) {
				float count_fun_x1 = (center_y - rectagle[1]) * ((rectagle[6] - rectagle[0]) / rectagle[7] - rectagle[1]) + rectagle[0];
				if (center_x <= count_fun_x1) {
					count++;
				}
			}
		}
		if (rectagle[7] != rectagle[13]) {
			if (rectagle[6] != rectagle[12]) {
				float count_fun_x2 = (center_y - rectagle[7]) * ((rectagle[12] - rectagle[6]) / rectagle[13] - rectagle[7]) + rectagle[6];
				if (center_x <= count_fun_x2) {
					count++;
				}
			}
		}
		if (rectagle[13] != rectagle[19]) {
			if (rectagle[12] != rectagle[18]) {
				float count_fun_x3 = (center_y - rectagle[13]) * ((rectagle[18] - rectagle[12]) / rectagle[19] - rectagle[13]) + rectagle[12];
				if (center_x <= count_fun_x3) {
					count++;
				}
			}
		}
		if (rectagle[19] != rectagle[1]) {
			if (rectagle[18] != rectagle[0]) {
				float count_fun_x4 = (center_y - rectagle[19]) * ((rectagle[0] - rectagle[18]) / rectagle[1] - rectagle[19]) + rectagle[18];
				if (center_x <= count_fun_x4) {
					count++;
				}
			}
		}
		if (count % 2 == 0) {
			click_mode = 5;
		}
	}
}
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		click_mode = 0;


		x2 = (((float)(x - (windowWidth / 2)) / (windowWidth / 2)) - 0.1);
		x3 = (((float)(x - (windowWidth / 2)) / (windowWidth / 2)) + 0.1);
		y2 = ((((float)(windowHeight / 2) - y) / (windowHeight / 2)) - 0.1);
		y3 = ((((float)(windowHeight / 2) - y) / (windowHeight / 2)) + 0.1);
		center_x = ((float)(x - (windowWidth / 2)) / (windowWidth / 2));
		center_y = ((float)((windowHeight/ 2) - y) / (windowHeight / 2));
		if (x2 <= rectagle[0] && rectagle[0] <= x3) {
			if (y2 <= rectagle[1] && rectagle[1] <= y3) {
				click_mode = 1;
			}
		}
		if (x2 <= rectagle[6] && rectagle[6] <= x3) {
			if (y2 <= rectagle[7] && rectagle[7] <= y3) {
				click_mode = 2;
			}
		}
		if (x2 <= rectagle[12] && rectagle[12] <= x3) {
			if (y2 <= rectagle[13] && rectagle[13] <= y3) {
				click_mode = 3;
			}
		}
		if (x2 <= rectagle[18] && rectagle[18] <= x3) {
			if (y2 <= rectagle[19] && rectagle[19] <= y3) {
				click_mode = 4;
			}
		}
		if (click_mode==0) {
			pointfind();
		}
		glutPostRedisplay();
	}
}
void mousemove(int x, int y) {
	
	x2 = (((float)(x - (windowWidth / 2)) / (windowWidth / 2)) - 0.1);
	x3 = (((float)(x - (windowWidth / 2)) / (windowWidth / 2)) + 0.1);
	y2 = ((((float)(windowHeight / 2) - y) / (windowHeight / 2)) - 0.1);
	y3 = ((((float)(windowHeight / 2) - y) / (windowHeight / 2)) + 0.1);
	center_x = ((float)(x - (windowWidth / 2)) / (windowWidth / 2));
	center_y = ((float)((windowHeight / 2) - y) / (windowHeight / 2));

	if (click_mode == 1) {
		rectagle[0] = center_x;
		rectagle[1] = center_y;
	}
	if (click_mode == 2) {
		rectagle[6] = center_x;
		rectagle[7] = center_y;
	}
	if (click_mode == 3) {
		rectagle[12] = center_x;
		rectagle[13] = center_y;
	}
	if (click_mode == 4) {
		rectagle[18] = center_x;
		rectagle[19] = center_y;
	}
	if (click_mode == 5) {
		if (x4 == 0 && y4 == 0) {
			rectagle[0] = rectagle[0] + center_x;
			rectagle[1] = rectagle[1] + center_y;
			rectagle[6] = rectagle[6] + center_x;
			rectagle[7] = rectagle[7] + center_y;
			rectagle[12] = rectagle[12] + center_x;
			rectagle[13] = rectagle[13] + center_y;
			rectagle[18] = rectagle[18] + center_x;
			rectagle[19] = rectagle[19] + center_y;
			x4 = center_x;
			y4 = center_y;
		}
		if (x4 != 0 || y4 != 0) {
			float delta_X = center_x - x4;
			float delta_Y = center_y - y4;


			rectagle[0] = rectagle[0] + delta_X;
			rectagle[1] = rectagle[1] + delta_Y;
			rectagle[6] = rectagle[6] + delta_X;
			rectagle[7] = rectagle[7] + delta_Y;
			rectagle[12] = rectagle[12] + delta_X;
			rectagle[13] = rectagle[13] + delta_Y;
			rectagle[18] = rectagle[18] + delta_X;
			rectagle[19] = rectagle[19] + delta_Y;

			x4 = center_x;
			y4 = center_y;
		}

	}




	glutPostRedisplay();
}
void resize(int width, int height) {
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, width, height);
}

void initbuffer() {

	glGenVertexArrays(1, VAO);

	glBindVertexArray(VAO[0]);
	glGenBuffers(2, &VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectagle), rectagle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectagle), rectagle, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}
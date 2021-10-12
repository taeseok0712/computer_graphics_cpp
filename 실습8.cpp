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
void mouse(int button, int state, int x, int y);
void timer(int value);
void resize(int width, int height);
char* filetobuf(const string name);

GLuint make_shaderProgram();
GLvoid reshape(int w, int h);
GLvoid drawscene(GLvoid);
GLvoid keyboard(unsigned char key, int x, int y);

GLboolean Count = true;
GLboolean check = false;
bool left_button = true;
bool onesave = true;
bool changemode = true;
bool make_rectangle = true;
bool moving_mode_change = true;
bool devide_connect_mode = true;
float xstep = 0.05;
float ystep = 0.15;
bool command_right_change = true;
bool command_left_change = true;
int windowWidth, windowHeight;

const GLchar* vertexShaderSource;
const GLchar* fragmentShaderSource;

GLuint VAO[6], VBO[12];

GLuint shaderID;
GLuint vertexshader;
GLuint fragmentshader;
GLuint triangleshaderProgram;

float InnerTriangelData[] = {
	-0.55,0.0,0,1,0,0,   //0~5
	-0.50,0.2,0,1,0,0,  //6~11
	-0.45,0.0,0,1,0,0,  //12~17

	-0.35,0.0,0,1,0,0,  //18~23
	-0.30,0.2,0,1,0,0,  //24~29
	-0.25,0.0,0,1,0,0,  //30
	
	-0.15,0.0,0,0,1,0,  //36
	-0.10,0.2,0,0,1,0,  //42
	-0.05,0.0,0,0,1,0,  //48

	0.05,0.0,0,0,1,0,  //54
	0.10,0.2,0,0,1,0,  //60
	0.15,0.0,0,0,1,0,	//66

	0.25,0.0,0,0,0,1,  //72
	0.30,0.2,0,0,0,1,  //78
	0.35,0.0,0,0,0,1,  //84

	0.45,0.0,0,0,0,1,  //90
	0.50,0.2,0,0,0,1,  //96
	0.55,0.0,0,0,0,1  //102

};
struct triangle_datas {
	float data[18];
	int vector_x, vector_y;
	int up_down_count;
};
queue<triangle_datas>triangle_move;

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
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(1, timer, 1);
	glutReshapeFunc(resize);
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
	if (!left_button) {
		for (int i = 0; i < 6; i++) {
			glBindVertexArray(VAO[i]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}
	glutSwapBuffers();
}
GLvoid reshape(int w, int h) {
	glViewport(0, 0, 800, 600);
}
void resize(int width, int height) {
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, width, height);
}
void save() {

	for (int i = 0; i < 6; i++) {
		triangle_datas temp;
		for (int j = 0; j < 18; j++) {
			temp.data[j] = InnerTriangelData[j + (i * 18)];
		}
		temp.vector_x = -1;
		triangle_move.push(temp);
	}
}

void initbuffer() {

	if (onesave) {
		save();
		onesave = false;
	}
	glGenVertexArrays(6, VAO);
	for (int i = 0; i < 6; i++) {
		triangle_datas temp = triangle_move.front();
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
		triangle_move.pop();
		triangle_move.push(temp);
	}
}
void command_m() {
	
	for (int i = 0; i < triangle_move.size(); i++) {
		triangle_datas temp = triangle_move.front();
		
		if (moving_mode_change) {
			temp.vector_x = 1;
			temp.vector_y = -1;
		}
		else {
			temp.vector_x *= -1;
			temp.vector_y *= -1;
		}
		triangle_move.pop();
		triangle_move.push(temp);
	}
	command_right_change = false;
	command_left_change = true;
	moving_mode_change = false;
}
void command_n() {
	for (int i = 0; i < triangle_move.size(); i++) {
		triangle_datas temp = triangle_move.front();
		if (moving_mode_change) {
			temp.vector_x = -1;
			temp.vector_y = -1;
		}
		else {
			temp.vector_x *= -1;
			temp.vector_y *= -1;
		}
		triangle_move.pop();
		triangle_move.push(temp);
	}
	command_right_change = true;
	command_left_change = false;
	moving_mode_change = false;

}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		left_button = false;
	}
	glFlush();
}





void timer(int value) {
	if (check) {
		for (int i = 0; i < triangle_move.size(); i++) {
			triangle_datas temp = triangle_move.front();
			if (temp.data[0] >= 1.0 || temp.data[6] >= 1.0 || temp.data[12] >= 1.0) {
				
				temp.vector_x = -1.0;

				if (temp.data[1] >= 1.0 || temp.data[7] >= 1.0 || temp.data[13] >= 1.0) {
					temp.vector_y = -1;

				}
				else if (temp.data[1] <= -1.0 || temp.data[7] <= -1.0 || temp.data[13] <= -1.0) {
					temp.vector_y = 1;
				}
				temp.data[1] += temp.vector_y * ystep;
				temp.data[7] += temp.vector_y * ystep;
				temp.data[13] += temp.vector_y * ystep;

			}
			else if (temp.data[0] <= -1.0 || temp.data[6] <= -1.0 || temp.data[12] <= -1.0) {
				
				temp.vector_x = 1.0;
				if (temp.data[1] >= 1.0 || temp.data[7] >= 1.0 || temp.data[13] >= 1.0) {
					temp.vector_y = -1;

				}
				else if (temp.data[1] <= -1.0 || temp.data[7] <= -1.0 || temp.data[13] <= -1.0) {
					temp.vector_y = 1;
				}
				temp.data[1] += temp.vector_y * ystep;
				temp.data[7] += temp.vector_y * ystep;
				temp.data[13] += temp.vector_y * ystep;
			}
			temp.data[0] += temp.vector_x * xstep;			
			temp.data[6] += temp.vector_x * xstep;			
			temp.data[12] += temp.vector_x * xstep;
	
			triangle_move.pop();
			triangle_move.push(temp);
		}
		
	}

	glutPostRedisplay();
	glutTimerFunc(60, timer, 1);
}
void command_a() {

	for (int i = 0; i < triangle_move.size(); i++) {
		triangle_datas temp = triangle_move.front();
		if (i % 2 == 1) {
			if (temp.vector_x == 1) {
				float x;
				x = temp.data[12] - triangle_move.back().data[0];

				temp.data[0] += x;
				temp.data[6] += x;
				temp.data[12] += x;
				

			}
			else if (temp.vector_x == -1) {
				
				float x;
				x = temp.data[0] - triangle_move.back().data[12];
				temp.data[0] -= x;
				temp.data[6] -= x;
				temp.data[12] -= x;

			}
			if (temp.vector_y !=  triangle_move.back().vector_y) {
				if (temp.vector_y == -1) {
					float y;
					y = temp.data[1] - triangle_move.back().data[1];
					temp.data[1] -= y;
					temp.data[7] -= y;
					temp.data[13] -= y;
				}
				else if (temp.vector_y == 1) {
					float y;
					y = triangle_move.back().data[1] - temp.data[1];
					temp.data[1] += y;
					temp.data[7] += y;
					temp.data[13] += y;
				}
			}
			else if (temp.vector_y == triangle_move.back().vector_y) {
				if (temp.vector_y == -1) {
					float y;
					y = temp.data[1] - triangle_move.back().data[1];
					temp.data[1] -= y;
					temp.data[7] -= y;
					temp.data[13] -= y;
				}
				else if (temp.vector_y == 1) {
					float y;
					y = triangle_move.back().data[1] - temp.data[1];
					temp.data[1] += y;
					temp.data[7] += y;
					temp.data[13] += y;
				}
			}
		}
		triangle_move.pop();
		triangle_move.push(temp);
	}
	devide_connect_mode = false;
}

void command_b() {
	for (int i = 0; i < triangle_move.size(); i++) {
		triangle_datas temp = triangle_move.front();
		if (i % 2 == 1) {
			if (temp.vector_x == 1) {
				temp.data[0] -= 0.1;
				temp.data[6] -= 0.1;
				temp.data[12] -= 0.1;

			}
			else if (temp.vector_x == -1) {
				temp.data[0] += 0.1;
				temp.data[6] += 0.1;
				temp.data[12] += 0.1;

			}
		}
		triangle_move.pop();
		triangle_move.push(temp);
	}
	devide_connect_mode = true;
}

GLvoid keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'm':
		if (command_right_change) {
			command_m();
		}
		check = true;
		cout << "right move(if triangle move at the first)" << endl;
		break;
	case 'n':
		if (command_left_change) {
			command_n();
		}
		check = true;

		cout << "left move(if triangle move at the first)" << endl;
		break;
	case '+':
		if (xstep <= 0.06) {
			xstep += 0.01;
			cout << "speed up" << endl;
		}
		else {
			cout << "speed is maximum" << endl;
		}
		break;
	case '-':
		if (xstep >= 0.02) {
			xstep -= 0.01;
			cout << "speed down" << endl;
		}
		else {
			cout << "speed is minimum" << endl;
		}
		break;
	case 'a':
		if (devide_connect_mode) {
			command_a();
			cout << "connect triangle" << endl;
		}
		else {
			cout << "triangle is not devided" << endl;
		}
		break;
	case 'b':
		if (!devide_connect_mode) {
			command_b();
			cout << "devide triangle" << endl;
		}
		else {
			cout << "triangle is not connected" << endl;
		}
		break;
	case 'q':
		exit(0);
		cout << "exit the program" << endl;
		break;
	default:
		break;
	}
}
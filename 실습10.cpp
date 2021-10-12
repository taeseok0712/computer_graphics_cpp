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
void save();
void initbuffer();
void make_vertexShaders();
void make_fragmentShaders();

void timer(int value);
void resize(int width, int height);
char* filetobuf(const string name);

GLuint make_shaderProgram();
GLvoid reshape(int w, int h);
GLvoid drawscene(GLvoid);
GLvoid keyboard(unsigned char key, int x, int y);

GLboolean Count = true;
GLboolean check = true;
bool left_button;
bool onesave = true;
bool changemode = true;
bool make_rectangle = true;

float ystep = 0.005;
int oringinal_count = 0;
int windowWidth, windowHeight;

const GLchar* vertexShaderSource;
const GLchar* fragmentShaderSource;

GLuint VAO[9], VBO[18];
GLuint shaderID;
GLuint vertexshader;
GLuint fragmentshader;
GLuint triangleshaderProgram;

float InnerTriangelData[] = {
	-0.4,0.2,0,1,0,0,   //0~5
	-0.6,0.2,0,1,0,0,  //6~11
	-0.2,0.2,0,1,0,0,  //12~17
	
	0.2,0.2,0,1,0,0,  //18~23
	0.4,0.6,0,1,0,0,  //24~29
	0.6,0.2,0,1,0,0,  //30
	
	0.4,0.6,0,1,0,0,  //36
	0.4,0.6,0,1,0,0,  //42
	0.6,0.2,0,1,0,0,  //48

	-0.5,-0.4,0,1,0,0,  //54
	-0.5,-0.6,0,1,0,0,  //60
	-0.3,-0.6,0,1,0,0,	//66
	
	-0.5,-0.4,0,1,0,0,  //72
	-0.3,-0.4,0,1,0,0,  //78
	-0.3,-0.6,0,1,0,0,  //84
	
	-0.5,-0.4,0,1,0,0,  //90
	-0.4,-0.4,0,1,0,0,  //96
	-0.3,-0.4,0,1,0,0,  //102

	0.2,-0.4,0,1,0,0,  //108
	0.3,-0.6,0,1,0,0,  //114
	0.5,-0.6,0,1,0,0,  //120

	0.2,-0.4,0,1,0,0,  //126
	0.5,-0.6,0,1,0,0,  //132
	0.6,-0.4,0,1,0,0,  //138
	
	0.2,-0.4,0,1,0,0,  //144
	0.4,-0.2,0,1,0,0,  //150
	0.6,-0.4,0,1,0,0   //156
};
float InnerTriangelData2[] = {
	-0.4,0.2,0,1,0,0,   //0~5
	-0.6,0.2,0,1,0,0,  //6~11
	-0.2,0.2,0,1,0,0,  //12~17

	0.2,0.2,0,1,0,0,  //18~23
	0.4,0.6,0,1,0,0,  //24~29
	0.6,0.2,0,1,0,0,  //30

	0.4,0.6,0,1,0,0,  //36
	0.4,0.6,0,1,0,0,  //42
	0.6,0.2,0,1,0,0,  //48

	-0.5,-0.4,0,1,0,0,  //54
	-0.5,-0.6,0,1,0,0,  //60
	-0.3,-0.6,0,1,0,0,	//66

	-0.5,-0.4,0,1,0,0,  //72
	-0.3,-0.4,0,1,0,0,  //78
	-0.3,-0.6,0,1,0,0,  //84

	-0.5,-0.4,0,1,0,0,  //90
	-0.4,-0.4,0,1,0,0,  //96
	-0.3,-0.4,0,1,0,0,  //102

	0.2,-0.4,0,1,0,0,  //108
	0.3,-0.6,0,1,0,0,  //114
	0.5,-0.6,0,1,0,0,  //120

	0.2,-0.4,0,1,0,0,  //126
	0.5,-0.6,0,1,0,0,  //132
	0.6,-0.4,0,1,0,0,  //138

	0.2,-0.4,0,1,0,0,  //144
	0.4,-0.2,0,1,0,0,  //150
	0.6,-0.4,0,1,0,0   //156
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
	
	save();
	
	initbuffer();
	glUseProgram(shaderID);
	for (int i = 0; i < 1; i++) {
		if (changemode) {
			glBindVertexArray(VAO[i]);
			glDrawArrays(GL_LINES, 1, 2);
			glDrawArrays(GL_TRIANGLES, 3, 25);
			changemode = false;
		}
		else {
			glBindVertexArray(VAO[i]);
			glDrawArrays(GL_TRIANGLES, 0, 28);
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
void initbuffer() {
	
	glGenVertexArrays(9, VAO);	
	for (int i = 0; i < 1; i++) {
		glBindVertexArray(VAO[i]);
		glGenBuffers(2, &VBO[2 * i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2 * i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(InnerTriangelData), InnerTriangelData, GL_STREAM_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2 * i + 1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(InnerTriangelData), InnerTriangelData, GL_STREAM_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
}
void timer(int value) {
	if (check) {
		
		if (InnerTriangelData[1] <= 0.6) {
			InnerTriangelData[1] += 0.01;
			if (InnerTriangelData[1] >= 0.6) {
				oringinal_count++;
			}
		}
		if ((InnerTriangelData[24] >= 0.2) && (InnerTriangelData[36] >= 0.2)) {
			InnerTriangelData[24] -= ystep;
			InnerTriangelData[36] -= ystep;
			if ((InnerTriangelData[24] <= 0.2) && (InnerTriangelData[36] <= 0.2)) {
				oringinal_count++;
			}
		}
		if (InnerTriangelData[42] <= 0.6) {
			InnerTriangelData[42] += ystep;
			if (InnerTriangelData[42] >= 0.6) {
				oringinal_count++;
			}

		}


		if ((InnerTriangelData[54] >= -0.6)&& (InnerTriangelData[72] >= -0.6)&& (InnerTriangelData[90] >= -0.6)) {
			InnerTriangelData[54] -= ystep;
			InnerTriangelData[72] -= ystep;
			InnerTriangelData[90] -= ystep;
			if ((InnerTriangelData[54] <= -0.6) && (InnerTriangelData[72] <= -0.6) && (InnerTriangelData[90] <= -0.6)) {
				oringinal_count++;
			}
		}
		if ((InnerTriangelData[78] <= -0.2) && (InnerTriangelData[102] <= -0.2)) {
			InnerTriangelData[78] += ystep;
			InnerTriangelData[102] += ystep;
			if ((InnerTriangelData[78] >= -0.2) && (InnerTriangelData[102] >= -0.2)) {
				oringinal_count++;
			}
		}
		if (InnerTriangelData[97] <= -0.2) {
			InnerTriangelData[97] += ystep;
			if (InnerTriangelData[97] >= -0.2) {
				oringinal_count++;
			}
		}


		if ((InnerTriangelData[108] <= 0.4) && (InnerTriangelData[126] <= 0.4) && (InnerTriangelData[144] <= 0.4)) {
			InnerTriangelData[108] += ystep;
			InnerTriangelData[126] += ystep;
			InnerTriangelData[144] += ystep;
			if ((InnerTriangelData[108] >= 0.4) && (InnerTriangelData[126] >= 0.4) && (InnerTriangelData[144] >= 0.4)) {
				oringinal_count++;
			}
		}
		if (InnerTriangelData[114] <= 0.4) {
			InnerTriangelData[114] += 0.0025;
			if (InnerTriangelData[114] >= 0.4) {
				oringinal_count++;
			}
		}
		if ((InnerTriangelData[120] >= 0.4) && (InnerTriangelData[132] >= 0.4)) {
			InnerTriangelData[120] -= 0.0025;
			InnerTriangelData[132] -= 0.0025;
			if ((InnerTriangelData[120] <= 0.4) && (InnerTriangelData[132] <= 0.4)) {
				oringinal_count++;
			}
		}


		if ((InnerTriangelData[138] >= 0.4) && (InnerTriangelData[156] >= 0.4)) {
			InnerTriangelData[138] -= ystep;
			InnerTriangelData[156] -= ystep;
			if ((InnerTriangelData[138] <= 0.4) && (InnerTriangelData[156] <= 0.4)) {
				oringinal_count++;
			}
		}
		if (InnerTriangelData[151] >= -0.4) {
			InnerTriangelData[151] -= ystep;
			if (InnerTriangelData[151] <= -0.4) {
				oringinal_count++;
			}
		}
		if ((InnerTriangelData[115] <= -0.4) && (InnerTriangelData[121] <= -0.4) && (InnerTriangelData[133] <= -0.4)) {
			InnerTriangelData[115] += ystep;
			InnerTriangelData[121] += ystep;
			InnerTriangelData[133] += ystep;
			if ((InnerTriangelData[115] >= -0.4) && (InnerTriangelData[121] >= -0.4) && (InnerTriangelData[133] >= -0.4)) {
				oringinal_count++;
			}
		}
	}

	glutPostRedisplay();
	glutTimerFunc(60, timer, 1);
}

void save() {
	if (oringinal_count >= 12) {
		for (int i = 0; i < 162; i++) {
			InnerTriangelData[i] = InnerTriangelData2[i];
		}
		oringinal_count = 0;
		changemode = true;
	}
	
}
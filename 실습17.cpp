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
#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include "readObj.h"

using namespace std;

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

bool ChangeShape = true;

bool TriOpenSlide = false;
bool TriCloseSlide = false;
bool ModelRotate = false;
bool ModelUpRotate = false;
bool OpenSlide = false;
bool CloseSlide = false;
bool OpenFront = false;
bool CloseFront = false;
bool ScreenChange = true;

int windowWidth, windowHeight;
int RotateCountx = 0;
int RotateCountX = 0;
int RotateCounty = 0;
int RotateCountY = 0;

float AxisAnglex = 0.0f;
float AxisAngley = 0.0f;

struct ModelAngle {
	float upAngle = 0.0f;
	float frontAngel = 0.0f;
	float totalAngle1 = 0.0f;
	float totalAngle2 = 0.0f;
}ModelAngleSize;

struct CameraViewPoint {
	float ViewX = -0.5f;
	float ViewY = 1.0f;
	float ViewZ = 5.0f;
}CameraPoint;

struct ModelTrans {
	float transX = 0.0f;
	float transY = 0.0f;
	float transZ = 0.0f;

	float trileft = 0.0f;
	float triright = 0.0f;
	float triback = 0.0f;

	float trifrontX = 0.0f;
	float trifrontY = 0.0f;
	float trifrontZ = 0.0f;


}ModelTransSize;

vector<glm::vec4>ModelVertexValue[11];

GLuint VAO[12], VBO[13];
GLuint shaderID[2];
GLuint qobjshader;
GLuint vertexshader;
GLuint fragmentshader;
GLuint triangleshaderProgram;

float make_line[] = {
	0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,

	-1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 1.0f, 0.5f, 0.0f, 0.5f,
	0.0f, 0.0f, -1.0f, 0.5f, 0.0f, 0.5f
};

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
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
	ReadObj("square1.obj", ModelVertexValue[0]);
	ReadObj("square2.obj", ModelVertexValue[1]);
	ReadObj("square3.obj", ModelVertexValue[2]);
	ReadObj("square1.obj", ModelVertexValue[3]);
	ReadObj("square2.obj", ModelVertexValue[4]);
	ReadObj("square3.obj", ModelVertexValue[5]);

	ReadObj("square1.obj", ModelVertexValue[6]);
	ReadObj("triangle1.obj", ModelVertexValue[7]);
	ReadObj("triangle2.obj", ModelVertexValue[8]);
	ReadObj("triangle3.obj", ModelVertexValue[9]);
	ReadObj("triangle4.obj", ModelVertexValue[10]);

	make_vertexShaders();
	make_fragmentShaders();
	initbuffer();
	glutDisplayFunc(drawscene);
	glutKeyboardFunc(keyboard);
	make_shaderProgram();
	glutTimerFunc(1, timer, 1);
	glutReshapeFunc(resize);
	glutMainLoop();
}
void make_vertexShaders() {
	GLchar* vertexsource1;

	vertexsource1 = filetobuf("vertexshader.glsl");

	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexshader, 1, &vertexsource1, NULL);
	glCompileShader(vertexshader);

	GLchar* vertexsource2;

	vertexsource2 = filetobuf("qobjvertexshader.glsl");

	qobjshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(qobjshader, 1, &vertexsource2, NULL);
	glCompileShader(qobjshader);

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
	shaderID[0] = glCreateProgram();

	glAttachShader(shaderID[0], vertexshader);
	glAttachShader(shaderID[0], fragmentshader);

	shaderID[1] = glCreateProgram();

	glAttachShader(shaderID[1], qobjshader);
	glAttachShader(shaderID[1], fragmentshader);

	glLinkProgram(shaderID[0]);
	glLinkProgram(shaderID[1]);

	glDeleteShader(vertexshader);
	glDeleteShader(qobjshader);
	glDeleteShader(fragmentshader);

	GLint result;
	GLchar errorlog[512];

	glGetProgramiv(shaderID[0], GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(triangleshaderProgram, 512, NULL, errorlog);
		cerr << "Error: shader program 연결 실패" << errorlog << endl;
	}
	glUseProgram(shaderID[0]);
	glUseProgram(shaderID[1]);

	return 1;
}

GLvoid drawscene() {
	glClearColor(0.5, 0.5, 0.5, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	
	initbuffer();

	glUseProgram(shaderID[0]);

    glm::vec3 CameraPos = glm::vec3(CameraPoint.ViewX, CameraPoint.ViewY, CameraPoint.ViewZ); //--- 카메라 위치
    glm::vec3 CameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
    glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	
	if (ScreenChange) {
		glm::mat4 ScreenProj = glm::mat4(1.0f);
		ScreenProj = glm::perspective(glm::radians(45.0f), (float)windowWidth / windowHeight, 0.1f, 100.0f);
		unsigned int ProjLocation = glGetUniformLocation(shaderID[0], "projectiontransform");
		glUniformMatrix4fv(ProjLocation, 1, GL_FALSE, &ScreenProj[0][0]); //투영
	}
	else {
		glm::mat4 ScreenOrtho = glm::mat4(1.0f);
		ScreenOrtho = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.7f, 20.0f);
		unsigned int OrthoLocation = glGetUniformLocation(shaderID[0], "projectiontransform");
		glUniformMatrix4fv(OrthoLocation, 1, GL_FALSE, glm::value_ptr(ScreenOrtho)); //투영
	}

    glm::mat4 ScreenView = glm::mat4(1.0f);
    ScreenView = glm::lookAt(CameraPos, CameraDirection, CameraUp);
	unsigned int ViewModelLocation = glGetUniformLocation(shaderID[0], "viewtransform");
    glUniformMatrix4fv(ViewModelLocation, 1, GL_FALSE, glm::value_ptr(ScreenView)); //뷰

	glm::mat4 AxisModel = glm::mat4(1.0f);
	AxisModel = glm::rotate(AxisModel, glm::radians(AxisAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
	AxisModel = glm::rotate(AxisModel, glm::radians(AxisAngley), glm::vec3(0.0f, 1.0f, 0.0f));
	AxisModel = glm::translate(AxisModel, glm::vec3(0, 0, 0));
	unsigned int modelLocation = glGetUniformLocation(shaderID[0], "modeltransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(AxisModel));
	glBindVertexArray(VAO[0]);
	glLineWidth(2.0f);
	glDrawArrays(GL_LINES, 0, 6);

	if (ChangeShape) {
		glUseProgram(shaderID[1]);

		glm::vec3 ModelCameraPos = glm::vec3(CameraPoint.ViewX, CameraPoint.ViewY, CameraPoint.ViewZ); //--- 카메라 위치
		glm::vec3 ModelCameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
		glm::vec3 ModelCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		if (ScreenChange) {
			glm::mat4 ModelScreenProj = glm::mat4(1.0f);
			ModelScreenProj = glm::perspective(glm::radians(45.0f), (float)windowWidth / windowHeight, 0.1f, 100.0f);
			unsigned int ModelProjLocation = glGetUniformLocation(shaderID[1], "projectiontransform");
			glUniformMatrix4fv(ModelProjLocation, 1, GL_FALSE, &ModelScreenProj[0][0]); //투영
		}
		else {
			glm::mat4 ScreenOrtho = glm::mat4(1.0f);
			ScreenOrtho = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.7f, 20.0f);
			unsigned int OrthoLocation = glGetUniformLocation(shaderID[1], "projectiontransform");
			glUniformMatrix4fv(OrthoLocation, 1, GL_FALSE, glm::value_ptr(ScreenOrtho)); //투영
		}
		glm::mat4 ModelScreenView = glm::mat4(1.0f);
		ModelScreenView = glm::lookAt(ModelCameraPos, ModelCameraDirection, ModelCameraUp);
		unsigned int ModelViewLocation = glGetUniformLocation(shaderID[1], "viewtransform");
		glUniformMatrix4fv(ModelViewLocation, 1, GL_FALSE, glm::value_ptr(ModelScreenView)); //뷰

		glBindVertexArray(VAO[1]);
		glm::mat4 HexaUpModel = glm::mat4(1.0f);
		HexaUpModel = glm::rotate(HexaUpModel, glm::radians(AxisAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
		HexaUpModel = glm::rotate(HexaUpModel, glm::radians(AxisAngley), glm::vec3(0.0f, 1.0f, 0.0f));
		HexaUpModel = glm::translate(HexaUpModel, glm::vec3(0.0f, 0.51f, 0.0f));
		HexaUpModel = glm::rotate(HexaUpModel, glm::radians(ModelAngleSize.upAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		unsigned int ModelUpLocation = glGetUniformLocation(shaderID[1], "modeltransform");
		glUniformMatrix4fv(ModelUpLocation, 1, GL_FALSE, glm::value_ptr(HexaUpModel));
		unsigned int ModelUpFragLocation = glGetUniformLocation(shaderID[1], "vColor");
		glUniform3f(ModelUpFragLocation, 1.0f, 0.0f, 0.0f);
		glDrawArrays(GL_QUADS, 0, 24);

		glBindVertexArray(VAO[4]);
		glm::mat4 HexaDownModel = glm::mat4(1.0f);
		HexaDownModel = glm::rotate(HexaDownModel, glm::radians(AxisAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
		HexaDownModel = glm::rotate(HexaDownModel, glm::radians(AxisAngley), glm::vec3(0.0f, 1.0f, 0.0f));
		HexaDownModel = glm::translate(HexaDownModel, glm::vec3(0.0f, -0.51f, 0.0f));
		HexaDownModel = glm::rotate(HexaDownModel, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		HexaDownModel = glm::rotate(HexaDownModel, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		unsigned int ModelDownLocation = glGetUniformLocation(shaderID[1], "modeltransform");
		glUniformMatrix4fv(ModelDownLocation, 1, GL_FALSE, glm::value_ptr(HexaDownModel));
		unsigned int ModelDownFragLocation = glGetUniformLocation(shaderID[1], "vColor");
		glUniform3f(ModelDownFragLocation, 0.86f, 0.625f, 0.86f);
		glDrawArrays(GL_QUADS, 0, 24);

		glBindVertexArray(VAO[2]);
		glm::mat4 HexaFrontModel = glm::mat4(1.0f);
		HexaFrontModel = glm::rotate(HexaFrontModel, glm::radians(AxisAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
		HexaFrontModel = glm::rotate(HexaFrontModel, glm::radians(AxisAngley), glm::vec3(0.0f, 1.0f, 0.0f));
		HexaFrontModel = glm::translate(HexaFrontModel, glm::vec3(0.0f, 0.0f, ModelTransSize.transZ));
		HexaFrontModel = glm::rotate(HexaFrontModel, glm::radians(ModelAngleSize.frontAngel), glm::vec3(1.0f, 0.0f, 0.0f));
		HexaFrontModel = glm::translate(HexaFrontModel, glm::vec3(0.0f, 0.0f, 0.51f));
		HexaFrontModel = glm::rotate(HexaFrontModel, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		unsigned int ModelFrontLocation = glGetUniformLocation(shaderID[1], "modeltransform");
		glUniformMatrix4fv(ModelFrontLocation, 1, GL_FALSE, glm::value_ptr(HexaFrontModel));
		unsigned int ModelFrontFragLocation = glGetUniformLocation(shaderID[1], "vColor");
		glUniform3f(ModelFrontFragLocation, 1.0f, 0.4f, 0.0f);
		glDrawArrays(GL_QUADS, 0, 24);

		glBindVertexArray(VAO[5]);
		glm::mat4 HexaBackModel = glm::mat4(1.0f);
		HexaBackModel = glm::rotate(HexaBackModel, glm::radians(AxisAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
		HexaBackModel = glm::rotate(HexaBackModel, glm::radians(AxisAngley), glm::vec3(0.0f, 1.0f, 0.0f));
		HexaBackModel = glm::translate(HexaBackModel, glm::vec3(0.0f, 0.0f, -0.51f));
		HexaBackModel = glm::rotate(HexaBackModel, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		HexaBackModel = glm::rotate(HexaBackModel, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		unsigned int ModelBackLocation = glGetUniformLocation(shaderID[1], "modeltransform");
		glUniformMatrix4fv(ModelBackLocation, 1, GL_FALSE, glm::value_ptr(HexaBackModel));
		unsigned int ModelBackFragLocation = glGetUniformLocation(shaderID[1], "vColor");
		glUniform3f(ModelBackFragLocation, 1.0f, 1.0f, 0.0f);
		glDrawArrays(GL_QUADS, 0, 24);

		glBindVertexArray(VAO[3]);
		glm::mat4 HexaLeftModel = glm::mat4(1.0f);
		HexaLeftModel = glm::rotate(HexaLeftModel, glm::radians(AxisAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
		HexaLeftModel = glm::rotate(HexaLeftModel, glm::radians(AxisAngley), glm::vec3(0.0f, 1.0f, 0.0f));
		HexaLeftModel = glm::translate(HexaLeftModel, glm::vec3(-0.51, 0.0f, 0.0f));
		HexaLeftModel = glm::translate(HexaLeftModel, glm::vec3(0.0f, ModelTransSize.transY, 0.0f));
		HexaLeftModel = glm::rotate(HexaLeftModel, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		HexaLeftModel = glm::rotate(HexaLeftModel, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		unsigned int ModelLeftLocation = glGetUniformLocation(shaderID[1], "modeltransform");
		glUniformMatrix4fv(ModelLeftLocation, 1, GL_FALSE, glm::value_ptr(HexaLeftModel));
		unsigned int ModelLeftFragLocation = glGetUniformLocation(shaderID[1], "vColor");
		glUniform3f(ModelLeftFragLocation, 0.54f, 0.0f, 1.0f);
		glDrawArrays(GL_QUADS, 0, 24);

		glBindVertexArray(VAO[6]);
		glm::mat4 HexaRightModel = glm::mat4(1.0f);
		HexaRightModel = glm::rotate(HexaRightModel, glm::radians(AxisAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
		HexaRightModel = glm::rotate(HexaRightModel, glm::radians(AxisAngley), glm::vec3(0.0f, 1.0f, 0.0f));
		HexaRightModel = glm::translate(HexaRightModel, glm::vec3(0.51f, 0.0f, 0.0f));
		HexaRightModel = glm::translate(HexaRightModel, glm::vec3(0.0f, ModelTransSize.transY, 0.0f));
		HexaRightModel = glm::rotate(HexaRightModel, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		HexaRightModel = glm::rotate(HexaRightModel, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		unsigned int ModelRightLocation = glGetUniformLocation(shaderID[1], "modeltransform");
		glUniformMatrix4fv(ModelRightLocation, 1, GL_FALSE, glm::value_ptr(HexaRightModel));
		unsigned int ModelRightFragLocation = glGetUniformLocation(shaderID[1], "vColor");
		glUniform3f(ModelRightFragLocation, 0.54f, 0.0f, 1.0f);
		glDrawArrays(GL_QUADS, 0, 24);
	}
	else {
	glUseProgram(shaderID[1]);

	glm::vec3 ModelCameraPos = glm::vec3(CameraPoint.ViewX, CameraPoint.ViewY, CameraPoint.ViewZ); //--- 카메라 위치
	glm::vec3 ModelCameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
	glm::vec3 ModelCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	if (ScreenChange) {
		glm::mat4 ModelScreenProj = glm::mat4(1.0f);
		ModelScreenProj = glm::perspective(glm::radians(45.0f), (float)windowWidth / windowHeight, 0.1f, 100.0f);
		unsigned int ModelProjLocation = glGetUniformLocation(shaderID[1], "projectiontransform");
		glUniformMatrix4fv(ModelProjLocation, 1, GL_FALSE, &ModelScreenProj[0][0]); //투영
	}
	else {
		glm::mat4 ScreenOrtho = glm::mat4(1.0f);
		ScreenOrtho = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.7f, 20.0f);
		unsigned int OrthoLocation = glGetUniformLocation(shaderID[1], "projectiontransform");
		glUniformMatrix4fv(OrthoLocation, 1, GL_FALSE, glm::value_ptr(ScreenOrtho)); //투영
	}
	glm::mat4 ModelScreenView = glm::mat4(1.0f);
	ModelScreenView = glm::lookAt(ModelCameraPos, ModelCameraDirection, ModelCameraUp);
	unsigned int ModelViewLocation = glGetUniformLocation(shaderID[1], "viewtransform");
	glUniformMatrix4fv(ModelViewLocation, 1, GL_FALSE, glm::value_ptr(ModelScreenView)); //뷰

	glBindVertexArray(VAO[7]);
	glm::mat4 SquareModel = glm::mat4(1.0f);
	SquareModel = glm::rotate(SquareModel, glm::radians(AxisAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
	SquareModel = glm::rotate(SquareModel, glm::radians(AxisAngley), glm::vec3(0.0f, 1.0f, 0.0f));
	unsigned int SquareModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
	glUniformMatrix4fv(SquareModelLocation, 1, GL_FALSE, glm::value_ptr(SquareModel));
	unsigned int SquareModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
	glUniform3f(SquareModelFragLocation, 1.0f, 0.0f, 0.0f);
	glDrawArrays(GL_QUADS, 0, 24);

	glBindVertexArray(VAO[8]);
	glm::mat4 TriLeftModel = glm::mat4(1.0f);
	TriLeftModel = glm::rotate(TriLeftModel, glm::radians(AxisAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
	TriLeftModel = glm::rotate(TriLeftModel, glm::radians(AxisAngley), glm::vec3(0.0f, 1.0f, 0.0f));
	TriLeftModel = glm::translate(TriLeftModel, glm::vec3(0.5f,0.0f,0.0f));
	TriLeftModel = glm::rotate(TriLeftModel, glm::radians(ModelAngleSize.totalAngle2), glm::vec3(0.0f, 0.0f, 1.0f));
	TriLeftModel = glm::translate(TriLeftModel, glm::vec3(-0.5f, 0.0f, 0.0f));
	unsigned int TriLeftModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
	glUniformMatrix4fv(TriLeftModelLocation, 1, GL_FALSE, glm::value_ptr(TriLeftModel));
	unsigned int TriLeftModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
	glUniform3f(TriLeftModelFragLocation, 0.86f, 0.625f, 0.86f);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(VAO[9]);
	glm::mat4 TriBackModel = glm::mat4(1.0f);
	TriBackModel = glm::rotate(TriBackModel, glm::radians(AxisAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
	TriBackModel = glm::rotate(TriBackModel, glm::radians(AxisAngley), glm::vec3(0.0f, 1.0f, 0.0f));
	TriBackModel = glm::translate(TriBackModel, glm::vec3(0.0f,0.0f,-0.5f));
	TriBackModel = glm::rotate(TriBackModel, glm::radians(ModelAngleSize.totalAngle2), glm::vec3(1.0f, 0.0f, 0.0f));
	TriBackModel = glm::translate(TriBackModel, glm::vec3(0.0f, 0.0f, 0.5f));
	unsigned int TriBackModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
	glUniformMatrix4fv(TriBackModelLocation, 1, GL_FALSE, glm::value_ptr(TriBackModel));
	unsigned int TriBackModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
	glUniform3f(TriBackModelFragLocation, 1.0f, 0.4f, 0.0f);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(VAO[10]);
	glm::mat4 TriRightModel = glm::mat4(1.0f);
	TriRightModel = glm::rotate(TriRightModel, glm::radians(AxisAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
	TriRightModel = glm::rotate(TriRightModel, glm::radians(AxisAngley), glm::vec3(0.0f, 1.0f, 0.0f));
	TriRightModel = glm::translate(TriRightModel, glm::vec3(-0.5f,0.0f,0.0f));
	TriRightModel = glm::rotate(TriRightModel, glm::radians(ModelAngleSize.totalAngle1), glm::vec3(0.0f, 0.0f, 1.0f));
	TriRightModel = glm::translate(TriRightModel, glm::vec3(0.5f, 0.0f, 0.0f));
	unsigned int TriRightModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
	glUniformMatrix4fv(TriRightModelLocation, 1, GL_FALSE, glm::value_ptr(TriRightModel));
	unsigned int TriRightModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
	glUniform3f(TriRightModelFragLocation, 1.0f, 1.0f, 0.0f);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(VAO[11]);
	glm::mat4 TriFrontModel = glm::mat4(1.0f);
	TriFrontModel = glm::rotate(TriFrontModel, glm::radians(AxisAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
	TriFrontModel = glm::rotate(TriFrontModel, glm::radians(AxisAngley), glm::vec3(0.0f, 1.0f, 0.0f));
	TriFrontModel = glm::translate(TriFrontModel, glm::vec3(0.0f,0.0f,0.5f));
	TriFrontModel = glm::rotate(TriFrontModel, glm::radians(ModelAngleSize.totalAngle1), glm::vec3(1.0f, 0.0f, 0.0f));
	TriFrontModel = glm::translate(TriFrontModel, glm::vec3(0.0f, 0.0f, -0.5f));

	unsigned int TriFrontModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
	glUniformMatrix4fv(TriFrontModelLocation, 1, GL_FALSE, glm::value_ptr(TriFrontModel));
	unsigned int TriFrontModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
	glUniform3f(TriFrontModelFragLocation, 0.54f, 0.0f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, 3);
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
	glGenVertexArrays(12, VAO);

	glBindVertexArray(VAO[0]);
	glGenBuffers(2, &VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(make_line), make_line, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(make_line), make_line, GL_STREAM_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	if (ChangeShape) {
		for (int i = 1; i < 7; i++) {
			glBindVertexArray(VAO[i]);
			glGenBuffers(1, &VBO[i+1]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[i+1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * ModelVertexValue[i-1].size(), &ModelVertexValue[i-1][0], GL_STREAM_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
			glEnableVertexAttribArray(0);
		}
	}
	else {
		for (int i = 7; i < 12; i++) {
			glBindVertexArray(VAO[i]);
			glGenBuffers(1, &VBO[i + 1]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[i + 1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * ModelVertexValue[i - 1].size(), &ModelVertexValue[i - 1][0], GL_STREAM_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
			glEnableVertexAttribArray(0);
		}
	}
}

GLvoid keyboard(unsigned char key, int x, int y) {
	switch (key){
	case 'c':
		AxisAngley = 0.0f;
		AxisAnglex = 0.0f;
		memset(&ModelAngleSize, 0, sizeof(ModelAngleSize));
		memset(&ModelTransSize, 0, sizeof(ModelTransSize));
		CameraPoint.ViewX = -0.5f;
		CameraPoint.ViewY = 1.0f;
		CameraPoint.ViewZ = 3.0f;
		ChangeShape = true;	
		TriOpenSlide = false;
		TriCloseSlide = false;
		ModelRotate = false;
		ModelUpRotate = false;
		OpenSlide = false;
		CloseSlide = false;
		OpenFront = false;
		CloseFront = false;
		break;
	case 'l':
		AxisAngley = 0.0f;
		AxisAnglex = 0.0f;
		memset(&ModelAngleSize, 0, sizeof(ModelAngleSize));
		memset(&ModelTransSize, 0, sizeof(ModelTransSize));
		CameraPoint.ViewX = -0.5f;
		CameraPoint.ViewY = 1.0f;
		CameraPoint.ViewZ = 3.0f;
		TriOpenSlide = false;
		TriCloseSlide = false;
		ModelRotate = false;
		ModelUpRotate = false;
		OpenSlide = false;
		CloseSlide = false;
		OpenFront = false;
		CloseFront = false;
		ChangeShape = false;
		break;
	case 'o':
		TriOpenSlide = true;
		TriCloseSlide = false;
		ChangeShape = false;
		break;
	case 'O':
		TriOpenSlide = false;
		TriCloseSlide = true;
		ChangeShape = false;
		break;
	case 't':
		ChangeShape = true;
		ModelUpRotate = !ModelUpRotate;
		break;
	case 'f':
		ChangeShape = true;
		OpenFront = true;
		CloseFront = false;
		break;
	case 'F':
		ChangeShape = true;
		CloseFront = true;
		OpenFront = false;
		break;
	case 'y':
		ModelRotate = !ModelRotate;
		break;
	case 'p':
		ScreenChange = !ScreenChange;
		break;
	case '1':
		ChangeShape = true;
		OpenSlide = true;
		CloseSlide = false;
		break;
	case '2':
		ChangeShape = true;
		OpenSlide = false;
		CloseSlide = true;
		break;
	case 'q':
		exit(0);
		cout << "exit the program" << endl;
		break;
	default:
		break;
	}
	glutPostRedisplay();

}

void timer(int value) {
	if (OpenSlide) {
		if (ModelTransSize.transY >= 1.0) {
			OpenSlide = false;
		}
		else {
			ModelTransSize.transY += 0.02;
		}
	}
	if (CloseSlide) {
		if (ModelTransSize.transY <= 0.0) {
			CloseSlide = false;
		}
		else {
			ModelTransSize.transY -= 0.02;
		}
	}

	if (OpenFront) {
		if (ModelAngleSize.frontAngel >= 90.0) {
			OpenFront = false;
			ModelTransSize.transZ = 1.0f;
		}
		else {
			ModelAngleSize.frontAngel++;
			ModelTransSize.transZ += 0.011f;
		}
	}
	if (CloseFront) {
		if (ModelAngleSize.frontAngel <= 0.0) {
			CloseFront = false;
			ModelTransSize.transZ = 0.0f;
		}
		else {
			ModelAngleSize.frontAngel--;
			ModelTransSize.transZ -= 0.011f;
		}
	}
	if (TriOpenSlide) {
		if (ModelAngleSize.totalAngle1 >= 233.13010236+0.02) {
			TriOpenSlide = false;
			ModelAngleSize.totalAngle1 = 233.13010236 + 0.02;
		}
		else {	
			ModelAngleSize.totalAngle1++;
			ModelAngleSize.totalAngle2--;
		}
	}
	if (TriCloseSlide) {
		if (ModelAngleSize.totalAngle1 <= 0.0) {
			TriCloseSlide = false;
			ModelAngleSize.totalAngle1 = 0;
		}
		else {
			ModelAngleSize.totalAngle1--;
			ModelAngleSize.totalAngle2++;
		}
	}
	if (ModelUpRotate) {
		ModelAngleSize.upAngle++;
	}

	if (ModelRotate) {
		AxisAngley++;
	}

	glutPostRedisplay();
	glutTimerFunc(30, timer, 1);
}
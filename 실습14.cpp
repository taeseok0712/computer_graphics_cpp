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

using namespace std;

void initbuffer();
void make_vertexShaders();
void make_fragmentShaders();
void timer(int value);
void resize(int width, int height);
void specialkeycall(int key, int x, int y);
char* filetobuf(const string name);
GLuint make_shaderProgram();
GLvoid reshape(int w, int h);
GLvoid drawscene(GLvoid);
GLvoid keyboard(unsigned char key, int x, int y);

bool check = false;
bool program_start = false;
bool change_shape = true;
bool culling = false;
bool polygon_wire_mode = true;
bool data_release = true;
int windowWidth, windowHeight;
int rotate_x = 0;
int rotate_X = 0;
int rotate_y = 0;
int rotate_Y = 0;
float angle_x = 30.0f;
float angle_y = 30.0f;
float trans_x = 0.0f;
float trans_y = 0.0f;
const GLchar* vertexShaderSource;
const GLchar* fragmentShaderSource;

GLuint VAO[3], VBO[6];
GLuint shaderID;
GLuint vertexshader;
GLuint fragmentshader;
GLuint triangleshaderProgram;

float make_line[] = {
	0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,

	-1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
};

float make_hexahedron[] = {
	-0.25f, 0.25f, 0.25f, 1.0f, 0.0f, 0.0f,  
	0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 0.0f,  
	-0.25f, 0.25f, -0.25f, 0.5f, 0.5f, 0.5f,  

	0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 0.0f,  
	0.25f, 0.25f, -0.25f, 0.0f, 0.0f, 1.0f, 
	-0.25f, 0.25f, -0.25f, 0.5f, 0.5f, 0.5f,  

	-0.25f, -0.25f, 0.25f, 0.0f, 0.0f, 0.0f,  
	-0.25f, -0.25f, -0.25f, 0.3f, 0.0f, 0.7f,  
	0.25f, -0.25f, -0.25f, 0.5f, 0.3f, 0.7f,  

	-0.25f, -0.25f, 0.25f, 0.0f, 0.0f, 0.0f,  
	0.25f, -0.25f, -0.25f, 0.5f, 0.3f, 0.7f,  
	0.25f, -0.25f, 0.25f, 0.0f, 1.0f, 1.0f,  

	-0.25f, -0.25f, 0.25f, 0.0f, 0.0f, 0.0f,  
	0.25f, -0.25f, 0.25f, 0.0f, 1.0f, 1.0f,  
	-0.25f, 0.25f, 0.25f, 1.0f, 0.0f, 0.0f,  

	0.25f, -0.25f, 0.25f, 0.0f, 1.0f, 1.0f,  
	0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 0.0f,  
	-0.25f, 0.25f, 0.25f, 1.0f, 0.0f, 0.0f,  

	0.25f, -0.25f, -0.25f, 0.5f, 0.3f, 0.7f,  
	-0.25f, -0.25f, -0.25f, 0.3f, 0.0f, 0.7f,  
	0.25f, 0.25f, -0.25f, 0.0f, 0.0f, 1.0f, 

	-0.25f, -0.25f, -0.25f, 0.3f, 0.0f, 0.7f,  
	-0.25f, 0.25f, -0.25f, 0.5f, 0.5f, 0.5f,  
	0.25f, 0.25f, -0.25f, 0.0f, 0.0f, 1.0f, 

	-0.25f, -0.25f, -0.25f, 0.3f, 0.0f, 0.7f,  
	-0.25f, 0.25f, 0.25f, 1.0f, 0.0f, 0.0f,  
	-0.25f, 0.25f, -0.25f, 0.5f, 0.5f, 0.5f,  

	-0.25f, -0.25f, -0.25f, 0.3f, 0.0f, 0.7f,  
	-0.25f, -0.25f, 0.25f, 0.0f, 0.0f, 0.0f,  
	-0.25f, 0.25f, 0.25f, 1.0f, 0.0f, 0.0f,  

	0.25f, -0.25f, 0.25f, 0.0f, 1.0f, 1.0f, 
	0.25f, -0.25f, -0.25f, 0.5f, 0.3f, 0.7f,
	0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 0.0f,

	0.25f, -0.25f, -0.25f, 0.5f, 0.3f, 0.7f,
	0.25f, 0.25f, -0.25f, 0.0f, 0.0f, 1.0f,
	0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 0.0f 
};

float make_quadrant[] = {
	-0.3f, -0.3f, -0.3f, 0.5f, 0.5f, 0.5f,  //1
	0.0f, 0.3f, 0.0f, 1.0f, 0.0f, 0.0f,  //0
	0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.3f,  //2

	0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.3f,  //2
	0.0f, 0.3f, 0.0f, 1.0f, 0.0f, 0.0f,  //0
	0.3f, -0.3f, 0.3f, 0.0f, 0.7f, 1.0f,  //3

	0.3f, -0.3f, 0.3f, 0.0f, 0.7f, 1.0f,  //3
	0.0f, 0.3f, 0.0f, 1.0f, 0.0f, 0.0f,  //0
	-0.3f, -0.3f, 0.3f, 1.0f, 1.0f, 0.12f,  //4

	- 0.3f, -0.3f, -0.3f, 0.5f, 0.5f, 0.5f,  //1
	-0.3f, -0.3f, 0.3f, 1.0f, 1.0f, 0.12f,  //4
	0.0f, 0.3f, 0.0f, 1.0f, 0.0f, 0.0f,  //0

	-0.3f, -0.3f, 0.3f, 1.0f, 1.0f, 0.12f,  //4
	- 0.3f, -0.3f, -0.3f, 0.5f, 0.5f, 0.5f,  //1
	0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.3f,  //2

	-0.3f, -0.3f, 0.3f, 1.0f, 1.0f, 0.12f,  //4
	0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.3f,  //2
	0.3f, -0.3f, 0.3f, 0.0f, 0.7f, 1.0f  //3
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
	make_vertexShaders();
	make_fragmentShaders();
	shaderID = make_shaderProgram();
	initbuffer();
	glutDisplayFunc(drawscene);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkeycall);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (culling) {
		glEnable(GL_CULL_FACE);
	}
	else {
		glDisable(GL_CULL_FACE);
	}

	initbuffer();
	glUseProgram(shaderID);	
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	unsigned int modelLocation = glGetUniformLocation(shaderID, "modeltransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(VAO[0]);
	glLineWidth(2.0f);
	glDrawArrays(GL_LINES, 0, 4);

	if (program_start) {
		if (change_shape) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model,glm::vec3(trans_x, trans_y, 0.0f));
			model = glm::rotate(model, glm::radians(angle_x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(angle_y), glm::vec3(0.0f, 1.0f, 0.0f));
			unsigned int modelLocation = glGetUniformLocation(shaderID, "modeltransform");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
			
			if (polygon_wire_mode) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			glBindVertexArray(VAO[1]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		else {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(trans_x, trans_y + 0.3f, 0.0f));
			model = glm::rotate(model, glm::radians(angle_x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(angle_y), glm::vec3(0.0f, 1.0f, 0.0f));
			unsigned int modelLocation = glGetUniformLocation(shaderID, "modeltransform");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO[2]);
			if (polygon_wire_mode) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			glDrawArrays(GL_TRIANGLES, 0, 18);
		}
	}
	glutSwapBuffers();
}

GLvoid reshape(int w, int h) {
	glViewport(0, 0, 800, 600);
}
void specialkeycall(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		trans_x -= 0.03f;
		break;
	case GLUT_KEY_RIGHT:
		trans_x += 0.03f;
		break;
	case GLUT_KEY_UP:
		trans_y += 0.03f;
		break;
	case GLUT_KEY_DOWN:
		trans_y -= 0.03f;
		break;
	default:
		break;
	}

}

void resize(int width, int height) {
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, width, height);
}

void initbuffer() {
	
	glGenVertexArrays(3, VAO);
	
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

	if (change_shape) {
		glBindVertexArray(VAO[1]);
		glGenBuffers(2, &VBO[2]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(make_hexahedron), make_hexahedron, GL_STREAM_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(make_hexahedron), make_hexahedron, GL_STREAM_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	else {
		glBindVertexArray(VAO[2]);
		glGenBuffers(2, &VBO[4]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(make_quadrant), make_quadrant, GL_STREAM_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(make_quadrant), make_quadrant, GL_STREAM_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

}

GLvoid keyboard(unsigned char key, int x, int y) {
	switch (key)
	{

	case 'c':
		check = false;
		change_shape = true;
		program_start = true;
		angle_x = 30.0f;
		angle_y = 30.0f;
		trans_x = 0.0f;
		trans_y = 0.0f;
		break;
	case 'p':
		check = false;
		program_start = true;
		change_shape = false;
		angle_x = 30.0f;
		angle_y = 30.0f; 
		trans_x = 0.0f;
		trans_y = 0.0f;
		break;
	case 'h':
		culling = !culling;
		break;
	case 'x':
		rotate_x++;
		rotate_X = 0;
		check = true;
		break;
	case 'X':
		rotate_X++;
		rotate_x = 0;
		check = true;
		break;
	case 'y':
		rotate_y++;
		rotate_Y = 0;
		check = true;
		break;
	case 'Y':
		rotate_Y++;
		rotate_y = 0;
		check = true;
		break;
	case 'w':
		polygon_wire_mode = false;
		break;
	case 'W':
		polygon_wire_mode = true;
		break;
	case 's':
		check = false;
		angle_x = 30.0f;
		angle_y = 30.0f;
		trans_x = 0.0f;
		trans_y = 0.0f;
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
	if (check) {
		if (rotate_x > 0) {
			if (rotate_x % 2 == 1) {
				angle_x++;
			}
			else if (rotate_x % 2 == 0) {
				rotate_x = 0;
			}
		}
		if (rotate_X > 0) {
			if (rotate_X % 2 == 1) {
				angle_x--;
			}
			else if (rotate_X % 2 == 0) {
	
				rotate_X = 0;
			}
		}
		if (rotate_y > 0) {
			if (rotate_y % 2 == 1) {
				angle_y++;
			}
			else if (rotate_y % 2 == 0) {
				rotate_y = 0;
			}
		}
		if (rotate_Y > 0) {
			if (rotate_Y % 2 == 1) {
				angle_y--;
			}
			else if (rotate_Y % 2 == 0) {
				rotate_y = 0;
			}
		}
	}

	glutPostRedisplay();
	glutTimerFunc(60, timer, 1);
}
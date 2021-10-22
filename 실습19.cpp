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

//vertex shader programs
void initbuffer();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const string name);
GLuint make_shaderProgram();

//main loop funtions
void timer(int value);
void resize(int width, int height);
GLvoid drawscene(GLvoid);
GLvoid reshape(int w, int h);
GLvoid keyboard(unsigned char key, int x, int y);

bool CrainAllTransAlpha = false;
bool CrainAllTransBeta = false;

int windowWidth, windowHeight;

struct CameraViewPoint {
    float ViewX = 0.0f;
    float ViewY = 1.0f;
    float ViewZ = 2.3f;
}CameraPoint;

struct CrainTrans {
    float AllTransX = 0.0f;
    float AllTransY = 0.0f;
    float AllTransZ = 0.0f;
}CrainTransSize;


vector<glm::vec4>ModelsValue[5];

GLuint VAO[5], VBO[5];
GLuint shaderID[2];
GLuint qobjshader;
GLuint vertexshader;
GLuint fragmentshader;
GLuint triangleshaderProgram;



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
    //obj 파일읽기
    ReadObj("plane.obj", ModelsValue[0]);
    ReadObj("crainCube.obj", ModelsValue[1]);
    ReadObj("crainCube2.obj", ModelsValue[2]);
    ReadObj("crainArm.obj", ModelsValue[3]);
    ReadObj("crainArm.obj", ModelsValue[4]);
    
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
    glAttachShader(shaderID[0], fragmentshader); // vao 연결한거

    shaderID[1] = glCreateProgram();

    glAttachShader(shaderID[1], qobjshader);
    glAttachShader(shaderID[1], fragmentshader); //vao 말고 다른것들? 

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

    initbuffer();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glUseProgram(shaderID[1]);

    unsigned int ModelViewLocation = glGetUniformLocation(shaderID[1], "viewtransform");
    unsigned int ModelProjLocation = glGetUniformLocation(shaderID[1], "projectiontransform");

    glm::vec3 ModelCameraPos = glm::vec3(CameraPoint.ViewX, CameraPoint.ViewY, CameraPoint.ViewZ); //--- 카메라 위치
    glm::vec3 ModelCameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
    glm::vec3 ModelCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 ModelProj = glm::mat4(1.0f);
    ModelProj = glm::perspective(glm::radians(60.0f), (float)windowWidth / windowHeight, 0.1f, 100.0f);
    glUniformMatrix4fv(ModelProjLocation, 1, GL_FALSE, &ModelProj[0][0]);

    glm::mat4 ModelView = glm::mat4(1.0f);
    ModelView = glm::lookAt(ModelCameraPos, ModelCameraDirection, ModelCameraUp);
    glUniformMatrix4fv(ModelViewLocation, 1, GL_FALSE, glm::value_ptr(ModelView));
    
    //바닥면
    glBindVertexArray(VAO[0]);
    glm::mat4 PlaneModel = glm::mat4(1.0f);
    //PlaneModel = glm::rotate(PlaneModel, glm::radians(AxisAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
    //PlaneModel = glm::rotate(PlaneModel, glm::radians(AxisAngley), glm::vec3(0.0f, 1.0f, 0.0f));
    //PlaneModel = glm::translate(PlaneModel, glm::vec3(CrainTransSize.AllTransX, CrainTransSize.AllTransY, CrainTransSize.AllTransZ));
    PlaneModel = glm::scale(PlaneModel, glm::vec3(2.5f, 2.5f, 2.5f));
    unsigned int PlaneModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(PlaneModelLocation, 1, GL_FALSE, glm::value_ptr(PlaneModel));
    unsigned int PlaneModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(PlaneModelFragLocation, 0.5f, 0.75f, 0.27f);
    glDrawArrays(GL_QUADS, 0, ModelsValue[0].size());

    //탱크 다리부분
    glBindVertexArray(VAO[1]);
    glm::mat4 RailModel = glm::mat4(1.0f);
    //RailModel = glm::rotate(RailModel, glm::radians(AxisAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
    //RailModel = glm::rotate(RailModel, glm::radians(AxisAngley), glm::vec3(0.0f, 1.0f, 0.0f));
    RailModel = glm::translate(RailModel, glm::vec3(CrainTransSize.AllTransX, CrainTransSize.AllTransY, CrainTransSize.AllTransZ));
    //RailModel = glm::scale(RailModel, glm::vec3(0.5f, 0.5f, 0.5f));
    unsigned int RailModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(RailModelLocation, 1, GL_FALSE, glm::value_ptr(RailModel));
    unsigned int RailModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(RailModelFragLocation, 0.5f, 0.5f, 0.0f);
    glDrawArrays(GL_QUADS, 0, 24);


    //탱크 몸통 부분
    glBindVertexArray(VAO[2]);
    glm::mat4 BodyModel = glm::mat4(1.0f);
    //BodyModel = glm::rotate(BodyModel, glm::radians(AxisAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
    //BodyModel = glm::rotate(BodyModel, glm::radians(AxisAngley), glm::vec3(0.0f, 1.0f, 0.0f));
    BodyModel = glm::translate(BodyModel, glm::vec3(CrainTransSize.AllTransX, CrainTransSize.AllTransY, CrainTransSize.AllTransZ));
    //BodyModel = glm::scale(BodyModel, glm::vec3(0.5f, 0.5f, 0.5f));
    unsigned int BodyModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(BodyModelLocation, 1, GL_FALSE, glm::value_ptr(BodyModel));
    unsigned int BodyModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(BodyModelFragLocation, 0.56f, 0.47f, 0.29f);
    glDrawArrays(GL_QUADS, 0, 24);
    
    //탱크 왼쪽 팔 부분
    glBindVertexArray(VAO[3]);
    glm::mat4 LeftArmModel = glm::mat4(1.0f);
    //LeftArmModel = glm::rotate(LeftArmModel, glm::radians(AxisAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
    //LeftArmModel = glm::rotate(LeftArmModel, glm::radians(AxisAngley), glm::vec3(0.0f, 1.0f, 0.0f));
    LeftArmModel = glm::translate(LeftArmModel, glm::vec3(-0.2f, 0.0f, 0.0f));
    LeftArmModel = glm::translate(LeftArmModel, glm::vec3(CrainTransSize.AllTransX, CrainTransSize.AllTransY, CrainTransSize.AllTransZ));
    //LeftArmModel = glm::scale(LeftArmModel, glm::vec3(0.5f, 0.5f, 0.5f));
    unsigned int LeftArmModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(LeftArmModelLocation, 1, GL_FALSE, glm::value_ptr(LeftArmModel));
    unsigned int LeftArmModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(LeftArmModelFragLocation, 0.75f, 0.75f, 0.75f);
    glDrawArrays(GL_QUADS, 0, 24);
    
    //탱크 오른쪽 팔 부분
    glBindVertexArray(VAO[4]);
    glm::mat4 RightArmModel = glm::mat4(1.0f);
    //RightArmModel = glm::rotate(RightArmModel, glm::radians(AxisAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
    //RightArmModel = glm::rotate(RightArmModel, glm::radians(AxisAngley), glm::vec3(0.0f, 1.0f, 0.0f));
    RightArmModel = glm::translate(RightArmModel, glm::vec3(0.2f, 0.0f, 0.0f));
    RightArmModel = glm::translate(RightArmModel, glm::vec3(CrainTransSize.AllTransX, CrainTransSize.AllTransY, CrainTransSize.AllTransZ));
    //RightArmModel = glm::scale(RightArmModel, glm::vec3(0.5f, 0.5f, 0.5f));
    unsigned int RightArmModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(RightArmModelLocation, 1, GL_FALSE, glm::value_ptr(RightArmModel));
    unsigned int RightArmModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(RightArmModelFragLocation, 0.22f, 0.18f, 0.19f);
    glDrawArrays(GL_QUADS, 0, 24);


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
   
    glGenVertexArrays(5, VAO);

    for (int i = 0; i < 5; i++) {
        glBindVertexArray(VAO[i]);
        glGenBuffers(1, &VBO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * ModelsValue[i].size(), &ModelsValue[i][0], GL_STREAM_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
        glEnableVertexAttribArray(0);
    }
   
}

GLvoid keyboard(unsigned char key, int x, int y) {
    switch (key)
    {
    case 'c': //초기화
        CameraPoint.ViewZ = 2.3f;
        CameraPoint.ViewX = 0.0f;
        CameraPoint.ViewY = 1.0f;
       
        break;
        //카메라 방향
    case 'x':
        CameraPoint.ViewX += 0.15f;
        break;
    case 'X':
        CameraPoint.ViewX -= 0.15f;
        break;
    case 'y':
        CameraPoint.ViewY += 0.15f;
        break;
    case 'Y':
        CameraPoint.ViewY -= 0.15f;
        break;
    case 'z':
        CameraPoint.ViewZ += 0.15f;
        break;
    case 'Z':
        CameraPoint.ViewZ -= 0.15f;
        break;
    case 'b':
        CrainTransSize.AllTransZ += 0.2f;
        break;
    case 'B':
        CrainTransSize.AllTransZ -= 0.2f;
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

   
    glutPostRedisplay();
    glutTimerFunc(60, timer, 1);
}
#include "readObj.h"
#include <vector>
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <Windows.h>
#include <fstream>
#include <random>
#include <string>
#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <time.h>

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

float ObjectTheta = 0.0f;
float CameraTheta = 0.0f;
int windowWidth, windowHeight;
int ObitalRotate = 0;

void drawmodels();
void drawObital();
void RandomColorLight();

bool CameraRotationA = false;
bool CameraRotationB = false;


float SquareRotate = 0.0f;
float LightRaidian = 0.0f;
bool SquareRotateA = false;
bool SquareRotateB = false;
bool LightRotateA = false;
bool LightRotateB = false;
bool ChangeModel = true;
int LightCount = 0;
struct CameraViewPoint {
    float ViewX = 0.0f;
    float ViewY = 3.0f;
    float ViewZ = 8.0f;
}CameraPoint;
struct LightObital {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

}LightRotateOrbital;
float ScaleZ = 5.0f;
float ScaleY = 0.0f;
GLuint VAO[4], VBO[8];
GLuint shaderID;
GLuint qobjshader;
GLuint vertexshader;
GLuint fragmentshader;
GLuint triangleshaderProgram;

vector<vector<glm::vec4>>ModelsValue;
vector<vector<glm::vec4>>normalValue;
vector<float>ObitalValue;
vector<float>LightOrbitalValue;

glm::vec3 LightColor = glm::vec3(1.0f, 1.0f, 1.0f);

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 600);
    glutCreateWindow("example3");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cerr << "Unable to initialize GLEW" << endl;
        exit(EXIT_FAILURE);
    }
    else {
        cout << "GLEW initialized" << endl;
    }
    vector<glm::vec4> modelTemp;
    vector<glm::vec4> normalTemp;
    ReadObj("Sphere2.obj", modelTemp, normalTemp);
    ModelsValue.push_back(modelTemp);
    normalValue.push_back(normalTemp);
    modelTemp.resize(0);
    normalTemp.resize(0);
    ReadObj("Sphere2.obj", modelTemp, normalTemp);
    ModelsValue.push_back(modelTemp);
    normalValue.push_back(normalTemp);
    modelTemp.resize(0);
    normalTemp.resize(0);
    ReadObj("Sphere2.obj", modelTemp, normalTemp);
    ModelsValue.push_back(modelTemp);
    normalValue.push_back(normalTemp);
    modelTemp.resize(0);
    normalTemp.resize(0);
    ReadObj("cube.obj", modelTemp, normalTemp);
    ModelsValue.push_back(modelTemp);
    normalValue.push_back(normalTemp);

    make_vertexShaders();
    make_fragmentShaders();
    make_shaderProgram();
    initbuffer();

    glutDisplayFunc(drawscene);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(1, timer, 1);
    glutReshapeFunc(resize);
    glutMainLoop();
}

void make_vertexShaders() {
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

    shaderID = glCreateProgram();

    glAttachShader(shaderID, qobjshader);
    glAttachShader(shaderID, fragmentshader);

    glLinkProgram(shaderID);

    glDeleteShader(qobjshader);
    glDeleteShader(fragmentshader);

    GLint result;
    GLchar errorlog[512];

    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderID, 512, NULL, errorlog);
        cerr << "Error: shader program 연결 실패" << errorlog << endl;
    }
    glUseProgram(shaderID);

    return shaderID;
}


void drawmodels() {
    //태양
	glBindVertexArray(VAO[0]);
	glm::mat4 Sunmodel = glm::mat4(1.0f);
    Sunmodel = glm::rotate(Sunmodel, glm::radians(SquareRotate), glm::vec3(0.0f, 1.0f, 0.0f));
    Sunmodel = glm::scale(Sunmodel, glm::vec3(1.0f, 1.0f, 1.0f));
	unsigned int SunmodelLocation = glGetUniformLocation(shaderID, "ModelTransform");
	glUniformMatrix4fv(SunmodelLocation, 1, GL_FALSE, glm::value_ptr(Sunmodel));
	glm::mat4 SunNormalmodel = glm::mat4(1.0f);
    SunNormalmodel = glm::rotate(SunNormalmodel, glm::radians(SquareRotate), glm::vec3(0.0f, 1.0f, 0.0f));
	unsigned int SunNormalmodelLocation = glGetUniformLocation(shaderID, "NormalTransform");
	glUniformMatrix4fv(SunNormalmodelLocation, 1, GL_FALSE, glm::value_ptr(SunNormalmodel));
	unsigned int SunobjColorLocation = glGetUniformLocation(shaderID, "ObjectColor");
	glUniform3f(SunobjColorLocation, 1.0f, 0.3f, 0.1f);
	glDrawArrays(GL_TRIANGLES, 0, ModelsValue[0].size());

	//지구
	glBindVertexArray(VAO[1]);
	glm::mat4 Earthmodel = glm::mat4(1.0f);
    Earthmodel = glm::rotate(Earthmodel, glm::radians(SquareRotate), glm::vec3(0.0f, 1.0f, 0.0f));
    Earthmodel = glm::translate(Earthmodel, glm::vec3(-1.75f, 0.0f, 0.0f));
    Earthmodel = glm::scale(Earthmodel, glm::vec3(0.5f, 0.5f, 0.5f));
	unsigned int EarthmodelLocation = glGetUniformLocation(shaderID, "ModelTransform");
	glUniformMatrix4fv(EarthmodelLocation, 1, GL_FALSE, glm::value_ptr(Earthmodel));
	glm::mat4 EarthNormalmodel = glm::mat4(1.0f);
    EarthNormalmodel = glm::rotate(EarthNormalmodel, glm::radians(SquareRotate), glm::vec3(0.0f, 1.0f, 0.0f));
	unsigned int EarthNormalmodelLocation = glGetUniformLocation(shaderID, "NormalTransform");
	glUniformMatrix4fv(EarthNormalmodelLocation, 1, GL_FALSE, glm::value_ptr(EarthNormalmodel));
	unsigned int EarthobjColorLocation = glGetUniformLocation(shaderID, "ObjectColor");
	glUniform3f(EarthobjColorLocation, 0.1f, 1.0f, 0.3f);
	glDrawArrays(GL_TRIANGLES, 0, ModelsValue[1].size());

    //달
    glBindVertexArray(VAO[2]);
    glm::mat4 Moonmodel = glm::mat4(1.0f);
    Moonmodel = glm::rotate(Moonmodel, glm::radians(SquareRotate), glm::vec3(0.0f, 1.0f, 0.0f));
    Moonmodel = glm::translate(Moonmodel, glm::vec3(-2.5f, 0.0f, 0.0f));
    Moonmodel = glm::scale(Moonmodel, glm::vec3(0.25f, 0.25f, 0.25f));
    unsigned int MoonmodelLocation = glGetUniformLocation(shaderID, "ModelTransform");
    glUniformMatrix4fv(MoonmodelLocation, 1, GL_FALSE, glm::value_ptr(Moonmodel));
    glm::mat4 MoonNormalmodel = glm::mat4(1.0f);
    MoonNormalmodel = glm::rotate(MoonNormalmodel, glm::radians(SquareRotate), glm::vec3(0.0f, 1.0f, 0.0f));
    unsigned int MoonNormalmodelLocation = glGetUniformLocation(shaderID, "NormalTransform");
    glUniformMatrix4fv(MoonNormalmodelLocation, 1, GL_FALSE, glm::value_ptr(MoonNormalmodel));
    unsigned int MoonobjColorLocation = glGetUniformLocation(shaderID, "ObjectColor");
    glUniform3f(MoonobjColorLocation, 0.3f, 0.1f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, ModelsValue[2].size());

	//조명 박스
    glBindVertexArray(VAO[3]);
    glm::mat4 LightBox = glm::mat4(1.0f);
    LightBox = glm::rotate(LightBox, glm::radians(LightRaidian), glm::vec3(0.0f, 1.0f, 0.0f));
    LightBox = glm::translate(LightBox, glm::vec3(0.0f, 0.0f, ScaleZ));
    LightBox = glm::scale(LightBox, glm::vec3(0.3f, 0.3f, 0.3f));
    unsigned int LightBoxLocation = glGetUniformLocation(shaderID, "ModelTransform");
    glUniformMatrix4fv(LightBoxLocation, 1, GL_FALSE, glm::value_ptr(LightBox));
    glm::mat4 LightBoxNormalmodel = glm::mat4(1.0f);
    LightBoxNormalmodel = glm::rotate(LightBoxNormalmodel, glm::radians(LightRaidian), glm::vec3(0.0f, 1.0f, 0.0f));
    unsigned int LightBoxNormalmodelLocation = glGetUniformLocation(shaderID, "NormalTransform");
    glUniformMatrix4fv(LightBoxNormalmodelLocation, 1, GL_FALSE, glm::value_ptr(LightBoxNormalmodel));
    unsigned int LightBoxColorLocation = glGetUniformLocation(shaderID, "ObjectColor");
    glUniform3f(LightBoxColorLocation, 0.2f, 0.2f, 0.2f);
    glDrawArrays(GL_TRIANGLES, 0, ModelsValue[3].size());

}

GLvoid drawscene() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glUseProgram(shaderID);

    glm::vec3 ModelCameraPos = glm::vec3(CameraPoint.ViewX, CameraPoint.ViewY, CameraPoint.ViewZ); // EYE
    glm::mat4 CameraPosDistance = glm::mat4(1.0f);
    CameraPosDistance = glm::rotate(CameraPosDistance, glm::radians(ObjectTheta), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec4 CameraPosDis = glm::vec4(ModelCameraPos, 1);
    CameraPosDis = CameraPosDistance * CameraPosDis;
    glm::vec3 CameraPostionDirection = glm::vec3(CameraPosDis.x, CameraPosDis.y, CameraPosDis.z);  //EYE 변환

    glm::vec3 ModelCameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
    glm::vec3 ModelCameraDirection = CameraPostionDirection - ModelCameraTarget;

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 CameraDirectionDistance = glm::mat4(1.0f);
    CameraDirectionDistance = glm::rotate(CameraDirectionDistance, glm::radians(CameraTheta), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec4 CameraDirectionDis = CameraDirectionDistance * glm::vec4(ModelCameraDirection, 1);
    glm::vec3 CameraDirection;
    CameraDirection.x = CameraDirectionDis.x;
    CameraDirection.y = CameraDirectionDis.y;
    CameraDirection.z = CameraDirectionDis.z;
    CameraDirection = glm::normalize(CameraDirection);

    glm::mat4 CenterDirectionDistance = glm::mat4(1.0f);
    CenterDirectionDistance = glm::rotate(CenterDirectionDistance, glm::radians(CameraTheta), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 CenterDirectionDis = glm::vec3(ModelCameraTarget - CameraPostionDirection);
    glm::vec4 CenterDirection = glm::vec4(CenterDirectionDis, 1);
    CenterDirection = CenterDirectionDistance * CenterDirection;
    CenterDirectionDis = glm::vec3(CenterDirection.x + CameraPostionDirection.x,
        CenterDirection.y + CameraPostionDirection.y, CenterDirection.z + CameraPostionDirection.z); //n

    glm::vec3 ModelCameraRight = glm::normalize(glm::cross(up, CameraDirection)); //v

    glm::vec3 cameraUp = glm::cross(CameraDirection, ModelCameraRight); //u

    glm::mat4 ModelView = glm::mat4(1.0f);
    ModelView = glm::lookAt(CameraPostionDirection, CenterDirectionDis, cameraUp);
    unsigned int ModelViewLocation = glGetUniformLocation(shaderID, "ViewTransform");
    glUniformMatrix4fv(ModelViewLocation, 1, GL_FALSE, glm::value_ptr(ModelView));

    //조명 카메라 위치
    glm::vec3 ViewP;
    ViewP = glm::vec3(CameraPostionDirection);
    unsigned int ViewPositionLocation = glGetUniformLocation(shaderID, "ViewPosTransform");
    glUniform3fv(ViewPositionLocation, 1, glm::value_ptr(ViewP));

    //원근
    {
        glm::mat4 ModelProj = glm::mat4(1.0f);
        ModelProj = glm::perspective(glm::radians(60.0f), (float)windowWidth / windowHeight, 0.1f, 100.0f);
        unsigned int ModelProjLocation = glGetUniformLocation(shaderID, "ProjectionTransform");
        glUniformMatrix4fv(ModelProjLocation, 1, GL_FALSE, &ModelProj[0][0]);
    }
    //조명
    {
        glm::mat4 LightPosition = glm::mat4(1.0f);
        LightPosition = glm::rotate(LightPosition, glm::radians(LightRaidian), glm::vec3(0.0f, 1.0f, 0.0f));
        LightPosition = glm::translate(LightPosition, glm::vec3(0.0f, ScaleY, ScaleZ - 0.5f));
        glm::vec4 LightPosDis = glm::vec4(1.0f);
        LightPosDis = LightPosition * LightPosDis;
        glm::vec3 LightPos
            = glm::vec3(LightPosDis.x, LightPosDis.y, LightPosDis.z);
        unsigned int lightPosLocation = glGetUniformLocation(shaderID, "LightPos");
        glUniform3fv(lightPosLocation, 1, glm::value_ptr(glm::vec3(0, 0, 0)));
        unsigned int lightColorLocation = glGetUniformLocation(shaderID, "LightColor");
        glUniform3fv(lightColorLocation, 1, glm::value_ptr(LightColor));
        unsigned int LightTransformLocation = glGetUniformLocation(shaderID, "LightTransform");
        glUniformMatrix4fv(LightTransformLocation, 1, GL_FALSE, glm::value_ptr(LightPosition));
    }
    drawmodels();

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
    glGenVertexArrays(4, VAO);

    for (int i = 0; i < 4; i++) {
        glBindVertexArray(VAO[i]);
        glGenBuffers(2, &VBO[2 * i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[2 * i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * ModelsValue[i].size(), &ModelsValue[i][0], GL_STREAM_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0); //--- 위치 속성
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[2 * i + 1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * normalValue[i].size(), &normalValue[i][0], GL_STREAM_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
        glEnableVertexAttribArray(1);
    }
}
void RandomColorLight() {
    if (LightCount % 3 == 1) {
        LightColor.x = 1.0f;
        LightColor.y = 0.0f;
        LightColor.z = 0.0f;
    }
    else if (LightCount % 3 == 2) {
        LightColor.x = 0.0f;
        LightColor.y = 1.0f;
        LightColor.z = 0.0f;
    }
    else if (LightCount % 3 == 0) {
        LightColor.x = 0.0f;
        LightColor.y = 0.0f;
        LightColor.z = 1.0f;
    }
}
GLvoid keyboard(unsigned char key, int x, int y) {
    switch (key)
    {
    case 'y':
        CameraRotationA = !CameraRotationA;
        CameraRotationB = false;
        break;
    case 'Y':
        CameraRotationB = !CameraRotationB;
        CameraRotationA = false;
        break;
    case 'z':
        ScaleZ += 0.2f;
        break;
    case 'Z':
        ScaleZ -= 0.2f;
        break;
    case 'm':
        ScaleY = 20.0f;
        LightColor.x = 0.2f;
        LightColor.y = 0.2f;
        LightColor.z = 0.2f;
        break;
    case 'M':
        ScaleY = 0.0f;
        LightColor.x = 1.0f;
        LightColor.y = 1.0f;
        LightColor.z = 1.0f;
        break;
    case 't':
        SquareRotateA = !SquareRotateA;
        SquareRotateB = false;
        break;
    case 'T':
        SquareRotateB = !SquareRotateB;
        SquareRotateA = false;
        break;
    case 'r':
        LightRotateA = !LightRotateA;
        LightRotateB = false;
        break;
    case 'R':
        LightRotateB = !LightRotateB;
        LightRotateA = false;
        break;
    case 'c':
        RandomColorLight();
        LightCount++;
        break;
    case 'C':
        LightColor.x = 1.0f;
        LightColor.y = 1.0f;
        LightColor.z = 1.0f;
        LightCount = 0;
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
    if (CameraRotationA) {
        ObjectTheta++;
    }
    if (CameraRotationB) {
        ObjectTheta--;
    }
    if (LightRotateA) {
        LightRaidian -= 1.0f;
    }
    if (LightRotateB) {
        LightRaidian += 1.0f;
    }
    if (SquareRotateA) {
        SquareRotate += 1.0f;
    }
    if (SquareRotateB) {
        SquareRotate -= 1.0f;
    }
    glutPostRedisplay();
    glutTimerFunc(1, timer, 1);
}
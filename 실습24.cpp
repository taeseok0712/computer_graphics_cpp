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
bool CameraRotationA = false;
bool CameraRotationB = false;
bool MakeOrbital = true;
float SquareRotate = 0.0f;
float LightRaidian = 0.0f;
bool SquareRotateA = false;
bool SquareRotateB = false;
bool LightRotateA = false;
bool LightRotateB = false;



struct CameraViewPoint {
    float ViewX = 0.0f;
    float ViewY = 3.0f;
    float ViewZ = 6.0f;
}CameraPoint;
struct LightObital {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

}LightRotateOrbital;
GLuint VAO[3], VBO[6];
GLuint shaderID;
GLuint qobjshader;
GLuint vertexshader;
GLuint fragmentshader;
GLuint triangleshaderProgram;

GLUquadricObj* CenterQobj[5];
vector<glm::vec4>ModelsValue[3];
vector<glm::vec4>normalValue[3];
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
    ReadObj("cube.obj", ModelsValue[0], normalValue[0]);
    ReadObj("cube.obj", ModelsValue[1], normalValue[1]);

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

    glBindVertexArray(VAO[0]);
    glm::mat4 model = glm::mat4(1.0f); 
    model = glm::rotate(model, glm::radians(SquareRotate), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    unsigned int modelLocation = glGetUniformLocation(shaderID, "ModelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model)); 
    glm::mat4 Normalmodel = glm::mat4(1.0f);
    Normalmodel = glm::rotate(Normalmodel,glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    unsigned int NormalmodelLocation = glGetUniformLocation(shaderID, "NormalTransform");
    glUniformMatrix4fv(NormalmodelLocation, 1, GL_FALSE, glm::value_ptr(Normalmodel));
   
    //glm::vec3 ModelColor = glm::vec3(0.6f, 0.5f, 0.3f);
    unsigned int objColorLocation = glGetUniformLocation(shaderID, "ObjectColor");
    glUniform3f(objColorLocation, 0.6f, 0.5f, 0.3f);   
    glDrawArrays(GL_TRIANGLES, 0, ModelsValue[0].size());
    
    //조명 박스
    glBindVertexArray(VAO[2]);
    glm::mat4 LightBox = glm::mat4(1.0f);

    LightBox = glm::rotate(LightBox,glm::radians(LightRaidian), glm::vec3(0.0f, 1.0f, 0.0f));
    LightBox = glm::translate(LightBox, glm::vec3(3.0f, 0.0f, 0.0f));
    LightBox = glm::scale(LightBox, glm::vec3(1.0f, 1.0f, 1.0f));
    unsigned int LightBoxLocation = glGetUniformLocation(shaderID, "ModelTransform");
    glUniformMatrix4fv(LightBoxLocation, 1, GL_FALSE, glm::value_ptr(LightBox));
	
    glm::mat4 LightBoxNormalmodel = glm::mat4(1.0f);
    LightBoxNormalmodel = glm::rotate(LightBoxNormalmodel, glm::radians(LightRaidian), glm::vec3(0.0f, 1.0f, 0.0f));
    LightBoxNormalmodel = glm::translate(LightBoxNormalmodel, glm::vec3(3.0f, 0.0f, 0.0f));
    unsigned int LightBoxNormalmodelLocation = glGetUniformLocation(shaderID, "NormalTransform");
	glUniformMatrix4fv(LightBoxNormalmodelLocation, 1, GL_FALSE, glm::value_ptr(LightBoxNormalmodel));
    unsigned int LightBoxColorLocation = glGetUniformLocation(shaderID, "ObjectColor");
    glUniform3f(LightBoxColorLocation, 0.2f, 0.2f, 0.2f);
    glDrawArrays(GL_TRIANGLES, 0, ModelsValue[1].size());


    glBindVertexArray(VAO[1]);
    glm::mat4 OrbitalModel = glm::mat4(1.0f);
    unsigned int OrbitalModelModelLocation = glGetUniformLocation(shaderID, "ModelTransform");
    glUniformMatrix4fv(OrbitalModelModelLocation, 1, GL_FALSE, glm::value_ptr(OrbitalModel));
    unsigned int OrbitalModelLocation = glGetUniformLocation(shaderID, "ObjectColor");
    glUniform3f(OrbitalModelLocation, 0.2f, 0.5f, 0.8f);
    //glDrawArrays(GL_TRIANGLES, 0, ModelsValue[0].size());
    glLineWidth(2.0f);
    glDrawArrays(GL_LINE_STRIP, 0, ObitalValue.size() / 6);
    

    

}
void drawObital() {
    float CircleX, CircleZ, CircleN;
    for (int i = 0; i < 360; i++) {
        float r = 3.0;
        CircleX = r * cos(3.14 * i / 180);
        CircleZ = r * sin(3.14 * i / 180);

        ObitalValue.push_back(CircleX);
        ObitalValue.push_back(0.0f);
        ObitalValue.push_back(CircleZ);

        ObitalValue.push_back(0.0f);
        ObitalValue.push_back(1.0f);
        ObitalValue.push_back(0.0f);
    }
    for (int i = 0; i < 360; i++) {
        float r = 1.5;
        CircleX = r * cos(3.14 * i / 180);
        CircleZ = r * sin(3.14 * i / 180);

        LightOrbitalValue.push_back(CircleX);
        LightOrbitalValue.push_back(0.0f);
        LightOrbitalValue.push_back(CircleZ);
       
        LightOrbitalValue.push_back(0.0f);
        LightOrbitalValue.push_back(1.0f);
        LightOrbitalValue.push_back(0.0f);
    }
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
        LightPosition = glm::rotate(LightPosition, glm::radians(LightRaidian - 45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        LightPosition = glm::translate(LightPosition,glm::vec3(1.5f , 0.0f, 0.0f));
        LightPosition = glm::scale(LightPosition, glm::vec3(1.0f, 1.0f, 1.0f));

        glm::vec4 LightPosDis = glm::vec4(1.0f);
        LightPosDis = LightPosition * LightPosDis;
        glm::vec3 LightPos 
            = glm::vec3(LightPosDis.x, LightPosDis.y, LightPosDis.z);
        unsigned int lightPosLocation = glGetUniformLocation(shaderID, "LightPos");
        glUniform3fv(lightPosLocation, 1, glm::value_ptr(LightPos));

        unsigned int lightColorLocation = glGetUniformLocation(shaderID, "LightColor");
        glUniform3fv(lightColorLocation, 1, glm::value_ptr(LightColor));

        glm::mat4 LightTransformMat = glm::mat4(1.0f);
        LightTransformMat = glm::rotate(LightTransformMat, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        unsigned int LightTransformLocation = glGetUniformLocation(shaderID, "LightTransform");
        glUniformMatrix4fv(LightTransformLocation, 1, GL_FALSE, glm::value_ptr(LightTransformMat));
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
    if (MakeOrbital) {
        drawObital();
        MakeOrbital = false;
    }
    glGenVertexArrays(3, VAO);
    
    glBindVertexArray(VAO[0]);
    glGenBuffers(2, &VBO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*ModelsValue[0].size(), &ModelsValue[0][0], GL_STREAM_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0); //--- 위치 속성
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * normalValue[0].size(), &normalValue[0][0], GL_STREAM_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[1]);
    glGenBuffers(2, &VBO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ObitalValue.size(), &ObitalValue[0], GL_STREAM_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //--- 위치 속성
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ObitalValue.size(), &ObitalValue[0], GL_STREAM_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[2]);
    glGenBuffers(2, &VBO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * ModelsValue[1].size(), &ModelsValue[1][0], GL_STREAM_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0); //--- 위치 속성
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * normalValue[1].size(), &normalValue[1][0], GL_STREAM_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
    glEnableVertexAttribArray(1);
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
    case'R':
        LightRotateB = !LightRotateB;
        LightRotateA = false;
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
        LightRotateOrbital.x = LightOrbitalValue[6 * ObitalRotate];
        LightRotateOrbital.y = LightOrbitalValue[6 * ObitalRotate + 1];
        LightRotateOrbital.z = LightOrbitalValue[6 * ObitalRotate + 2];




        ObitalRotate++;
        LightRaidian -= 1.0f;
        if (ObitalRotate == 360) {
            ObitalRotate = 0;
        }
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
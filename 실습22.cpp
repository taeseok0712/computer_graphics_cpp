#include "readObj.h"
#include <vector>
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <Windows.h>
#include <fstream>
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

clock_t TimeStart;
clock_t TimeEnd;

float CameraTheta = 0;
float ObjectTheta = 0;

bool OnJump = false;
bool CameraRotationA = false;
bool CameraRotationB = false;
bool OpenFrontWall = false;
bool CloseFrontWall = false;
bool MoveOnBack = false;
bool MoveOnFront = false;
bool MoveOnLeft = false;
bool MoveOnRight = false;
int ShakeCount = 0;
int jumpcount = 0;

float FrontWallAngle = 90.0f;

int ModelNumber = 30;
int windowWidth, windowHeight;

struct Saver {
    bool CamRotationA = false;
    bool CamRotationB = false;
    bool OpenWall = false;
    bool CloseWall = false;
    bool MoOnBack = false;
    bool MoOnFront = false;
    bool MoOnLeft = false;
    bool MoOnRight = false;
    float LLeg = 180.0f;
    float RLeg = 180.0f;
    float AllR = 0.0f;
    float LArm = 0.0f;
    float RArm = 0.0f;
    int count = 0;
    float AllX = 0.0f;
    float AllY = 0.0f;
    float AllZ = 0.0f;
    float open = 0.0f;
}BoolSaver;

struct CameraViewPoint {
    float ViewX = 0.0f;
    float ViewY = 1.0f;
    float ViewZ = 18.0f;
}CameraPoint;

struct RobotAngle {
    float LeftLeg = 180.0f;
    float RightLeg = 180.0f;
    float AllRotate = 0.0f;
    float LeftArm = 0.0f;
    float RightArm = 0.0f;
}RobotAngleSize;

struct RobotTrans {
    float RobotAllX = 0.0f;
    float RobotAllY = 0.0f;
    float RobotAllZ = 0.0f;
}RobotTransSize;

vector<glm::vec4>ModelsValue[13];

GLuint VAO[13], VBO[13];
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
    glutCreateWindow("example3");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cerr << "Unable to initialize GLEW" << endl;
        exit(EXIT_FAILURE);
    }
    else {
        cout << "GLEW initialized" << endl;
    }
    //obj 파일읽기
    for (int i = 0; i < 6; i++) {
        ReadObj("plane.obj", ModelsValue[i]); // 바닥
    }

    ReadObj("crainArm.obj", ModelsValue[6]); // 다리1
    ReadObj("crainArm.obj", ModelsValue[7]); //다리2
    ReadObj("crainCube.obj", ModelsValue[8]); //몸통
    ReadObj("crainArm.obj", ModelsValue[9]); // 팔1
    ReadObj("crainArm.obj", ModelsValue[10]); //팔2
    ReadObj("crainCube.obj", ModelsValue[11]); //얼굴
    ReadObj("crainCube.obj", ModelsValue[12]); //코

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



void drawmodels() {
    //바닥면
    glBindVertexArray(VAO[0]);
    glm::mat4 PlaneModel = glm::mat4(1.0f);
    PlaneModel = glm::translate(PlaneModel, glm::vec3(0.0f, -3.0f, 0.0f));
    PlaneModel = glm::scale(PlaneModel, glm::vec3(6.0f, 0.0f, 6.0f));
    unsigned int PlaneModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(PlaneModelLocation, 1, GL_FALSE, glm::value_ptr(PlaneModel));
    unsigned int PlaneModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(PlaneModelFragLocation, 1.0f, 0.2f, 0.6f);
    glDrawArrays(GL_QUADS, 0, ModelsValue[0].size());

    glDisable(GL_CULL_FACE);
    //오른쪽 벽
    glBindVertexArray(VAO[1]);
    glm::mat4 RightWall = glm::mat4(1.0f);
    RightWall = glm::translate(RightWall, glm::vec3(-3.0f, 0.0f, 0.0f));
    RightWall = glm::rotate(RightWall,glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    RightWall = glm::scale(RightWall, glm::vec3(6.0f, 0.0f, 6.0f));
    unsigned int RightWallLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(RightWallLocation, 1, GL_FALSE, glm::value_ptr(RightWall));
    unsigned int RightWallFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(RightWallFragLocation, 0.31f, 0.73f, 0.87f);
    glDrawArrays(GL_QUADS, 0, ModelsValue[1].size());
    
    //왼쪽 벽
    glBindVertexArray(VAO[2]);
    glm::mat4 LeftWall = glm::mat4(1.0f);
    LeftWall = glm::translate(LeftWall, glm::vec3(3.0f, 0.0f, 0.0f));
    LeftWall = glm::rotate(LeftWall,glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    LeftWall = glm::scale(LeftWall, glm::vec3(6.0f, 0.0f, 6.0f));
    unsigned int LeftWallLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(LeftWallLocation, 1, GL_FALSE, glm::value_ptr(LeftWall));
    unsigned int LeftWallFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(LeftWallFragLocation, 0.29f, 0.0f, 0.5f);
    glDrawArrays(GL_QUADS, 0, ModelsValue[2].size());

    //천장
    glBindVertexArray(VAO[3]);
    glm::mat4 RoofModel = glm::mat4(1.0f);
    RoofModel = glm::translate(RoofModel, glm::vec3(0.0f, 3.0f, 0.0f));
    RoofModel = glm::scale(RoofModel, glm::vec3(6.0f, 0.0f, 6.0f));
    unsigned int RoofModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(RoofModelLocation, 1, GL_FALSE, glm::value_ptr(RoofModel));
    unsigned int RoofModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(RoofModelFragLocation, 1.0f, 0.85f, 0.72f);
    glDrawArrays(GL_QUADS, 0, ModelsValue[3].size());

    //뒤쪽벽
    glBindVertexArray(VAO[4]);
    glm::mat4 BackWall = glm::mat4(1.0f);
    BackWall = glm::translate(BackWall, glm::vec3(0.0f, 0.0f, -3.0f));
    BackWall = glm::rotate(BackWall, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    BackWall = glm::scale(BackWall, glm::vec3(6.0f, 0.0f, 6.0f));
    unsigned int BackWallLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(BackWallLocation, 1, GL_FALSE, glm::value_ptr(BackWall));
    unsigned int BackWallFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(BackWallFragLocation, 0.93f, 0.5f, 0.93f);
    glDrawArrays(GL_QUADS, 0, ModelsValue[4].size());

    //앞면
    glBindVertexArray(VAO[5]);
    glm::mat4 FrontWall = glm::mat4(1.0f);
    FrontWall = glm::translate(FrontWall, glm::vec3(0.0f, -3.0f, 3.0f));
    FrontWall = glm::rotate(FrontWall, glm::radians(FrontWallAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    FrontWall = glm::translate(FrontWall, glm::vec3(0.0f, 0.0f, -3.0f));
    FrontWall = glm::scale(FrontWall, glm::vec3(6.0f, 0.0f, 6.0f));
    unsigned int FrontWallLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(FrontWallLocation, 1, GL_FALSE, glm::value_ptr(FrontWall));
    unsigned int FrontWallFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(FrontWallFragLocation, 0.9f, 0.9f, 0.98f);
    glDrawArrays(GL_QUADS, 0, ModelsValue[5].size());

    //로봇
    //왼쪽다리
    glBindVertexArray(VAO[6]);
    glm::mat4 LeftLeg = glm::mat4(1.0f);
    LeftLeg = glm::translate(LeftLeg, 
        glm::vec3(RobotTransSize.RobotAllX , RobotTransSize.RobotAllY, RobotTransSize.RobotAllZ));
    LeftLeg = glm::rotate(LeftLeg, glm::radians(RobotAngleSize.AllRotate), glm::vec3(0.0f, 1.0f, 0.0f));
    LeftLeg = glm::translate(LeftLeg, glm::vec3(-0.25f, -1.5f, 0.0f));
    LeftLeg = glm::rotate(LeftLeg, glm::radians(RobotAngleSize.LeftLeg), glm::vec3(1.0f, 0.0f, 0.0f));
    LeftLeg = glm::scale(LeftLeg, glm::vec3(0.5f, 1.0f, 1.0f));
    unsigned int LeftLegLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(LeftLegLocation, 1, GL_FALSE, glm::value_ptr(LeftLeg));
    unsigned int LeftLegFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(LeftLegFragLocation, 0.39f, 0.58f, 0.92f);
    glDrawArrays(GL_QUADS, 0, ModelsValue[6].size());

    //오른쪽다리
    glBindVertexArray(VAO[7]);
    glm::mat4 RightLeg = glm::mat4(1.0f);
    RightLeg = glm::translate(RightLeg,
        glm::vec3(RobotTransSize.RobotAllX, RobotTransSize.RobotAllY, RobotTransSize.RobotAllZ));
    RightLeg = glm::rotate(RightLeg, glm::radians(RobotAngleSize.AllRotate), glm::vec3(0.0f, 1.0f, 0.0f));
    RightLeg = glm::translate(RightLeg, glm::vec3(0.25f, -1.5f, 0.0f));
    RightLeg = glm::rotate(RightLeg, glm::radians(RobotAngleSize.RightLeg), glm::vec3(1.0f, 0.0f, 0.0f));
    RightLeg = glm::scale(RightLeg, glm::vec3(0.5f, 1.0f, 1.0f));
    unsigned int RightLegLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(RightLegLocation, 1, GL_FALSE, glm::value_ptr(RightLeg));
    unsigned int RightLegFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(RightLegFragLocation, 0.0f, 0.0f, 0.54f);
    glDrawArrays(GL_QUADS, 0, ModelsValue[7].size());

    //몸통
    glBindVertexArray(VAO[8]);
    glm::mat4 Body = glm::mat4(1.0f);
    Body = glm::translate(Body,
        glm::vec3(RobotTransSize.RobotAllX, RobotTransSize.RobotAllY, RobotTransSize.RobotAllZ));
    Body = glm::rotate(Body, glm::radians(RobotAngleSize.AllRotate), glm::vec3(0.0f, 1.0f, 0.0f));
    Body = glm::translate(Body, glm::vec3(0.0f, -2.0f, 3.0f));
    Body = glm::translate(Body, glm::vec3(0.0f, 0.0f, -3.0f));
    Body = glm::scale(Body, glm::vec3(0.75f, 6.0f, 0.75f));
    unsigned int BodyLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(BodyLocation, 1, GL_FALSE, glm::value_ptr(Body));
    unsigned int BodyFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(BodyFragLocation, 0.0f, 0.8f, 0.8f);
    glDrawArrays(GL_QUADS, 0, ModelsValue[8].size());

    //왼쪽팔
    glBindVertexArray(VAO[9]);
    glm::mat4 LeftArm = glm::mat4(1.0f);
    LeftArm = glm::translate(LeftArm,
        glm::vec3(RobotTransSize.RobotAllX, RobotTransSize.RobotAllY, RobotTransSize.RobotAllZ));
    LeftArm = glm::rotate(LeftArm, glm::radians(RobotAngleSize.AllRotate), glm::vec3(0.0f, 1.0f, 0.0f));
    LeftArm = glm::rotate(LeftArm, glm::radians(RobotAngleSize.LeftArm), glm::vec3(1.0f, 0.0f, 0.0f));
    LeftArm = glm::translate(LeftArm, glm::vec3(0.2f, -0.75f, 0.0f));
    LeftArm = glm::rotate(LeftArm, glm::radians(-150.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    LeftArm = glm::scale(LeftArm, glm::vec3(1.0f, 1.0f, 1.0f));
    unsigned int LeftArmLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(LeftArmLocation, 1, GL_FALSE, glm::value_ptr(LeftArm));
    unsigned int LeftArmFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(LeftArmFragLocation, 0.0f, 0.5f, 0.5f);
    glDrawArrays(GL_QUADS, 0, ModelsValue[9].size());

    //오른쪽팔
    glBindVertexArray(VAO[10]);
    glm::mat4 RightArm = glm::mat4(1.0f);
    RightArm = glm::translate(RightArm,
        glm::vec3(RobotTransSize.RobotAllX, RobotTransSize.RobotAllY, RobotTransSize.RobotAllZ));
    RightArm = glm::rotate(RightArm, glm::radians(RobotAngleSize.AllRotate), glm::vec3(0.0f, 1.0f, 0.0f));
    RightArm = glm::rotate(RightArm, glm::radians(RobotAngleSize.RightArm), glm::vec3(1.0f, 0.0f, 0.0f));
    RightArm = glm::translate(RightArm, glm::vec3(-0.2f, -0.75f, 0.0f));
    RightArm = glm::rotate(RightArm, glm::radians(150.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    RightArm = glm::scale(RightArm, glm::vec3(1.0f, 1.0f, 1.0f));
    unsigned int RightArmLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(RightArmLocation, 1, GL_FALSE, glm::value_ptr(RightArm));
    unsigned int RightArmFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(RightArmFragLocation, 0.49f, 1.0f, 0.83f);
    glDrawArrays(GL_QUADS, 0, ModelsValue[10].size());

    //얼굴
    glBindVertexArray(VAO[11]);
    glm::mat4 FrontFace = glm::mat4(1.0f);
    FrontFace = glm::translate(FrontFace,
        glm::vec3(RobotTransSize.RobotAllX, RobotTransSize.RobotAllY, RobotTransSize.RobotAllZ));
    FrontFace = glm::rotate(FrontFace, glm::radians(RobotAngleSize.AllRotate), glm::vec3(0.0f, 1.0f, 0.0f));
    FrontFace = glm::translate(FrontFace, glm::vec3(0.0f, -1.0f, 3.0f));
    FrontFace = glm::translate(FrontFace, glm::vec3(0.0f, 0.0f, -3.0f));
    FrontFace = glm::scale(FrontFace, glm::vec3(0.4f, 3.0f, 0.4f));
    unsigned int FrontFaceLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(FrontFaceLocation, 1, GL_FALSE, glm::value_ptr(FrontFace));
    unsigned int FrontFaceFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(FrontFaceFragLocation, 0.9f, 0.9f, 0.98f);
    glDrawArrays(GL_QUADS, 0, ModelsValue[11].size());

    //코
    glBindVertexArray(VAO[12]);
    glm::mat4 FrontNose = glm::mat4(1.0f);
    FrontNose = glm::translate(FrontNose,
        glm::vec3(RobotTransSize.RobotAllX, RobotTransSize.RobotAllY, RobotTransSize.RobotAllZ));
    FrontNose = glm::rotate(FrontNose, glm::radians(RobotAngleSize.AllRotate), glm::vec3(0.0f, 1.0f, 0.0f));
    FrontNose = glm::translate(FrontNose, glm::vec3(0.0f, -0.65f, 3.25f));
    FrontNose = glm::translate(FrontNose, glm::vec3(0.0f, 0.0f, -3.0f));
    FrontNose = glm::scale(FrontNose, glm::vec3(0.15f, 0.25f, 0.15f));
    unsigned int FrontNoseLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(FrontNoseLocation, 1, GL_FALSE, glm::value_ptr(FrontNose));
    unsigned int FrontNoseFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(FrontNoseFragLocation, 0.6f, 0.8f, 0.19f);
    glDrawArrays(GL_QUADS, 0, ModelsValue[12].size());
}
GLvoid drawscene() {
    glClearColor(0.0, 0.0, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    initbuffer();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glUseProgram(shaderID[1]);

    unsigned int ModelViewLocation = glGetUniformLocation(shaderID[1], "viewtransform");
    unsigned int ModelProjLocation = glGetUniformLocation(shaderID[1], "projectiontransform");

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
    glUniformMatrix4fv(ModelViewLocation, 1, GL_FALSE, glm::value_ptr(ModelView));

    //원근
    glm::mat4 ModelProj = glm::mat4(1.0f);
    ModelProj = glm::perspective(glm::radians(60.0f), (float)windowWidth / windowHeight, 0.1f, 100.0f);
    glUniformMatrix4fv(ModelProjLocation, 1, GL_FALSE, &ModelProj[0][0]);

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
    glGenVertexArrays(13, VAO);
    for (int i = 0; i < 13; i++) {
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
    case 'i': //초기화
        CameraPoint.ViewZ = 18.0f;
        CameraPoint.ViewX = 0.0f;
        CameraPoint.ViewY = 1.0f;

        BoolSaver.MoOnBack = MoveOnBack;
        BoolSaver.MoOnFront = MoveOnFront;
        BoolSaver.MoOnLeft = MoveOnLeft;
        BoolSaver.MoOnRight = MoveOnRight;
        
        BoolSaver.CamRotationA = CameraRotationA;
        BoolSaver.CamRotationB = CameraRotationB;
        BoolSaver.RLeg = RobotAngleSize.RightLeg;
        BoolSaver.LLeg = RobotAngleSize.LeftLeg;
        BoolSaver.AllR = RobotAngleSize.AllRotate;
        BoolSaver.RArm = RobotAngleSize.RightArm;
        BoolSaver.LArm = RobotAngleSize.LeftArm;

        BoolSaver.CloseWall = CloseFrontWall;
        BoolSaver.OpenWall = OpenFrontWall;

        BoolSaver.AllX = RobotTransSize.RobotAllX;
        BoolSaver.AllY = RobotTransSize.RobotAllY;
        BoolSaver.AllZ = RobotTransSize.RobotAllZ;
        BoolSaver.open = FrontWallAngle;
        BoolSaver.count = ShakeCount;
        
        memset(&RobotTransSize, 0.0f, sizeof(RobotTransSize));
        OnJump = false;
        FrontWallAngle = 90.0f;
        CloseFrontWall = false;
        OpenFrontWall = false;
        MoveOnBack = false;
        MoveOnFront = false;
        MoveOnLeft= false;
        MoveOnRight = false;
        ShakeCount = 0;
        RobotAngleSize.LeftLeg = 180.0f;
        RobotAngleSize.RightLeg = 180.0f;
        RobotAngleSize.AllRotate = 0.0f;
        RobotAngleSize.LeftArm = 0.0f;
        RobotAngleSize.RightArm = 0.0f;
        break;
    case 'I':
        MoveOnBack = BoolSaver.MoOnBack;
        MoveOnFront = BoolSaver.MoOnFront;
        MoveOnLeft = BoolSaver.MoOnLeft;
        MoveOnRight = BoolSaver.MoOnRight;

        BoolSaver.CamRotationA = CameraRotationA;
        BoolSaver.CamRotationB = CameraRotationB;
        RobotAngleSize.RightLeg = BoolSaver.RLeg;
        RobotAngleSize.LeftLeg = BoolSaver.LLeg;
        RobotAngleSize.AllRotate = BoolSaver.AllR;
        RobotAngleSize.RightArm = BoolSaver.RArm;
        RobotAngleSize.LeftArm = BoolSaver.LArm;

        CloseFrontWall = BoolSaver.CloseWall;
        OpenFrontWall = BoolSaver.OpenWall;

        RobotTransSize.RobotAllX = BoolSaver.AllX;
        RobotTransSize.RobotAllY = BoolSaver.AllY;
        RobotTransSize.RobotAllZ = BoolSaver.AllZ;
        FrontWallAngle = BoolSaver.open;
        ShakeCount = BoolSaver.count;

        OnJump = false;
        break;
        //카메라 방향
    case 'x':
        CameraPoint.ViewX += 0.15f;
        break;
    case 'X':
        CameraPoint.ViewX -= 0.15f;
        break;
    case 'y':
        CameraRotationA = !CameraRotationA;
        CameraRotationB = false;
        break;
    case 'Y':
        CameraRotationB = !CameraRotationB;
        CameraRotationA = false;
        break;
    
    case 'z':
        CameraPoint.ViewZ += 0.15f;
        break;
    case 'Z':
        CameraPoint.ViewZ -= 0.15f;
        break;

        //오브젝트 옮기기
    case 'w':
        TimeStart = clock();
        if (RobotTransSize.RobotAllZ <= -3.0f) {
            RobotTransSize.RobotAllZ = 2.8f;
        }
        else {
            RobotTransSize.RobotAllZ -= 0.2f;
        }
        RobotAngleSize.AllRotate = 180.0f;
        MoveOnFront = true;
        MoveOnBack = false;
        MoveOnLeft = false;
        MoveOnRight = false;
        break;
    case 's':
        TimeStart = clock();
        if (RobotTransSize.RobotAllZ >= 3.0f) {
            RobotTransSize.RobotAllZ = -2.8f;
        }
        else {
            RobotTransSize.RobotAllZ += 0.2f;
        }
        RobotAngleSize.AllRotate = 0.0f;
        MoveOnFront = false;
        MoveOnBack = true;
        MoveOnLeft = false;
        MoveOnRight = false;
        break;
    case 'a':
        TimeStart = clock();
        if (RobotTransSize.RobotAllX <= -3.0f) {
            RobotTransSize.RobotAllX = 2.8f;
        }
        else {
            RobotTransSize.RobotAllX -= 0.2f;
        }
        RobotAngleSize.AllRotate = 270.0f;
        MoveOnFront = false;
        MoveOnBack = false;
        MoveOnLeft = true;
        MoveOnRight = false;
        break;
    case 'd':
        TimeStart = clock();
        if (RobotTransSize.RobotAllX >= 3.0f) {
            RobotTransSize.RobotAllX = -2.8f;
        }
        else {
            RobotTransSize.RobotAllX += 0.2f;
        }
        RobotAngleSize.AllRotate = 90.0f;
        MoveOnFront = false;
        MoveOnBack = false;
        MoveOnLeft = false;
        MoveOnRight = true;
        break;
    case 'o':
        OpenFrontWall = !OpenFrontWall;
        CloseFrontWall = false;
        break;
    case 'O':
        CloseFrontWall = !CloseFrontWall;
        OpenFrontWall = false;
        break;
    case 'j':
        if (OnJump == false) {
            OnJump = true;
        }
        else {
            break;
        }
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
    if (OpenFrontWall) {
        if (FrontWallAngle < 180.0f) {
            FrontWallAngle += 2.0f;
        }
        else {
            OpenFrontWall = false;
        }
    }
    if (CloseFrontWall) {
        if (90.0f < FrontWallAngle) {
            FrontWallAngle -= 2.0f;
        }
        else {
            CloseFrontWall = false;
        }
    }
    if (MoveOnBack) {
        TimeEnd = clock();
        if (((TimeEnd - TimeStart)/CLK_TCK) < 3) {
            if (ShakeCount == 0) {
                RobotAngleSize.LeftLeg += 2.5f;
                RobotAngleSize.RightLeg -= 2.5f;
                RobotAngleSize.LeftArm += 2.5f;
                RobotAngleSize.RightArm -= 2.5f;
                if (RobotAngleSize.LeftLeg >= 195.0f) {
                    ShakeCount++;
                }
            }
            else {
                RobotAngleSize.LeftLeg -= 2.5f;
                RobotAngleSize.RightLeg += 2.5f;
                RobotAngleSize.LeftArm -= 2.5f;
                RobotAngleSize.RightArm += 2.5f;
                if (RobotAngleSize.RightLeg >= 195.0f) {
                    ShakeCount--;
                }
            }
        }
        else {
            MoveOnBack = false;
            RobotAngleSize.LeftLeg = 180.0f;
            RobotAngleSize.RightLeg = 180.0f;
            RobotAngleSize.LeftArm = 0.0f;
            RobotAngleSize.RightArm = 0.0f;
        }

    }
    if (MoveOnFront) {
        TimeEnd = clock();
        if (((TimeEnd - TimeStart) / CLK_TCK) < 3) {
            if (ShakeCount == 0) {
                RobotAngleSize.LeftLeg += 2.5f;
                RobotAngleSize.RightLeg -= 2.5f;
                RobotAngleSize.LeftArm += 2.5f;
                RobotAngleSize.RightArm -= 2.5f;
                if (RobotAngleSize.LeftLeg >= 195.0f) {
                    ShakeCount++;
                }
            }
            else {
                RobotAngleSize.LeftLeg -= 2.5f;
                RobotAngleSize.RightLeg += 2.5f;
                RobotAngleSize.LeftArm -= 2.5f;
                RobotAngleSize.RightArm += 2.5f;
                if (RobotAngleSize.RightLeg >= 195.0f) {
                    ShakeCount--;
                }
            }
        }
        else {
            MoveOnFront = false;
            RobotAngleSize.LeftLeg = 180.0f;
            RobotAngleSize.RightLeg = 180.0f;
            RobotAngleSize.LeftArm = 0.0f;
            RobotAngleSize.RightArm = 0.0f;
        }

    }
    if (MoveOnRight) {
        TimeEnd = clock();
        if (((TimeEnd - TimeStart) / CLK_TCK) < 3) {
            if (ShakeCount == 0) {
                RobotAngleSize.LeftLeg += 2.5f;
                RobotAngleSize.RightLeg -= 2.5f;
                RobotAngleSize.LeftArm += 2.5f;
                RobotAngleSize.RightArm -= 2.5f;
                if (RobotAngleSize.LeftLeg >= 195.0f) {
                    ShakeCount++;
                }
            }
            else {
                RobotAngleSize.LeftLeg -= 2.5f;
                RobotAngleSize.RightLeg += 2.5f;
                RobotAngleSize.LeftArm -= 2.5f;
                RobotAngleSize.RightArm += 2.5f;
                if (RobotAngleSize.RightLeg >= 195.0f) {
                    ShakeCount--;
                }
            }
        }
        else {
            MoveOnFront = false;
            RobotAngleSize.LeftLeg = 180.0f;
            RobotAngleSize.RightLeg = 180.0f;
            RobotAngleSize.LeftArm = 0.0f;
            RobotAngleSize.RightArm = 0.0f;
        }

    }
    if (MoveOnLeft) {
        TimeEnd = clock();
        if (((TimeEnd - TimeStart) / CLK_TCK) < 3) {
            if (ShakeCount == 0) {
                RobotAngleSize.LeftLeg += 2.5f;
                RobotAngleSize.RightLeg -= 2.5f;
                RobotAngleSize.LeftArm += 2.5f;
                RobotAngleSize.RightArm -= 2.5f;
                if (RobotAngleSize.LeftLeg >= 195.0f) {
                    ShakeCount++;
                }
            }
            else {
                RobotAngleSize.LeftLeg -= 2.5f;
                RobotAngleSize.RightLeg += 2.5f;
                RobotAngleSize.LeftArm -= 2.5f;
                RobotAngleSize.RightArm += 2.5f;
                if (RobotAngleSize.RightLeg >= 195.0f) {
                    ShakeCount--;
                }
            }
        }
        else {
            MoveOnFront = false;
            RobotAngleSize.LeftLeg = 180.0f;
            RobotAngleSize.RightLeg = 180.0f;
            RobotAngleSize.LeftArm = 0.0f;
            RobotAngleSize.RightArm = 0.0f;
        }

    }
    if (CameraRotationA) {
        ObjectTheta++;
    }
    if (CameraRotationB) {
        ObjectTheta--;
    }
    if (OnJump) {
        if (jumpcount == 0) {
            RobotTransSize.RobotAllY += 0.02f;
            if (RobotTransSize.RobotAllY >= 0.5) {
                jumpcount++;
            }
        }
        else {
            RobotTransSize.RobotAllY -= 0.02f;
            if (RobotTransSize.RobotAllY <= 0.0f) {
                RobotTransSize.RobotAllY = 0.0f;
                jumpcount = 0;
                OnJump = false;
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(60, timer, 1);
}
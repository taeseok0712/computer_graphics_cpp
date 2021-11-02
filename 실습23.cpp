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
void mousemove(int x, int y);
void mouse(int button, int state, int x, int y);
void drawmodels();

void PrimalCoordinate();

clock_t TimeStart;
clock_t TimeEnd;

float CameraTheta = 0;
float ObjectTheta = 0;

int windowWidth, windowHeight;
bool CameraRotationA = false;
bool CameraRotationB = false;
bool AllRotateA = false;
bool AllRotateB = false;
bool onesave = true;

int TotalSphere = 0;
int MouseCount = 0;
float MouseX = 0.0f;
float MouseY = 0.0f;
float PrimeMouseX = 0.0f;
float PrimeMouseY = 0.0f;
float AllObjectTheta = 0.0f;
int CollideCount[5];

struct CameraViewPoint {
    float ViewX = 0.0f;
    float ViewY = 1.0f;
    float ViewZ = 18.0f;
}CameraPoint;
struct SpeherTrans {
    float Tx = 0.0f;
    float Ty = 0.0f;
    float Tz = 0.0f;
};
struct SpeherCol {
    float SR = 0.0f;
    float SG = 0.0f;
    float SB = 0.0f;
};

vector<glm::vec4>ModelsValue[8];

GLuint VAO[8], VBO[8];
GLuint shaderID[2];
GLuint qobjshader;
GLuint vertexshader;
GLuint fragmentshader;
GLuint triangleshaderProgram;
SpeherCol SpeherColor[5];
SpeherTrans SpeherTransSize[5];
GLUquadricObj* CenterQobj[5];

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
    for (int i = 0; i < 5; i++) {
        ReadObj("plane.obj", ModelsValue[i]); // 바닥
    }
    for (int j = 5; j < 8; j++) {
        ReadObj("crainCube.obj", ModelsValue[j]); //육면체
    }

    make_vertexShaders();
    make_fragmentShaders();
    make_shaderProgram();
    initbuffer();

    glutDisplayFunc(drawscene);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mousemove);
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
void PrimalCoordinate() {
    for (int i = 0; i < 5; i++) {
        random_device rd;
        default_random_engine dre(rd());
        uniform_real_distribution<>spehery(0.0, 2.5);
        uniform_real_distribution<>speherxz(-1.5, 2.0);
        uniform_real_distribution<>SColor(0.0, 1.0);

        float PrimeY = spehery(dre);
        float PrimeX = speherxz(dre);
        float PrimeZ = speherxz(dre);
        float SpeherR = SColor(dre);
        float SpeherG = SColor(dre);
        float SpeherB = SColor(dre);

        SpeherTransSize[i].Ty = PrimeY;
        SpeherTransSize[i].Tx = PrimeX;
        SpeherTransSize[i].Tz = PrimeZ;

        SpeherColor[i].SR = SpeherR;
        SpeherColor[i].SG = SpeherG;
        SpeherColor[i].SB = SpeherB;
    }
}


void drawmodels() {
    if (onesave) {
        PrimalCoordinate();
        onesave = false;
    }
    //바닥면
    glEnable(GL_CULL_FACE);
    glBindVertexArray(VAO[0]);
    glm::mat4 PlaneModel = glm::mat4(1.0f);
    PlaneModel = glm::rotate(PlaneModel,glm::radians(AllObjectTheta), glm::vec3(0.0f, 0.0f, 1.0f));
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
    RightWall = glm::rotate(RightWall, glm::radians(AllObjectTheta), glm::vec3(0.0f, 0.0f, 1.0f));
    RightWall = glm::translate(RightWall, glm::vec3(-3.0f, 0.0f, 0.0f));
    RightWall = glm::rotate(RightWall, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    RightWall = glm::scale(RightWall, glm::vec3(6.0f, 0.0f, 6.0f));
    unsigned int RightWallLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(RightWallLocation, 1, GL_FALSE, glm::value_ptr(RightWall));
    unsigned int RightWallFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(RightWallFragLocation, 0.31f, 0.73f, 0.87f);
    glDrawArrays(GL_QUADS, 0, ModelsValue[1].size());

    //왼쪽 벽
    glBindVertexArray(VAO[2]);
    glm::mat4 LeftWall = glm::mat4(1.0f);
    LeftWall = glm::rotate(LeftWall, glm::radians(AllObjectTheta), glm::vec3(0.0f, 0.0f, 1.0f));
    LeftWall = glm::translate(LeftWall, glm::vec3(3.0f, 0.0f, 0.0f));
    LeftWall = glm::rotate(LeftWall, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    LeftWall = glm::scale(LeftWall, glm::vec3(6.0f, 0.0f, 6.0f));
    unsigned int LeftWallLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(LeftWallLocation, 1, GL_FALSE, glm::value_ptr(LeftWall));
    unsigned int LeftWallFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(LeftWallFragLocation, 0.29f, 0.0f, 0.5f);
    glDrawArrays(GL_QUADS, 0, ModelsValue[2].size());

    //천장
    glBindVertexArray(VAO[3]);
    glm::mat4 RoofModel = glm::mat4(1.0f);
    RoofModel = glm::rotate(RoofModel, glm::radians(AllObjectTheta), glm::vec3(0.0f, 0.0f, 1.0f));
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
    BackWall = glm::rotate(BackWall, glm::radians(AllObjectTheta), glm::vec3(0.0f, 0.0f, 1.0f));
    BackWall = glm::translate(BackWall, glm::vec3(0.0f, 0.0f, -3.0f));
    BackWall = glm::rotate(BackWall, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    BackWall = glm::scale(BackWall, glm::vec3(6.0f, 0.0f, 6.0f));
    unsigned int BackWallLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(BackWallLocation, 1, GL_FALSE, glm::value_ptr(BackWall));
    unsigned int BackWallFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(BackWallFragLocation, 0.93f, 0.5f, 0.93f);
    glDrawArrays(GL_QUADS, 0, ModelsValue[4].size()); 

    //제일 아래 사각형
    glBindVertexArray(VAO[5]);
    glm::mat4 Boundary = glm::mat4(1.0f);
    Boundary = glm::rotate(Boundary, glm::radians(AllObjectTheta), glm::vec3(0.0f, 0.0f, 1.0f));
    Boundary = glm::translate(Boundary, glm::vec3(0.0f, -2.9f, -1.95f));
    //Boundary = glm::rotate(Boundary, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    Boundary = glm::scale(Boundary, glm::vec3(2.0f, 3.0f, 2.0f));
    unsigned int BoundaryLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(BoundaryLocation, 1, GL_FALSE, glm::value_ptr(Boundary));
    unsigned int BoundaryFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(BoundaryFragLocation, 1.0f, 0.0f, 0.0f);
    glDrawArrays(GL_QUADS, 0, ModelsValue[5].size());

    //중간 사각형
    glBindVertexArray(VAO[6]);
    glm::mat4 Semi = glm::mat4(1.0f);
    Semi = glm::rotate(Semi, glm::radians(AllObjectTheta), glm::vec3(0.0f, 0.0f, 1.0f));
    Semi = glm::translate(Semi, glm::vec3(0.0f, -2.9f, -0.95f));
    //Semi = glm::rotate(Semi, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    Semi = glm::scale(Semi, glm::vec3(1.5f, 2.5f, 1.5f));
    unsigned int SemiLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(SemiLocation, 1, GL_FALSE, glm::value_ptr(Semi));
    unsigned int SemiFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(SemiFragLocation, 0.0f, 1.0f, 0.0f);
    glDrawArrays(GL_QUADS, 0, ModelsValue[6].size());

    //제일 위 사각형
    glBindVertexArray(VAO[7]);
    glm::mat4 Upper = glm::mat4(1.0f);
    Upper = glm::rotate(Upper, glm::radians(AllObjectTheta), glm::vec3(0.0f, 0.0f, 1.0f));
    Upper = glm::translate(Upper, glm::vec3(0.0f, -2.9f, 0.05f));
    //Upper = glm::rotate(Upper, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    Upper = glm::scale(Upper, glm::vec3(1.0f, 2.0f, 1.0f));
    unsigned int UpperLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(UpperLocation, 1, GL_FALSE, glm::value_ptr(Upper));
    unsigned int UpperFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(UpperFragLocation, 0.0f, 0.0f, 1.0f);
    glDrawArrays(GL_QUADS, 0, ModelsValue[7].size());

    if (TotalSphere != 0) {
        for (int i = 0; i < TotalSphere; i++) {
            CenterQobj[i] = gluNewQuadric();
            gluQuadricDrawStyle(CenterQobj[i], GLU_FILL);
            glm::mat4 CenterSphereModel = glm::mat4(1.0f);
            CenterSphereModel = glm::rotate(CenterSphereModel, glm::radians(AllObjectTheta), glm::vec3(0.0f, 0.0f, 1.0f));
            CenterSphereModel = glm::translate(CenterSphereModel, 
                glm::vec3(SpeherTransSize[i].Tx, SpeherTransSize[i].Ty, SpeherTransSize[i].Tz));
            CenterSphereModel = glm::scale(CenterSphereModel, glm::vec3(0.5f, 0.5f, 0.5f));
            unsigned int CenterSphereModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
            glUniformMatrix4fv(CenterSphereModelLocation, 1, GL_FALSE, glm::value_ptr(CenterSphereModel));
            unsigned int CenterSphereModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
            glUniform3f(CenterSphereModelFragLocation,
                SpeherColor[i].SR, SpeherColor[i].SG, SpeherColor[i].SB);
            gluSphere(CenterQobj[i], 0.25, 20, 20);
        }
    }
}
GLvoid drawscene() {
    glClearColor(0.0, 0.0, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    initbuffer();

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

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
    glGenVertexArrays(8, VAO);
    for (int i = 0; i < 8; i++) {
        glBindVertexArray(VAO[i]);
        glGenBuffers(1, &VBO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * ModelsValue[i].size(), &ModelsValue[i][0], GL_STREAM_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
        glEnableVertexAttribArray(0);
    }
}
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (MouseCount == 0) {
            PrimeMouseX = ((float)(x - (windowWidth / 2)) / (windowWidth / 2));
            PrimeMouseY = ((float)((windowHeight / 2) - y) / (windowHeight / 2));
            MouseCount = 1;
        }
    }
    else {
        MouseCount = 0;
    }
        glutPostRedisplay();
}
void mousemove(int x, int y) {
    MouseX = ((float)(x - (windowWidth / 2)) / (windowWidth / 2));
    MouseY = ((float)((windowHeight / 2) - y) / (windowHeight / 2));
    if ((PrimeMouseX < MouseX)) {
        if (PrimeMouseX < 0.0f) {
            if ((PrimeMouseY > MouseY)) {
                AllObjectTheta += 16.0f * glm::radians(atan2f(PrimeMouseY - MouseY, MouseX - PrimeMouseX));
            }
            else if ((PrimeMouseY < MouseY)) {
                AllObjectTheta -= 16.0f * glm::radians(atan2f(MouseY - PrimeMouseY, MouseX - PrimeMouseX));
            }
        }
        else if(PrimeMouseX > 0.0f){
            if ((PrimeMouseY > MouseY)) {
                AllObjectTheta -= 16.0f * glm::radians(atan2f(PrimeMouseY - MouseY, MouseX - PrimeMouseX));
            }
            else if ((PrimeMouseY < MouseY)) {
                AllObjectTheta += 16.0f * glm::radians(atan2f(MouseY - PrimeMouseY, MouseX - PrimeMouseX));
            }
        }
        
    }
    else if ((PrimeMouseX > MouseX)) {
        if (PrimeMouseX < 0.0f) {
            if ((PrimeMouseY > MouseY)) {
                AllObjectTheta += 16.0f * glm::radians(atan2f(PrimeMouseY - MouseY, PrimeMouseX - MouseX));
            }
            else if ((PrimeMouseY < MouseY)) {
                AllObjectTheta -= 16.0f * glm::radians(atan2f(MouseY - PrimeMouseY, PrimeMouseX - MouseX));
            }
        }
        else if (PrimeMouseX > 0.0f) {
            if ((PrimeMouseY > MouseY)) {
                AllObjectTheta -= 16.0f * glm::radians(atan2f(PrimeMouseY - MouseY, PrimeMouseX - MouseX));
            }
            else if ((PrimeMouseY < MouseY)) {
                AllObjectTheta += 16.0f * glm::radians(atan2f(MouseY - PrimeMouseY, PrimeMouseX - MouseX));
            }
        }
    }

    glutPostRedisplay();
}
GLvoid keyboard(unsigned char key, int x, int y) {
    switch (key)
    {
    case 'i': //초기화
       
        break;
    case 'I':
        
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
    case 'b':
        if (TotalSphere >= 5) {
            break;
        }
        else {
            TotalSphere++;
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
    if (CameraRotationA) {
        ObjectTheta++;
    }
    if (CameraRotationB) {
        ObjectTheta--;
    }
    if (TotalSphere != 0) {
        for (int i = 0; i < TotalSphere; i++) {
            if (CollideCount[i] == 0) {
                SpeherTransSize[i].Ty -= 0.2f;
                if (SpeherTransSize[i].Ty <= -3.0f) {
                    CollideCount[i]++;
                }
            }
            if (CollideCount[i] == 1) {
                SpeherTransSize[i].Ty += 0.2f;
                if (SpeherTransSize[i].Ty >= 3.0f) {
                    CollideCount[i]--;
                }
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(1, timer, 1);
}
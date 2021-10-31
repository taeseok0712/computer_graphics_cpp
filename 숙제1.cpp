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
#define COL 25
#define ROW 25

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

void randomScale(int i, int j);
void makeModelColor(int i, int j);
void drawmodels();

float CameraTheta = 0;
float ObjectThetaX = -20.0f;
float ObjectThetaY = 0.0f;
bool OnceMakeColor = true;
bool MoutainScaleOn = false;

int windowWidth, windowHeight;
int ArmRotateVector = 1;

int ModelCountCol = 0;
int ModelCountRow = 0;

float ModelColorR[COL][ROW];
float ModelColorG[COL][ROW];
float ModelColorB[COL][ROW];

float MoutainScale[COL][ROW];
int ScaleCount[COL][ROW];

bool ViewTransX = false;
bool ViewTransY = false;

struct Modelcolor {
    float modelR = 0.0f;
    float modelB = 0.0f;
    float modelG = 0.0f;
}ModelColorSize;

struct CameraViewPoint {
    float ViewX = 0.0f;
    float ViewY = 0.0f;
    float ViewZ = 6.0f;
}CameraPoint;

struct CrainTrans {
    float AllTransX = 0.0f;
    float AllTransY = 0.0f;
    float AllTransZ = 0.0f;
}CrainTransSize;

vector<glm::vec4>ModelsValue[2];

GLuint VAO[2], VBO[2];
GLuint shaderID[2];
GLuint qobjshader;
GLuint vertexshader;
GLuint fragmentshader;
GLuint triangleshaderProgram;

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    cout << "Write the size of the square you want to make." << endl;
    cin >> ModelCountCol;
    cin >> ModelCountRow;
    if (ModelCountCol < 5 || ModelCountCol>25 || ModelCountRow < 5 || ModelCountRow>25) {
        cin >> ModelCountCol;
        cin >> ModelCountRow;
    }
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(400, 300);
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
   

    ReadObj("plane.obj", ModelsValue[0]);
    ReadObj("crainCube.obj", ModelsValue[1]);

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

    glDisable(GL_CULL_FACE);
    glBindVertexArray(VAO[0]);
    glm::mat4 PlaneModel = glm::mat4(1.0f);
    PlaneModel = glm::translate(PlaneModel, glm::vec3(0.0f, -0.02f, 0.0f));
    PlaneModel = glm::scale(PlaneModel, glm::vec3(ModelCountCol*0.5f, 1.0f, ModelCountRow * 0.5f));
    unsigned int PlaneModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(PlaneModelLocation, 1, GL_FALSE, glm::value_ptr(PlaneModel));
    unsigned int PlaneModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(PlaneModelFragLocation, 0.5f, 0.75f, 0.27f);
    glDrawArrays(GL_QUADS, 0, ModelsValue[0].size());
    
    //아래-기본 y 스케일, 색깔 부여
    if (OnceMakeColor) {
        for (int i = 0; i < ModelCountCol; i++) {
            for (int j = 0; j < ModelCountRow; j++) {
                makeModelColor(i, j);
                randomScale(i, j);
                ScaleCount[i][j] = 0;
            }
        }
    }
    OnceMakeColor = false;

    //시작점 구함
    float PrimeX = ModelsValue[0][0].x * PlaneModel[0].x;
    float PrimeZ = ModelsValue[0][55].z * PlaneModel[2].z;
    //glEnable(GL_CULL_FACE);
    for (int i = 0; i < ModelCountCol; i++) {
        for (int j = 0; j < ModelCountRow; j++) {            
            glBindVertexArray(VAO[1]);
            glm::mat4 RailModel = glm::mat4(1.0f);
            RailModel = glm::translate(RailModel, glm::vec3(PrimeX + 0.5 * i + 0.25f, 0.0f, PrimeZ + 0.5 * j + 0.25f));
            RailModel = glm::scale(RailModel, glm::vec3(0.5f, MoutainScale[i][j], 0.5f));        
            unsigned int RailModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
            glUniformMatrix4fv(RailModelLocation, 1, GL_FALSE, glm::value_ptr(RailModel));
            unsigned int RailModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
            glUniform3f(RailModelFragLocation, ModelColorR[i][j], ModelColorG[i][j], ModelColorB[i][j]);
            glDrawArrays(GL_QUADS, 0, 24);
        }
    }
}
void makeModelColor(int i, int j) {
    random_device rd;
    default_random_engine dre(rd());
    uniform_real_distribution<>uid(0.0, 1.0);
    float mR = uid(dre);
    float mG = uid(dre);
    float mB = uid(dre);
    ModelColorR[i][j] = mR;
    ModelColorG[i][j] = mG;
    ModelColorB[i][j] = mB;
}
void randomScale(int i,int j) {
    random_device rd;
    default_random_engine dre(rd());
    uniform_real_distribution<>uid(0.5, 3.5);
    float SY = uid(dre);
    MoutainScale[i][j] = SY;
}

GLvoid drawscene() {
    glClearColor(0.0, 0.0, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    initbuffer();

    glEnable(GL_DEPTH_TEST);

    glUseProgram(shaderID[1]);

    //메인뷰
    glViewport(0, 0, windowWidth, windowHeight);

    unsigned int ModelViewLocation = glGetUniformLocation(shaderID[1], "viewtransform");
    unsigned int ModelProjLocation = glGetUniformLocation(shaderID[1], "projectiontransform");

    glm::vec3 ModelCameraPos = glm::vec3(CameraPoint.ViewX, CameraPoint.ViewY, CameraPoint.ViewZ); // EYE
    glm::mat4 CameraPosDistance = glm::mat4(1.0f);
    CameraPosDistance = glm::rotate(CameraPosDistance, glm::radians(ObjectThetaX), glm::vec3(1.0f, 0.0f, 0.0f));
    CameraPosDistance = glm::rotate(CameraPosDistance, glm::radians(ObjectThetaY), glm::vec3(0.0f, 1.0f, 0.0f));
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
    
    glm::vec3  ModelCameraRight; //v
    if ((90 <= (abs((int)ObjectThetaX)) % 360) && ((abs((int)ObjectThetaX)) % 360 <= 270)) {
        ModelCameraRight = glm::normalize(glm::cross(CameraDirection, up)); //v
    }
    else {
        ModelCameraRight = glm::normalize(glm::cross(up, CameraDirection));
    }



    glm::vec3 cameraUp = glm::cross(CameraDirection, ModelCameraRight); //u
    glm::mat4 ModelView = glm::mat4(1.0f);
    ModelView = glm::lookAt(CameraPostionDirection, CenterDirectionDis, cameraUp);
    glUniformMatrix4fv(ModelViewLocation, 1, GL_FALSE, glm::value_ptr(ModelView));


    //원근
    glm::mat4 ModelProj = glm::mat4(1.0f);
    ModelProj = glm::perspective(glm::radians(60.0f), (float)windowWidth / windowHeight, 0.1f, 100.0f);
    glUniformMatrix4fv(ModelProjLocation, 1, GL_FALSE, &ModelProj[0][0]);

    drawmodels();

    //탑뷰
    glViewport(windowWidth * 3 / 4, windowHeight * 3 / 4, windowWidth / 4, windowHeight / 4);

    unsigned int TopViewLocation = glGetUniformLocation(shaderID[1], "viewtransform");

    glm::vec3 TopCameraPos = glm::vec3(0.0f, 4.0f, 0.0f); // EYE

    glm::vec3 TopCameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
    glm::vec3 TopCameraDirection = TopCameraTarget - TopCameraPos;

    glm::vec3 TopViewUp = glm::vec3(0.0f, 0.0f, -1.0f);

    glm::mat4 TopView = glm::mat4(1.0f);
    TopView = glm::lookAt(TopCameraPos, TopCameraDirection, TopViewUp);
    glUniformMatrix4fv(TopViewLocation, 1, GL_FALSE, glm::value_ptr(TopView));

    glm::mat4 TopOrtho = glm::mat4(1.0f);
    TopOrtho = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.7f, 20.0f);
    unsigned int TopOrthoLocation = glGetUniformLocation(shaderID[1], "projectiontransform");
    glUniformMatrix4fv(TopOrthoLocation, 1, GL_FALSE, glm::value_ptr(TopOrtho)); //직각투영

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

    glGenVertexArrays(2, VAO);

    for (int i = 0; i < 2; i++) {
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
        MoutainScaleOn = false;
        OnceMakeColor = true;
        ObjectThetaX = -20.0f;
        ObjectThetaY = 0.0f;
        break;
        //카메라 방향
    case 'x':
        ViewTransX = !ViewTransX;
        break;
    case 'X':
        CameraPoint.ViewX -= 0.15f;
        break;
    case 'y':
        ViewTransY = !ViewTransY;
        ObjectThetaX = 0.0f;
        break;
    case 'Y':
        
        break;
    case 'z':
        CameraPoint.ViewZ += 0.15f;
        break;
    case 'Z':
        CameraPoint.ViewZ -= 0.15f;
        break;
    case 't':
        MoutainScaleOn = !MoutainScaleOn;
        break;
    case 'k':
        
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
    if (MoutainScaleOn) {
        for (int i = 0; i < COL; i++) {
            for (int j = 0; j < ROW; j++) {
                if (MoutainScale[i][j] >= 8.0) {
                    ScaleCount[i][j] ++;
                }
                else if (MoutainScale[i][j] <= 0.3) {
                    ScaleCount[i][j]--;
                }

                if (ScaleCount[i][j] == 0) {
                    MoutainScale[i][j] += 0.2f;
                }
                else {
                    MoutainScale[i][j] -= 0.2f;
                }
            }
        }
    }
    if (ViewTransX) {
        ObjectThetaX += 1.0f;
    }
    if (ViewTransY) {
        ObjectThetaY += 1.0f;
    }


    glutPostRedisplay();
    glutTimerFunc(1, timer, 1);
}
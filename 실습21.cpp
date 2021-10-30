#include "readObj.h"
#include <vector>
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

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

void randomExistSquare(int i);
void drawmodels();

float CameraTheta = 0;
float ObjectTheta = 0;

bool CrainBodyRotateA = false;
bool CrainBodyRotateB = false;
bool CrainArmRotateAB = false;
bool CameraRevolutionA = false;
bool CameraRevolutionB = false;
bool CameraRotationA = false;
bool CameraRotationB = false;
bool randomExist = true;

int ModelNumber = 30;
int windowWidth, windowHeight;
int ArmRotateVector = 1;
struct boolsave {
    bool crainBodyRA = false;
    bool crainBodyRB = false;
    bool crainArmRAB = false;

    bool cameraReA = false;
    bool cameraReB = false;
    bool cameraRoA = false;
    bool cameraRoB = false;
}boolsaver;

struct CameraViewPoint {
    float ViewX = 0.0f;
    float ViewY = 1.0f;
    float ViewZ = 8.0f;
}CameraPoint;
struct CrainRotate {
    float bodyRotateX = 0.0f;
    float bodyRotateY = 0.0f;
    float bodyRotateZ = 0.0f;

    float armLRotateX = 0.0f;
    float armLRotateY = 0.0f;
    float armLRotateZ = 0.0f;

    float armRRotateX = 0.0f;
    float armRRotateY = 0.0f;
    float armRRotateZ = 0.0f;

    float cameraAngle = 0.0f;
}CrainRotateSize;

struct CrainTrans {
    float AllTransX = 0.0f;
    float AllTransY = 0.0f;
    float AllTransZ = 0.0f;
}CrainTransSize;

vector<glm::vec4>ModelsValue[33];

GLuint VAO[33], VBO[33];
GLuint shaderID[2];
GLuint qobjshader;
GLuint vertexshader;
GLuint fragmentshader;
GLuint triangleshaderProgram;

float ModelTransX[30];
float ModelTransZ[30];
int colide[30];

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
    
    ReadObj("plane.obj", ModelsValue[0]);
    ReadObj("crainCube.obj", ModelsValue[1]);
    ReadObj("crainArm.obj", ModelsValue[2]);
    
    

    if (randomExist) {
		for (int ModelCountNumber = 0; ModelCountNumber < ModelNumber; ModelCountNumber++) {
			ReadObj("crainCube.obj", ModelsValue[ModelCountNumber + 3]);
		}
    }
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

void randomExistSquare(int i) {
    random_device rd;
    default_random_engine dre(rd());
    uniform_real_distribution<> urd(-2.5, 2.5);
    
    float randomX = urd(dre);
    float randomZ = urd(dre);

    ModelTransX[i] = randomX;
    ModelTransZ[i] = randomZ;
}

void drawmodels() {
    //바닥면
    glBindVertexArray(VAO[0]);
    glm::mat4 PlaneModel = glm::mat4(1.0f);
    PlaneModel = glm::scale(PlaneModel, glm::vec3(6.0f, 0, 6.0f));
    unsigned int PlaneModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(PlaneModelLocation, 1, GL_FALSE, glm::value_ptr(PlaneModel));
    unsigned int PlaneModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(PlaneModelFragLocation, 0.5f, 0.75f, 0.27f);
    glDrawArrays(GL_QUADS, 0, ModelsValue[0].size());

    //지우개 몸통
    glBindVertexArray(VAO[1]);
    glm::mat4 BodyModel = glm::mat4(1.0f);
    BodyModel = glm::translate(BodyModel, glm::vec3(CrainTransSize.AllTransX, CrainTransSize.AllTransY, CrainTransSize.AllTransZ));
    BodyModel = glm::scale(BodyModel, glm::vec3(0.5f,0.5f,0.5f));
    
    unsigned int BodyModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(BodyModelLocation, 1, GL_FALSE, glm::value_ptr(BodyModel));
    unsigned int BodyModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(BodyModelFragLocation, 1.0f, 0.2f, 0.6f);
    glDrawArrays(GL_QUADS, 0, 24);

    float BodybottomMinx = BodyModel[0].x * ModelsValue[1][0].x + BodyModel[3].x;
    float BodybottomMaxx = BodyModel[0].x * ModelsValue[1][2].x + BodyModel[3].x;
    float BodybottomMinz = BodyModel[2].z * ModelsValue[1][0].z + BodyModel[3].z;
    float BodybottomMaxz = BodyModel[2].z * ModelsValue[1][2].z + BodyModel[3].z;
    
    
    //탱크 왼쪽 팔 부분
    glBindVertexArray(VAO[2]);
    glm::mat4 LeftArmModel = glm::mat4(1.0f);
    LeftArmModel = glm::translate(LeftArmModel, glm::vec3(CrainTransSize.AllTransX, CrainTransSize.AllTransY, CrainTransSize.AllTransZ));
    LeftArmModel = glm::translate(LeftArmModel, glm::vec3(0.0f, -0.25f, 0.0f));
    LeftArmModel = glm::scale(LeftArmModel, glm::vec3(0.5f, 0.5f, 0.5f));
    unsigned int LeftArmModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(LeftArmModelLocation, 1, GL_FALSE, glm::value_ptr(LeftArmModel));
    unsigned int LeftArmModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(LeftArmModelFragLocation, 0.75f, 0.75f, 0.75f);
    glDrawArrays(GL_QUADS, 0, 24);
    
    //지워질 아이들
    if (randomExist) {
        for (int randomtrans = 0; randomtrans < ModelNumber; randomtrans++) {
            randomExistSquare(randomtrans);
        }
    }
    randomExist = false;
    //충돌처리 곧 다감

    for (int ModelCountNumber = 0; ModelCountNumber < ModelNumber; ModelCountNumber++) {        
        
        glBindVertexArray(VAO[ModelCountNumber + 3]);
        glm::mat4 ExistSqure = glm::mat4(1.0f);
        ExistSqure = glm::translate(ExistSqure, glm::vec3(ModelTransX[ModelCountNumber], 0.0f, ModelTransZ[ModelCountNumber]));
        ExistSqure = glm::scale(ExistSqure, glm::vec3(0.25f, 0.25f, 0.25f));
        float SquareCenterX = ExistSqure[0].x + ExistSqure[3].x;
        float SquareCenterZ = ExistSqure[2].z + ExistSqure[3].z;
        //충돌 이따가 다시 
        if ((BodybottomMinx <= SquareCenterX <= BodybottomMaxx) && (BodybottomMinz <= SquareCenterZ <= BodybottomMaxz)) {
            swap(ModelsValue[ModelCountNumber + 3], ModelsValue[ModelNumber + 3]);
            ModelNumber--;
        }
        
            unsigned int ExistSqureLocation = glGetUniformLocation(shaderID[1], "modeltransform");
            glUniformMatrix4fv(ExistSqureLocation, 1, GL_FALSE, glm::value_ptr(ExistSqure));
            unsigned int ExistSqureFragLocation = glGetUniformLocation(shaderID[1], "vColor");
            glUniform3f(ExistSqureFragLocation, 0.1f, 0.5f, 0.2f);

            glDrawArrays(GL_QUADS, 0, 24);
        
    }
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

    glGenVertexArrays(ModelNumber+3, VAO);

    for (int i = 0; i < 3; i++) {
        glBindVertexArray(VAO[i]);
        glGenBuffers(1, &VBO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * ModelsValue[i].size(), &ModelsValue[i][0], GL_STREAM_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
        glEnableVertexAttribArray(0);
    }
    if (ModelNumber != 0) {
        for (int j = 0; j < ModelNumber; j++) {
            glBindVertexArray(VAO[j+3]);
            glGenBuffers(1, &VBO[j+3]);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[j+3]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * ModelsValue[j+3].size(), &ModelsValue[j+3][0], GL_STREAM_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
            glEnableVertexAttribArray(0);
        }
    }
}

GLvoid keyboard(unsigned char key, int x, int y) {
    switch (key)
    {
    case 'c': //초기화
        CameraPoint.ViewZ = 2.0f;
        CameraPoint.ViewX = 0.0f;
        CameraPoint.ViewY = 1.0f;
        memset(&CrainRotateSize, 0, sizeof(CrainRotateSize));
        memset(&CrainTransSize, 0, sizeof(CrainTransSize));
        ObjectTheta = 0;
        CameraTheta = 0;

        CrainBodyRotateA = false;
        CrainBodyRotateB = false;
        CrainArmRotateAB = false;
        CameraRotationA = false;
        CameraRotationB = false;
        CameraRevolutionB = false;
        CameraRevolutionA = false;

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
        CrainTransSize.AllTransZ -= 0.1f;
        break;
    case 's':
        CrainTransSize.AllTransZ += 0.1f;
        break;
    case 'a':
        CrainTransSize.AllTransX -= 0.1f;
        break;
    case 'd':
        CrainTransSize.AllTransX += 0.1f;
        break;



    case 'm':
        CrainBodyRotateA = !CrainBodyRotateA;
        CrainBodyRotateB = false;
        break;
    case 'M':
        CrainBodyRotateB = !CrainBodyRotateB;
        CrainBodyRotateA = false;
        break;
    case 't':
        CrainArmRotateAB = !CrainArmRotateAB;
        break;
    case 'h':
        boolsaver.crainBodyRA = CrainBodyRotateA;
        boolsaver.crainBodyRB = CrainBodyRotateB;
        boolsaver.crainArmRAB = CrainArmRotateAB;
        boolsaver.cameraReA = CameraRevolutionA;
        boolsaver.cameraReB = CameraRevolutionB;
        boolsaver.cameraRoA = CameraRotationA;
        boolsaver.cameraRoB = CameraRotationB;

        CrainBodyRotateA = false;
        CrainBodyRotateB = false;
        CrainArmRotateAB = false;
        CameraRevolutionA = false;
        CameraRevolutionB = false;
        CameraRotationA = false;
        CameraRotationB = false;
        break;
    case 'S':
        CrainBodyRotateA = boolsaver.crainBodyRA;
        CrainBodyRotateB = boolsaver.crainBodyRB;
        CrainArmRotateAB = boolsaver.crainArmRAB;
        CameraRevolutionA = boolsaver.cameraReA;
        CameraRevolutionB = boolsaver.cameraReB;
        CameraRotationA = boolsaver.cameraRoA;
        CameraRotationB = boolsaver.cameraRoB;

        break;
    case 'r':
        CameraRevolutionA = !CameraRevolutionA;
        CameraRevolutionB = false;
        break;
    case 'R':
        CameraRevolutionB = !CameraRevolutionB;
        CameraRevolutionA = false;
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
    if (CrainBodyRotateA) {
        CrainRotateSize.bodyRotateY++;
    }
    if (CrainBodyRotateB) {
        CrainRotateSize.bodyRotateY--;
    }
    if (CrainArmRotateAB) {
        if (CrainRotateSize.armLRotateX >= 90) {
            ArmRotateVector = -1;
        }
        else if (CrainRotateSize.armLRotateX <= -90) {
            ArmRotateVector = 1;
        }

        if (ArmRotateVector == 1) {
            CrainRotateSize.armLRotateX++;
            CrainRotateSize.armRRotateX--;
        }
        else if (ArmRotateVector == -1) {
            CrainRotateSize.armLRotateX--;
            CrainRotateSize.armRRotateX++;
        }
    }
    if (CameraRevolutionA) {
        CameraTheta++;
    }
    if (CameraRevolutionB) {
        CameraTheta--;
    }
    if (CameraRotationA) {
        ObjectTheta++;
    }
    if (CameraRotationB) {
        ObjectTheta--;
    }
    


    glutPostRedisplay();
    glutTimerFunc(1, timer, 1);
}
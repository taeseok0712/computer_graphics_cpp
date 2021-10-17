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
void MakeCircleLine();
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

bool PolygonWireMode = true;
bool OneSave = true;
bool ViewX = false;
bool ViewZ = false;
bool MiniRotate = false;
bool SubRotate = false;

int windowWidth, windowHeight;
int RotateCountx = 0;
int RotateCountX = 0;
int RotateCounty = 0;
int RotateCountY = 0;

int CenterRotatey = 0;
int CenterRotateY = 0;

float CenterAnglex = 30.0f;
float CenterAngley = -24.0f;
float CenterAnglez = 0.0f;
float AllTransX = 0.0f;
float AllTransY = 0.0f;
float AllTransZ = 0.0f;

int SubSphere1Rotate = 0;
int SubSphere2Rotate = 0;
int SubSphere3Rotate = 0;
int MiniSphere1Rotate = 0;
int MiniSphere2Rotate = 0;
int MiniSphere3Rotate = 0;

struct CameraViewPoint {
    float ViewX = 0.0f;
    float ViewY = 0.0f;
    float ViewZ = 2.3f;
}CameraPoint;

struct SubAngle {
    float Sub1X = 0.0f;
    float Sub1Y = 0.0f;
    float Sub1Z = 0.0f;

    float Sub2X = 0.0f;
    float Sub2Y = 0.0f;
    float Sub2Z = 0.0f;

    float Sub3X = 0.0f;
    float Sub3Y = 0.0f;
    float Sub3Z = 0.0f;
}SubAngleSize;

struct ObjectTrans {
    float AllTransX = 0.0f;
    float AllTransY = 0.0f;
    float AllTransZ = 0.0f;

}ObjectTransSize;

struct SubTrans {
    float Sub1X = 0.8f;
    float Sub1Y = 0.0f;
    float Sub1Z = 0.0f;

    float Sub2X = 0.8f;
    float Sub2Y = -0.8f;
    float Sub2Z = 0.0f;

    float Sub3X = 0.8f;
    float Sub3Y = 0.8f;
    float Sub3Z = 0.0f;

}SubTransSize;

struct MiniAngle {
    float Mini1X = 0.0f;
    float Mini1Y = 0.0f;
    float Mini1Z = 0.0f;

    float Mini2X = 0.0f;
    float Mini2Y = 0.0f;
    float Mini2Z = 0.0f;

    float Mini3X = 0.0f;
    float Mini3Y = 0.0f;
    float Mini3Z = 0.0f;

}MiniAngleSize;

struct MiniTrans {
    float Mini1X = 0.0f;
    float Mini1Y = 0.0f;
    float Mini1Z = 0.0f;

    float Mini2X = 0.0f;
    float Mini2Y = 0.0f;
    float Mini2Z = 0.0f;

    float Mini3X = 0.0f;
    float Mini3Y = 0.0f;
    float Mini3Z = 0.0f;
}MiniTransSize;

vector<float>SubSphere1;
vector<float>SubSphere2;
vector<float>SubSphere3;
vector<float>MiniSphere1;
vector<float>MiniSphere2;
vector<float>MiniSphere3;

float make_line[] = {
    0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,

    -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,

    0.0f, 0.0f, 1.0f, 0.5f, 0.0f, 0.5f,
    0.0f, 0.0f, -1.0f, 0.5f, 0.0f, 0.5f
};

GLUquadricObj* CenterQobj;
GLUquadricObj* SubQobj1;
GLUquadricObj* SubQobj2;
GLUquadricObj* SubQobj3;
GLUquadricObj* MiniQobj1;
GLUquadricObj* MiniQobj2;
GLUquadricObj* MiniQobj3;


GLuint VAO[6], VBO[12];
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

    if (PolygonWireMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glUseProgram(shaderID[0]);
    
    unsigned int ViewModelLocation = glGetUniformLocation(shaderID[0], "viewtransform");
    unsigned int ProjLocation = glGetUniformLocation(shaderID[0], "projectiontransform");

    glm::vec3 CameraPos = glm::vec3(CameraPoint.ViewX, CameraPoint.ViewY, CameraPoint.ViewZ); //--- 카메라 위치
    glm::vec3 CameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
    glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    
    glm::mat4 ScreenProj = glm::mat4(1.0f);
    ScreenProj = glm::perspective(glm::radians(60.0f), (float)windowWidth / windowHeight, 0.1f, 100.0f);
    glUniformMatrix4fv(ProjLocation, 1, GL_FALSE, &ScreenProj[0][0]); //투영
   
    glm::mat4 ScreenView = glm::mat4(1.0f);
    ScreenView = glm::lookAt(CameraPos, CameraDirection, CameraUp);
    glUniformMatrix4fv(ViewModelLocation, 1, GL_FALSE, glm::value_ptr(ScreenView)); //뷰

    glm::mat4 SubOrbital1Model = glm::mat4(1.0f);
    SubOrbital1Model = glm::rotate(SubOrbital1Model, glm::radians(CenterAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
    SubOrbital1Model = glm::rotate(SubOrbital1Model, glm::radians(CenterAngley), glm::vec3(0.0f, 1.0f, 0.0f));
    SubOrbital1Model = glm::translate(SubOrbital1Model, glm::vec3(ObjectTransSize.AllTransX, ObjectTransSize.AllTransY, ObjectTransSize.AllTransZ));
    unsigned int SubOrbital1ModelLocation = glGetUniformLocation(shaderID[0], "modeltransform");
    glUniformMatrix4fv(SubOrbital1ModelLocation, 1, GL_FALSE, glm::value_ptr(SubOrbital1Model));
   
    glBindVertexArray(VAO[0]);
    glLineWidth(2.0f);
    glDrawArrays(GL_LINE_STRIP, 0, SubSphere1.size() / 6);

    glm::mat4 SubOrbital2Model = glm::mat4(1.0f);
    SubOrbital2Model = glm::rotate(SubOrbital2Model, glm::radians(CenterAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
    SubOrbital2Model = glm::rotate(SubOrbital2Model, glm::radians(CenterAngley), glm::vec3(0.0f, 1.0f, 0.0f));
    SubOrbital2Model = glm::translate(SubOrbital2Model, glm::vec3(ObjectTransSize.AllTransX, ObjectTransSize.AllTransY, ObjectTransSize.AllTransZ));
    unsigned int SubOrbital2ModelLocation = glGetUniformLocation(shaderID[0], "modeltransform");    
    glUniformMatrix4fv(SubOrbital2ModelLocation, 1, GL_FALSE, glm::value_ptr(SubOrbital2Model));
    glBindVertexArray(VAO[1]);
    glLineWidth(2.0f);
    glDrawArrays(GL_LINE_STRIP, 0, SubSphere2.size() / 6);

    glm::mat4 SubOrbital3Model = glm::mat4(1.0f);
    SubOrbital3Model = glm::rotate(SubOrbital3Model, glm::radians(CenterAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
    SubOrbital3Model = glm::rotate(SubOrbital3Model, glm::radians(CenterAngley), glm::vec3(0.0f, 1.0f, 0.0f));
    SubOrbital3Model = glm::translate(SubOrbital3Model, glm::vec3(ObjectTransSize.AllTransX, ObjectTransSize.AllTransY, ObjectTransSize.AllTransZ));
    unsigned int SubOrbital3ModelLocation = glGetUniformLocation(shaderID[0], "modeltransform");
    glUniformMatrix4fv(SubOrbital3ModelLocation, 1, GL_FALSE, glm::value_ptr(SubOrbital3Model));
    glBindVertexArray(VAO[2]);
    glLineWidth(2.0f);
    glDrawArrays(GL_LINE_STRIP, 0, SubSphere3.size() / 6);

    glm::mat4 MiniOrbital1Model = glm::mat4(1.0f);
    MiniOrbital1Model = glm::rotate(MiniOrbital1Model, glm::radians(CenterAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
    MiniOrbital1Model = glm::rotate(MiniOrbital1Model, glm::radians(CenterAngley), glm::vec3(0.0f, 1.0f, 0.0f));
    MiniOrbital1Model = glm::translate(MiniOrbital1Model, glm::vec3(ObjectTransSize.AllTransX, ObjectTransSize.AllTransY, ObjectTransSize.AllTransZ));
    MiniOrbital1Model = glm::translate(MiniOrbital1Model, glm::vec3(SubTransSize.Sub1X, SubTransSize.Sub1Y, SubTransSize.Sub1Z));  
    unsigned int MIniOrbitalModel1Location = glGetUniformLocation(shaderID[0], "modeltransform");
    glUniformMatrix4fv(MIniOrbitalModel1Location, 1, GL_FALSE, glm::value_ptr(MiniOrbital1Model));
    glBindVertexArray(VAO[3]);
    glLineWidth(1.5f);
    glDrawArrays(GL_LINE_STRIP, 0, MiniSphere1.size() / 6);

    glm::mat4 MiniOrbital2Model = glm::mat4(1.0f);
    MiniOrbital2Model = glm::rotate(MiniOrbital2Model, glm::radians(CenterAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
    MiniOrbital2Model = glm::rotate(MiniOrbital2Model, glm::radians(CenterAngley), glm::vec3(0.0f, 1.0f, 0.0f));
    MiniOrbital2Model = glm::translate(MiniOrbital2Model, glm::vec3(ObjectTransSize.AllTransX, ObjectTransSize.AllTransY, ObjectTransSize.AllTransZ));
    MiniOrbital2Model = glm::translate(MiniOrbital2Model, glm::vec3(SubTransSize.Sub2X, SubTransSize.Sub2Y, SubTransSize.Sub2Z));
    unsigned int MIniOrbitalModel2Location = glGetUniformLocation(shaderID[0], "modeltransform");
    glUniformMatrix4fv(MIniOrbitalModel2Location, 1, GL_FALSE, glm::value_ptr(MiniOrbital2Model));
    glBindVertexArray(VAO[4]);
    glLineWidth(1.5f);
    glDrawArrays(GL_LINE_STRIP, 0, MiniSphere2.size() / 6);

    glm::mat4 MiniOrbital3Model = glm::mat4(1.0f);
    MiniOrbital3Model = glm::rotate(MiniOrbital3Model, glm::radians(CenterAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
    MiniOrbital3Model = glm::rotate(MiniOrbital3Model, glm::radians(CenterAngley), glm::vec3(0.0f, 1.0f, 0.0f));
    MiniOrbital3Model = glm::translate(MiniOrbital3Model, glm::vec3(ObjectTransSize.AllTransX, ObjectTransSize.AllTransY, ObjectTransSize.AllTransZ));
    MiniOrbital3Model = glm::translate(MiniOrbital3Model, glm::vec3(SubTransSize.Sub3X, SubTransSize.Sub3Y, SubTransSize.Sub3Z));
    unsigned int MIniOrbitalModel3Location = glGetUniformLocation(shaderID[0], "modeltransform");
    glUniformMatrix4fv(MIniOrbitalModel3Location, 1, GL_FALSE, glm::value_ptr(MiniOrbital3Model));
    glBindVertexArray(VAO[5]);
    glLineWidth(1.5f);
    glDrawArrays(GL_LINE_STRIP, 0, MiniSphere3.size() / 6);






    glUseProgram(shaderID[1]);

    CenterQobj = gluNewQuadric();
    gluQuadricDrawStyle(CenterQobj, GLU_FILL);

    unsigned int SphereViewLocation = glGetUniformLocation(shaderID[1], "viewtransform");
    unsigned int SphereProjLocation = glGetUniformLocation(shaderID[1], "projectiontransform");

    glm::vec3 SphereCameraPos = glm::vec3(CameraPoint.ViewX, CameraPoint.ViewY, CameraPoint.ViewZ); //--- 카메라 위치
    glm::vec3 SphereCameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
    glm::vec3 SphereCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 SphereProj = glm::mat4(1.0f);
    SphereProj = glm::perspective(glm::radians(60.0f), (float)windowWidth / windowHeight, 0.1f, 100.0f);
    glUniformMatrix4fv(SphereProjLocation, 1, GL_FALSE, &SphereProj[0][0]);

    glm::mat4 SphereView = glm::mat4(1.0f);
    SphereView = glm::lookAt(SphereCameraPos, SphereCameraDirection, SphereCameraUp);
    glUniformMatrix4fv(SphereViewLocation, 1, GL_FALSE, glm::value_ptr(SphereView));

    //중앙 - 해
    glm::mat4 CenterSphereModel = glm::mat4(1.0f);
    CenterSphereModel = glm::rotate(CenterSphereModel, glm::radians(CenterAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
    CenterSphereModel = glm::rotate(CenterSphereModel, glm::radians(CenterAngley), glm::vec3(0.0f, 1.0f, 0.0f));
    CenterSphereModel = glm::rotate(CenterSphereModel, glm::radians(CenterAnglez), glm::vec3(0.0f, 0.0f, 1.0f));
    CenterSphereModel = glm::translate(CenterSphereModel, glm::vec3(ObjectTransSize.AllTransX, ObjectTransSize.AllTransY, ObjectTransSize.AllTransZ));
    CenterSphereModel = glm::scale(CenterSphereModel, glm::vec3(0.7f, 0.7f, 0.7f));
    unsigned int CenterSphereModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(CenterSphereModelLocation, 1, GL_FALSE, glm::value_ptr(CenterSphereModel));
    unsigned int CenterSphereModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(CenterSphereModelFragLocation, 1.0f, 1.0f, 0.5f);
    gluSphere(CenterQobj, 0.25, 20, 20);
     

    //중간
    SubQobj1 = gluNewQuadric();
    gluQuadricDrawStyle(SubQobj1, GLU_FILL);
    glm::mat4 Sub1SphereModel = glm::mat4(1.0f);
    Sub1SphereModel = glm::rotate(Sub1SphereModel, glm::radians(CenterAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
    Sub1SphereModel = glm::rotate(Sub1SphereModel, glm::radians(CenterAngley), glm::vec3(0.0f, 1.0f, 0.0f));
    Sub1SphereModel = glm::translate(Sub1SphereModel, glm::vec3(ObjectTransSize.AllTransX, ObjectTransSize.AllTransY, ObjectTransSize.AllTransZ));
    Sub1SphereModel = glm::translate(Sub1SphereModel, glm::vec3(SubTransSize.Sub1X, SubTransSize.Sub1Y, SubTransSize.Sub1Z));
    Sub1SphereModel = glm::rotate(Sub1SphereModel, glm::radians(SubAngleSize.Sub1X), glm::vec3(1.0f, 0.0f, 0.0f));
    Sub1SphereModel = glm::rotate(Sub1SphereModel, glm::radians(SubAngleSize.Sub1Y), glm::vec3(0.0f, 1.0f, 0.0f));
    Sub1SphereModel = glm::scale(Sub1SphereModel, glm::vec3(0.3f, 0.3f, 0.3f));
    unsigned int Sub1SphereModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(Sub1SphereModelLocation, 1, GL_FALSE, glm::value_ptr(Sub1SphereModel));
    unsigned int Sub1SphereModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(Sub1SphereModelFragLocation, 1.0f, 0.2f, 0.6f);
    gluSphere(SubQobj1, 0.25, 20, 20);

    SubQobj2 = gluNewQuadric();
    gluQuadricDrawStyle(SubQobj2, GLU_FILL);
    glm::mat4 Sub2SphereModel = glm::mat4(1.0f);
    Sub2SphereModel = glm::rotate(Sub2SphereModel, glm::radians(CenterAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
    Sub2SphereModel = glm::rotate(Sub2SphereModel, glm::radians(CenterAngley), glm::vec3(0.0f, 1.0f, 0.0f));
    Sub2SphereModel = glm::translate(Sub2SphereModel, glm::vec3(ObjectTransSize.AllTransX, ObjectTransSize.AllTransY, ObjectTransSize.AllTransZ));
    Sub2SphereModel = glm::translate(Sub2SphereModel, glm::vec3(SubTransSize.Sub2X, SubTransSize.Sub2Y, SubTransSize.Sub2Z));
    Sub2SphereModel = glm::rotate(Sub2SphereModel, glm::radians(SubAngleSize.Sub2X), glm::vec3(1.0f, 0.0f, 0.0f));
    Sub2SphereModel = glm::rotate(Sub2SphereModel, glm::radians(SubAngleSize.Sub2Y), glm::vec3(0.0f, 1.0f, 0.0f));
    Sub2SphereModel = glm::scale(Sub2SphereModel, glm::vec3(0.3f, 0.3f, 0.3f));
    unsigned int Sub2SphereModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(Sub2SphereModelLocation, 1, GL_FALSE, glm::value_ptr(Sub2SphereModel));
    unsigned int Sub2SphereModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(Sub2SphereModelFragLocation, 1.0f, 0.2f, 0.6f);
    gluSphere(SubQobj2, 0.25, 20, 20);

    SubQobj3 = gluNewQuadric();
    gluQuadricDrawStyle(SubQobj3, GLU_FILL);
    glm::mat4 Sub3SphereModel = glm::mat4(1.0f);
    Sub3SphereModel = glm::rotate(Sub3SphereModel, glm::radians(CenterAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
    Sub3SphereModel = glm::rotate(Sub3SphereModel, glm::radians(CenterAngley), glm::vec3(0.0f, 1.0f, 0.0f));
    Sub3SphereModel = glm::translate(Sub3SphereModel, glm::vec3(ObjectTransSize.AllTransX, ObjectTransSize.AllTransY, ObjectTransSize.AllTransZ));
    Sub3SphereModel = glm::translate(Sub3SphereModel, glm::vec3(SubTransSize.Sub3X, SubTransSize.Sub3Y, SubTransSize.Sub3Z));
    Sub3SphereModel = glm::rotate(Sub3SphereModel, glm::radians(SubAngleSize.Sub3X), glm::vec3(1.0f, 0.0f, 0.0f));
    Sub3SphereModel = glm::rotate(Sub3SphereModel, glm::radians(SubAngleSize.Sub3Y), glm::vec3(0.0f, 1.0f, 0.0f));
    Sub3SphereModel = glm::scale(Sub3SphereModel, glm::vec3(0.3f, 0.3f, 0.3f));
    unsigned int Sub3SphereModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(Sub3SphereModelLocation, 1, GL_FALSE, glm::value_ptr(Sub3SphereModel));
    unsigned int Sub3SphereModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(Sub3SphereModelFragLocation, 1.0f, 0.2f, 0.6f);
    gluSphere(SubQobj3, 0.25, 20, 20);

    //미니
    MiniQobj1 = gluNewQuadric();
    gluQuadricDrawStyle(MiniQobj1, GLU_FILL);
    glm::mat4 Mini1SphereModel = glm::mat4(1.0f);
    Mini1SphereModel = glm::rotate(Mini1SphereModel, glm::radians(CenterAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
    Mini1SphereModel = glm::rotate(Mini1SphereModel, glm::radians(CenterAngley), glm::vec3(0.0f, 1.0f, 0.0f));
    Mini1SphereModel = glm::translate(Mini1SphereModel, glm::vec3(ObjectTransSize.AllTransX, ObjectTransSize.AllTransY, ObjectTransSize.AllTransZ));
    Mini1SphereModel = glm::translate(Mini1SphereModel, glm::vec3(SubTransSize.Sub1X, SubTransSize.Sub1Y, SubTransSize.Sub1Z));
    Mini1SphereModel = glm::translate(Mini1SphereModel, glm::vec3(MiniTransSize.Mini1X, MiniTransSize.Mini1Y, MiniTransSize.Mini1Z));
    Mini1SphereModel = glm::rotate(Mini1SphereModel, glm::radians(MiniAngleSize.Mini1X), glm::vec3(1.0f, 0.0f, 0.0f));
    Mini1SphereModel = glm::rotate(Mini1SphereModel, glm::radians(MiniAngleSize.Mini1Y), glm::vec3(0.0f, 1.0f, 0.0f));
    Mini1SphereModel = glm::scale(Mini1SphereModel, glm::vec3(0.3f, 0.3f, 0.3f));
    unsigned int Mini1SphereModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(Mini1SphereModelLocation, 1, GL_FALSE, glm::value_ptr(Mini1SphereModel));
    unsigned int Mini1SphereModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(Mini1SphereModelFragLocation, 0.3f, 0.7f, 0.9f);
    gluSphere(MiniQobj1, 0.25, 20, 20);

    MiniQobj2 = gluNewQuadric();
    gluQuadricDrawStyle(MiniQobj2, GLU_FILL);
    glm::mat4 Mini2SphereModel = glm::mat4(1.0f);
    Mini2SphereModel = glm::rotate(Mini2SphereModel, glm::radians(CenterAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
    Mini2SphereModel = glm::rotate(Mini2SphereModel, glm::radians(CenterAngley), glm::vec3(0.0f, 1.0f, 0.0f));
    Mini2SphereModel = glm::translate(Mini2SphereModel, glm::vec3(ObjectTransSize.AllTransX, ObjectTransSize.AllTransY, ObjectTransSize.AllTransZ));
    Mini2SphereModel = glm::translate(Mini2SphereModel, glm::vec3(SubTransSize.Sub2X, SubTransSize.Sub2Y, SubTransSize.Sub2Z));
    Mini2SphereModel = glm::translate(Mini2SphereModel, glm::vec3(MiniTransSize.Mini2X, MiniTransSize.Mini2Y, MiniTransSize.Mini2Z));
    Mini2SphereModel = glm::rotate(Mini2SphereModel, glm::radians(MiniAngleSize.Mini2X), glm::vec3(1.0f, 0.0f, 0.0f));
    Mini2SphereModel = glm::rotate(Mini2SphereModel, glm::radians(MiniAngleSize.Mini2Y), glm::vec3(0.0f, 1.0f, 0.0f));
    Mini2SphereModel = glm::scale(Mini2SphereModel, glm::vec3(0.3f, 0.3f, 0.3f));
    unsigned int Mini2SphereModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(Mini2SphereModelLocation, 1, GL_FALSE, glm::value_ptr(Mini2SphereModel));
    unsigned int Mini2SphereModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(Mini2SphereModelFragLocation, 0.3f, 0.7f, 0.9f);
    gluSphere(MiniQobj2, 0.25, 20, 20);

    MiniQobj3 = gluNewQuadric();
    gluQuadricDrawStyle(MiniQobj3, GLU_FILL);
    glm::mat4 Mini3SphereModel = glm::mat4(1.0f);
    Mini3SphereModel = glm::rotate(Mini3SphereModel, glm::radians(CenterAnglex), glm::vec3(1.0f, 0.0f, 0.0f));
    Mini3SphereModel = glm::rotate(Mini3SphereModel, glm::radians(CenterAngley), glm::vec3(0.0f, 1.0f, 0.0f));
    Mini3SphereModel = glm::translate(Mini3SphereModel, glm::vec3(ObjectTransSize.AllTransX, ObjectTransSize.AllTransY, ObjectTransSize.AllTransZ));
    Mini3SphereModel = glm::translate(Mini3SphereModel, glm::vec3(SubTransSize.Sub3X, SubTransSize.Sub3Y, SubTransSize.Sub3Z));
    Mini3SphereModel = glm::translate(Mini3SphereModel, glm::vec3(MiniTransSize.Mini3X, MiniTransSize.Mini3Y, MiniTransSize.Mini3Z));
    Mini3SphereModel = glm::rotate(Mini3SphereModel, glm::radians(MiniAngleSize.Mini3X), glm::vec3(1.0f, 0.0f, 0.0f));
    Mini3SphereModel = glm::rotate(Mini3SphereModel, glm::radians(MiniAngleSize.Mini3Y), glm::vec3(0.0f, 1.0f, 0.0f));
    Mini3SphereModel = glm::scale(Mini3SphereModel, glm::vec3(0.3f, 0.3f, 0.3f));
    unsigned int Mini3SphereModelLocation = glGetUniformLocation(shaderID[1], "modeltransform");
    glUniformMatrix4fv(Mini3SphereModelLocation, 1, GL_FALSE, glm::value_ptr(Mini3SphereModel));
    unsigned int Mini3SphereModelFragLocation = glGetUniformLocation(shaderID[1], "vColor");
    glUniform3f(Mini3SphereModelFragLocation, 0.3f, 0.7f, 0.9f);
    gluSphere(MiniQobj3, 0.25, 20, 20);


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
    if (OneSave) {
        MakeCircleLine();
        OneSave = false;
    }
    glGenVertexArrays(6, VAO);

    glBindVertexArray(VAO[0]);
    glGenBuffers(2, &VBO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*SubSphere1.size(), &SubSphere1[0], GL_STREAM_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SubSphere1.size(), &SubSphere1[0], GL_STREAM_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[1]);
    glGenBuffers(2, &VBO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SubSphere2.size(), &SubSphere2[0], GL_STREAM_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SubSphere2.size(), &SubSphere2[0], GL_STREAM_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[2]);
    glGenBuffers(2, &VBO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SubSphere3.size(), &SubSphere3[0], GL_STREAM_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SubSphere3.size(), &SubSphere3[0], GL_STREAM_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);



    glBindVertexArray(VAO[3]);
    glGenBuffers(2, &VBO[6]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * MiniSphere1.size(), &MiniSphere1[0], GL_STREAM_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * MiniSphere1.size(), &MiniSphere1[0], GL_STREAM_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[4]);
    glGenBuffers(2, &VBO[8]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * MiniSphere2.size(), &MiniSphere2[0], GL_STREAM_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[9]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * MiniSphere2.size(), &MiniSphere2[0], GL_STREAM_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[5]);
    glGenBuffers(2, &VBO[10]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[10]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * MiniSphere3.size(), &MiniSphere3[0], GL_STREAM_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[11]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * MiniSphere3.size(), &MiniSphere3[0], GL_STREAM_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

GLvoid keyboard(unsigned char key, int x, int y) {
    switch (key)
    {

    case 'c': //초기화
        CameraPoint.ViewZ = 2.3f;
        CameraPoint.ViewX = 0.0f;
        CameraPoint.ViewY = 0.0f;
        memset(&ObjectTransSize, 0, sizeof(ObjectTransSize));
        memset(&SubAngleSize, 0, sizeof(SubAngleSize));
        memset(&MiniAngleSize, 0, sizeof(MiniAngleSize));
        MiniRotate = false;
        SubRotate = false;
        SubSphere1Rotate = 0;
        MiniSphere1Rotate = 0;
        SubSphere2Rotate = 0;
        MiniSphere2Rotate = 0;
        SubSphere3Rotate = 0;
        MiniSphere3Rotate = 0;
        break;
    //카메라 방향
    case 'x':
        CameraPoint.ViewX += 0.1;
        break;
    case 'X':
        CameraPoint.ViewX -= 0.1;
        break;
    case 'y':
        CameraPoint.ViewY += 0.1;
        break;
    case 'Y':
        CameraPoint.ViewY -= 0.1;
        break;
    case 'z':
        CameraPoint.ViewZ += 0.1;
        break;
    case 'Z':
        CameraPoint.ViewZ -= 0.1;
        break;
    case 'm':
        MiniRotate = !MiniRotate;
        break;
    case 'u':
        SubRotate = !SubRotate;
        break;
    //공전
    case 'r':
        CenterRotatey++;
        CenterRotateY = 0;
        break;
    case 'R':
        CenterRotateY++;
        CenterRotatey = 0;
        break;  
    //와이어 모드/솔리드 모드
    case 'n':
        PolygonWireMode = !PolygonWireMode;
        break;
    //방향
    case 'w':
        ObjectTransSize.AllTransY += 0.1f;
        break;
    case 'a':
        ObjectTransSize.AllTransX -= 0.1f;
        break;
    case 's':
        ObjectTransSize.AllTransY -= 0.1f;
        break;
    case 'd':
        ObjectTransSize.AllTransX += 0.1f;
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
void MakeCircleLine() {
    float CircleX, CircleZ, CircleY;

    for (int i = 0; i < 360; i++) {
        float r = 0.8;
        CircleX = r * cos(3.14 * i / 180);
        CircleZ = r * sin(3.14 * i / 180);

        SubSphere1.push_back(CircleX);
        SubSphere1.push_back(0);
        SubSphere1.push_back(CircleZ);
        SubSphere1.push_back(0);
        SubSphere1.push_back(0);
        SubSphere1.push_back(0);
    }
    for (int i = 0; i < 360; i++) {
        float r = 0.8;
        
        CircleX = r * cos(3.14 * i / 180);
        CircleY = (-1)*r * cos(3.14 * i / 180);
        CircleZ = r * sin(3.14 * i / 180);

        SubSphere2.push_back(CircleX);
        SubSphere2.push_back(CircleY);
        SubSphere2.push_back(CircleZ);
        SubSphere2.push_back(0);
        SubSphere2.push_back(0);
        SubSphere2.push_back(0);
    }
    for (int i = 0; i < 360; i++) {
        float r = 0.8;

        CircleX = r * cos(3.14 * i / 180);
        CircleY = r * cos(3.14 * i / 180);
        CircleZ = r * sin(3.14 * i / 180);

        SubSphere3.push_back(CircleX);
        SubSphere3.push_back(CircleY);
        SubSphere3.push_back(CircleZ);
        SubSphere3.push_back(0);
        SubSphere3.push_back(0);
        SubSphere3.push_back(0);
    }


    for (int i = 0; i < 360; i++) {
        float r = 0.4;

        CircleX = r * cos(3.14 * i / 180);
        CircleZ = r * sin(3.14 * i / 180);

        MiniSphere1.push_back(CircleX);
        MiniSphere1.push_back(0);
        MiniSphere1.push_back(CircleZ);
        MiniSphere1.push_back(1);
        MiniSphere1.push_back(1);
        MiniSphere1.push_back(1);
    }
    for (int i = 0; i < 360; i++) {
        float r = 0.4;

        CircleX = r * cos(3.14 * i / 180);
        CircleZ = r * sin(3.14 * i / 180);

        MiniSphere2.push_back(CircleX);
        MiniSphere2.push_back(0);
        MiniSphere2.push_back(CircleZ);
        MiniSphere2.push_back(1);
        MiniSphere2.push_back(1);
        MiniSphere2.push_back(1);
    }
    for (int i = 0; i < 360; i++) {
        float r = 0.4;

        CircleX = r * cos(3.14 * i / 180);
        CircleZ = r * sin(3.14 * i / 180);

        MiniSphere3.push_back(CircleX);
        MiniSphere3.push_back(0);
        MiniSphere3.push_back(CircleZ);
        MiniSphere3.push_back(1);
        MiniSphere3.push_back(1);
        MiniSphere3.push_back(1);
    }





}

void timer(int value) {

	//미니, 솔리드, 객체 전체 이동(z,x는 원근 투영 건드리라는거 같음)
    if (MiniRotate) {
        MiniAngleSize.Mini1X += 64.0f;
        MiniAngleSize.Mini1Y += 64.0f;
        MiniAngleSize.Mini1Z += 64.0f;

        MiniAngleSize.Mini2X += 32.0f;
        MiniAngleSize.Mini2Y += 32.0f;
        MiniAngleSize.Mini2Z += 32.0f;

        MiniAngleSize.Mini3X += 16.0f;
        MiniAngleSize.Mini3Y += 16.0f;
        MiniAngleSize.Mini3Z += 16.0f;
    }

    if (SubRotate) {
        SubAngleSize.Sub1X += 2.0f;
        SubAngleSize.Sub1Y += 2.0f;
        SubAngleSize.Sub1Z += 2.0f;

        SubAngleSize.Sub2X += 4.0f;
        SubAngleSize.Sub2Y += 4.0f;
        SubAngleSize.Sub2Z += 4.0f;

        SubAngleSize.Sub3X += 8.0f;
        SubAngleSize.Sub3Y += 8.0f;
        SubAngleSize.Sub3Z += 8.0f;
    }








    if (CenterRotateY > 0) {
        if (CenterRotateY % 2 == 1) {
            CenterAngley--;
        }
        else {
            CenterRotateY = 0;
        }
    }
    if (CenterRotatey > 0) {
        if (CenterRotatey % 2 == 1) {
            CenterAngley++;
        }
        else {
            CenterRotatey = 0;
        }
    }

    SubTransSize.Sub1X = SubSphere1[6 * SubSphere1Rotate];
    SubTransSize.Sub1Y = SubSphere1[6 * SubSphere1Rotate + 1];
    SubTransSize.Sub1Z = SubSphere1[6 * SubSphere1Rotate + 2];
   
    SubTransSize.Sub2X = SubSphere2[18 * SubSphere2Rotate];
    SubTransSize.Sub2Y = SubSphere2[18 * SubSphere2Rotate + 1];
    SubTransSize.Sub2Z = SubSphere2[18 * SubSphere2Rotate + 2];
    
    SubTransSize.Sub3X = SubSphere3[12 * SubSphere3Rotate];
    SubTransSize.Sub3Y = SubSphere3[12 * SubSphere3Rotate + 1];
    SubTransSize.Sub3Z = SubSphere3[12 * SubSphere3Rotate + 2];
    
    MiniTransSize.Mini1X = MiniSphere1[12 * MiniSphere1Rotate];
    MiniTransSize.Mini1Y = MiniSphere1[12 * MiniSphere1Rotate + 1];
    MiniTransSize.Mini1Z = MiniSphere1[12 * MiniSphere1Rotate + 2];

    MiniTransSize.Mini2X = MiniSphere2[24 * MiniSphere2Rotate];
    MiniTransSize.Mini2Y = MiniSphere2[24 * MiniSphere2Rotate + 1];
    MiniTransSize.Mini2Z = MiniSphere2[24 * MiniSphere2Rotate + 2];

    MiniTransSize.Mini3X = MiniSphere3[18 * MiniSphere3Rotate];
    MiniTransSize.Mini3Y = MiniSphere3[18 * MiniSphere3Rotate + 1];
    MiniTransSize.Mini3Z = MiniSphere3[18 * MiniSphere3Rotate + 2];

    SubSphere1Rotate++;
    MiniSphere1Rotate++;
    SubSphere2Rotate++;
    MiniSphere2Rotate++;
    SubSphere3Rotate++;
    MiniSphere3Rotate++;

    if (SubSphere1Rotate == 360) {
        SubSphere1Rotate = 0;
    }
    if (MiniSphere1Rotate == 180) {
        MiniSphere1Rotate = 0;
    }

    if (SubSphere2Rotate == 120) {
        SubSphere2Rotate = 0;
    }

    if (MiniSphere2Rotate == 90) {
        MiniSphere2Rotate = 0;
    }

    if (SubSphere3Rotate == 180) {
        SubSphere3Rotate = 0;
    }

    if (MiniSphere3Rotate == 120) {
        MiniSphere3Rotate = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(60, timer, 1);
}
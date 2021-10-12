#include <iostream>
#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <gl/freeglut.h>


using namespace std;
void make_vertexShaders();
void make_fragmentShader();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid KeyBoard(unsigned char Key, int x, int y);
GLvoid SpecialKeyBoard(int key, int x, int y);
void timer(int value);

GLuint s_program;
GLuint VAO[3], VBO[6];
GLuint shaderID;
GLint width, height;
GLuint vertexShader;
GLuint fragmentShader;
int shapechk = 0;
bool cullchk = false;
bool solidchk = false;
bool movekey = false;
float a_x = 30.0f;
float a_y = -30.0f;
float t_x = 0.0f;
float t_y = 0.0f;

float data_line[] =
{             //사분면선
   0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
   0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

   -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
   1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
};

float data_hexa[] =
{    //육면체
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

float data_quad[] =
{   //사각뿔
   -0.3f, -0.3f, -0.3f, 0.5f, 0.5f, 0.5f,  //1
   0.0f, 0.3f, 0.0f, 1.0f, 0.0f, 0.0f,  //0
   0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.3f,  //2

   0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.3f,  //2
   0.0f, 0.3f, 0.0f, 1.0f, 0.0f, 0.0f,  //0
   0.3f, -0.3f, 0.3f, 0.0f, 0.7f, 1.0f,  //3

   0.3f, -0.3f, 0.3f, 0.0f, 0.7f, 1.0f,  //3
   0.0f, 0.3f, 0.0f, 1.0f, 0.0f, 0.0f,  //0
   -0.3f, -0.3f, 0.3f, 1.0f, 1.0f, 0.12f,  //4

   -0.3f, -0.3f, -0.3f, 0.5f, 0.5f, 0.5f,  //1
   -0.3f, -0.3f, 0.3f, 1.0f, 1.0f, 0.12f,  //4
   0.0f, 0.3f, 0.0f, 1.0f, 0.0f, 0.0f,  //0

   -0.3f, -0.3f, 0.3f, 1.0f, 1.0f, 0.12f,  //4
   -0.3f, -0.3f, -0.3f, 0.5f, 0.5f, 0.5f,  //1
   0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.3f,  //2

   -0.3f, -0.3f, 0.3f, 1.0f, 1.0f, 0.12f,  //4
   0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.3f,  //2
   0.3f, -0.3f, 0.3f, 0.0f, 0.7f, 1.0f  //3
};

GLchar* filetobuf(const GLchar* file)
{
    FILE* fptr;
    long length;
    GLchar* buf;

    fopen_s(&fptr, file, "rb");
    if (!fptr)
        return NULL;

    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr);
    buf = (GLchar*)malloc(length + 1);
    fseek(fptr, 0, SEEK_SET);
    fread(buf, length, 1, fptr);
    fclose(fptr);
    buf[length] = 0;

    return buf;
}

void make_vertexShaders()
{
    GLchar* vertexSource;
    vertexSource = filetobuf("vertexshader.glsl");
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

}

void make_fragmentShader()
{
    GLchar* fragmentSource;
    fragmentSource = filetobuf("fragmentshader.glsl");
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

}

void InitShader()
{
    make_vertexShaders();
    make_fragmentShader();

    s_program = glCreateProgram();

    glAttachShader(s_program, vertexShader);
    glAttachShader(s_program, fragmentShader);
    glLinkProgram(s_program);

    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        cerr << "ERROR :  fragment Shader Fail Compile \n" << errorLog << endl;
        exit(-1);
    }

    else
        cout << "good";
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(s_program);
}
GLvoid InitBuffer()
{

    glGenVertexArrays(3, VAO);

    //line
    //->vertex
    glBindVertexArray(VAO[0]);
    glGenBuffers(2, &VBO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data_line), data_line, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //->color
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]); // 색상
    glBufferData(GL_ARRAY_BUFFER, sizeof(data_line), data_line, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    //hexa
    //->vertex
    glBindVertexArray(VAO[1]);
    glGenBuffers(2, &VBO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data_hexa), data_hexa, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //->color
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]); // 색상
    glBufferData(GL_ARRAY_BUFFER, sizeof(data_hexa), data_hexa, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //quad
    //->vertex
    glBindVertexArray(VAO[2]);
    glGenBuffers(2, &VBO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data_quad), data_quad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //->color
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]); // 색상
    glBufferData(GL_ARRAY_BUFFER, sizeof(data_quad), data_quad, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


}
void timer(int value)
{

    glutPostRedisplay();
    glutTimerFunc(100, timer, 1);
}
void drawScene()
{

    GLfloat rColor = 0.2;
    GLfloat gColor = 0.2;
    GLfloat bColor = 0.2;
    glClearColor(rColor, gColor, bColor, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //깊이 체크 (컬링)

    InitBuffer(); //순서
    glUseProgram(s_program);
    //line
    glm::mat4 L_rotMatrix = glm::mat4(1.0f); // 단위 행렬로 초기화 
    L_rotMatrix = glm::rotate(L_rotMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    unsigned int transformLocation = glGetUniformLocation(s_program, "modeltransform");
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(L_rotMatrix));
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_LINES, 0, 4);

    if (shapechk == 1)//hexa
    {

        glm::mat4 H_rotMatrix = glm::mat4(1.0f); // 단위 행렬로 초기화 
        H_rotMatrix = glm::translate(H_rotMatrix, glm::vec3(t_x, t_y, 0.0f));
        H_rotMatrix = glm::rotate(H_rotMatrix, glm::radians(a_x), glm::vec3(1.0f, 0.0f, 0.0f));
        H_rotMatrix = glm::rotate(H_rotMatrix, glm::radians(a_y), glm::vec3(0.0f, 1.0f, 0.0f));
        unsigned int transformLocation = glGetUniformLocation(s_program, "modeltransform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(H_rotMatrix));
        if (cullchk == true)
        {
            glEnable(GL_CULL_FACE);
        }
        else
            glDisable(GL_CULL_FACE);

        if (solidchk == true)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }


    if (shapechk == 2)//quad
    {
        glm::mat4 S_rotMatrix = glm::mat4(1.0f); // 단위 행렬로 초기화 
        S_rotMatrix = glm::translate(S_rotMatrix, glm::vec3(t_x, t_y, 0.0f));
        S_rotMatrix = glm::rotate(S_rotMatrix, glm::radians(a_x), glm::vec3(1.0f, 0.0f, 0.0f));
        S_rotMatrix = glm::rotate(S_rotMatrix, glm::radians(a_y), glm::vec3(0.0f, 1.0f, 0.0f));
        unsigned int transformLocation = glGetUniformLocation(s_program, "modeltransform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(S_rotMatrix));

        if (cullchk == true)
        {
            glEnable(GL_CULL_FACE);
        }
        else
            glDisable(GL_CULL_FACE);

        if (solidchk == true)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glBindVertexArray(VAO[2]);
        glDrawArrays(GL_TRIANGLES, 0, 18);

    }

    glutSwapBuffers();
}

GLvoid KeyBoard(unsigned char Key, int x, int y)
{
    switch (Key)
    {
    case 'c': //육면체
        shapechk = 1;
        drawScene();
        break;
    case 'p': // 사각뿔
        shapechk = 2;
        drawScene();
        break;
    case 'h': //은면제거 적용/해제
        cullchk = !cullchk;
        drawScene();
        break;
    case 'x': // x축 기준으로 양/음 회전
        break;
    case 'X':
        break;
    case 'y':  // y축 기준으로 양/음 회전
        break;
    case 'Y':
        break;
    case 'w': // 와이어 솔리드
        solidchk = !solidchk;
        drawScene();
        break;
    case 'W':
        solidchk = !solidchk;
        drawScene();
        break;
    case 's': //초기 위치로 리셋 자전도 멈춤
        drawScene();
        break;

    }
}

GLvoid SpecialKeyBoard(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        t_x -= 0.03f;
        break;
    case GLUT_KEY_RIGHT:
        t_x += 0.03f;
        break;
    case GLUT_KEY_UP:
        t_y += 0.03f;
        break;
    case GLUT_KEY_DOWN:
        t_y -= 0.03f;
        break;
    default:
        break;
    }

}
GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

void main(int argc, char** argv)
{

    width = 800;
    height = 600;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); //depth+
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(width, height);
    glutCreateWindow("Example 1");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        cerr << "NOT INIT" << endl;
    }
    else
        cout << "INIT<<endl";

    InitShader();

    InitBuffer();
    //glutTimerFunc(100, timer, 1);
    glutKeyboardFunc(KeyBoard);
    glutSpecialFunc(SpecialKeyBoard);
    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);

    glutMainLoop();
}

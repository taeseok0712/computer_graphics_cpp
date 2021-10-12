#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <Windows.h>
#include <queue>

using namespace std;
GLvoid drawscene(GLvoid);
GLvoid reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void resize(int width, int height);
void random2();
void oringerect();
void deleterect();
void timer(int value);
GLvoid keyboard(unsigned char key, int x, int y);
GLboolean Count = false;
GLboolean check = false;
float a = 0.0;
float b = 0.0;
float c = 0.0;
float d = 0.0;
float e = 0.0;
float f = 0.0;
bool left_button;

float rsize = 25;
float xstep = 0.007;
float ystep = 0.007;
int windowWidth;
int windowHeight;
float xscale = 0.003;
float yscale = 0.003;
struct rect {
    float color[3];
    float x2, y2, x3, y3;
    float xmove, ymove;
    float xsize = 1, ysize = -1;
    float firstx2, firstx3, firsty2, firsty3;
};
queue<rect>r;
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
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
    glutDisplayFunc(drawscene);
    glutReshapeFunc(resize);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(1, timer, 1);
    glutMainLoop();
}
GLvoid drawscene() {

    glClearColor(0.5, 0.5, 0.5, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    int sizeq = r.size();
    for (int i = 0; i < sizeq; i++) {
        glColor3f(r.front().color[0], r.front().color[1], r.front().color[2]);
        glRectf(r.front().x2, r.front().y2, r.front().x3, r.front().y3);
        rect temp = r.front();
        r.pop();
        r.push(temp);
    }
    glutSwapBuffers();
}
GLvoid reshape(int w, int h) {
    glViewport(0, 0, 800, 600);
}
void random2() {
    random_device rd;
    default_random_engine dre(rd());
    uniform_real_distribution<>uid(0.0, 1.0);
    float a1 = uid(dre);
    float b1 = uid(dre);
    float c1 = uid(dre);
    d = a1;
    e = b1;
    f = c1;
}
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int makecount = r.size();
        if (makecount == 5)
            r.pop();
        float x2 = (((float)(x - (windowWidth / 2)) / (windowWidth / 2)) - 0.05);
        float x3 = (((float)(x - (windowWidth / 2)) / (windowWidth / 2)) + 0.05);
        float y2 = ((((float)(windowHeight / 2) - y) / (windowHeight / 2)) - 0.05);
        float y3 = ((((float)(windowHeight / 2) - y) / (windowHeight / 2)) + 0.05);
        rect temp;
        temp.x2 = x2;
        temp.x3 = x3;
        temp.y2 = y2;
        temp.y3 = y3;
        random2();
        temp.color[0] = d;
        temp.color[1] = e;
        temp.color[2] = f;
        temp.xmove = 1.0;
        temp.ymove = 1.0;
        temp.xsize = 1;
        temp.ysize = -1;
        temp.firstx2 = x2;
        temp.firstx3 = x3;
        temp.firsty2 = y2;
        temp.firsty3 = y3;
        r.push(temp);
        glutPostRedisplay();
    }
}
void resize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
}
void timer(int value) {
    int sizet = r.size();
    if (Count) {
        for (int j = 0; j < sizet; j++) {
            if (r.front().xsize > 0 && r.front().x3 - r.front().x2 > 0.5) {
                r.front().xsize = -1.0;
            }
            else if (r.front().xsize < 0 && r.front().x3 - r.front().x2 < 0.05) {
                r.front().xsize = 1.0;
            }
            if (r.front().ysize < 0 && r.front().y2 - r.front().y3 > 0.5) {
                r.front().ysize = 1.0;
            }
            else if (r.front().ysize > 0 && r.front().y2 - r.front().y3 < 0.05) {
                r.front().ysize = -1.0;
            }
            r.front().x2 -= xscale * r.front().xsize;
            r.front().x3 += xscale * r.front().xsize;
            r.front().y2 -= yscale * r.front().ysize;
            r.front().y3 += yscale * r.front().ysize;
            rect temp = r.front();
            r.pop();
            r.push(temp);
        }
    }
    if (check) {
        for (int i = 0; i < sizet; i++) {
            if (r.front().x3 >= 1.0 )              
                    r.front().xmove = -1.0;                
            else if(r.front().x2 <= -1.0)
                r.front().xmove = 1.0;

            if (r.front().y3 >= 1.0 ) 
                r.front().ymove = -1.0;        
            else if(r.front().y2 <= -1.0)
                r.front().ymove = 1.0;

            r.front().x2 += xstep * r.front().xmove;
            r.front().x3 += xstep * r.front().xmove;
            r.front().y3 += ystep * r.front().ymove;
            r.front().y2 += ystep * r.front().ymove;
            rect temp = r.front();
            r.pop();
            r.push(temp);
        }
    }
    glutPostRedisplay();
    glutTimerFunc(1, timer, 1);
}
GLvoid keyboard(unsigned char key, int x, int y) {
    switch (key)
    {
    case 'a':
        check = true;
        cout << "move rectangle" << endl;
        break;
    case 's':
        check = false;
        Count = false;
        cout << "All stopped" << endl;
        break;
    case 'c':
        Count = true;
        cout << "scale rectangle" << endl;
        break;
    case 'm':
        oringerect();
        cout << "go back to the initial coordinates " << endl;
        break;
    case 'r':
        deleterect();
        cout << "delete all rectangle" << endl;
        break;
    case 'q':
        exit(0);
        cout << "exit the program" << endl;
        break;
    default:
        break;
    }
}
void oringerect() {
    int sizeo = r.size();
    for (int a = 0; a < sizeo; a++) {
        r.front().x2 = r.front().firstx2;
        r.front().x3 = r.front().firstx3;
        r.front().y2 = r.front().firsty2;
        r.front().y3 = r.front().firsty3;

        rect temp = r.front();
        r.pop();
        r.push(temp);
    }
    glutPostRedisplay();
}
void deleterect() {
    int sizek = r.size();
    for (int b = 0; b < sizek; b++) {
        r.pop();
    }
}
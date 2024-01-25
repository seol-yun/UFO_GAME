#define _USE_MATH_DEFINES
#include <string.h>
#include<string>
#include <math.h>
#include <glut.h>
#include <GLU.h>
#include <glaux.h>
#include <iostream>
#include<windows.h>
#define rad (3.1416/180)
using namespace std;
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

using namespace std;
unsigned int ids[2]; //�ؽ���
AUX_RGBImageRec* tex[2]; //�ؽ���
float uX = 0, uY = 0, uZ = -8; //UFO�� ��ǥ
float Z[] = { -20, -40, -60, -80, -100, -120 }; //background ��ǥ
float starZ[7] = { 0, 11, -97, -80, -57, -37, -17 }; //�� ������ǥ
float incX = 0, incY = 0, incZ = 0; //UFO ����
float speed = 0.3; //�ӵ�
float incBackFrac = 0.2; //�ٽ� ������ ����
bool start = false; //��������
float itemPosX[] = { 1,-2,6, 7, -5, 9, 3}; //�� �����ġ X
float itemPosY[] = { 2,3,10,6,7,4,1 ,10}; //�� �����ġ Y
int score = 0; //����


void UFO() {
    // ���� ��ü
    glColor3d(1, 1, 1);
    tex[0] = auxDIBImageLoad("img.bmp");
    glGenTextures(2, &ids[0]); // �ؽ���
    glBindTexture(GL_TEXTURE_2D, ids[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex[0]->sizeX, tex[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, tex[0]->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    GLfloat plane_coef_s[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat plane_coef_t[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat light_emission[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat ambient1[] = { 1.0, 1.0, 1.0, 1.0 };
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenfv(GL_S, GL_OBJECT_PLANE, plane_coef_s);

    glEnable(GL_TEXTURE_GEN_T);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenfv(GL_T, GL_OBJECT_PLANE, plane_coef_t);

    glBindTexture(GL_TEXTURE_2D, ids[0]);

    glPushMatrix();
    glScaled(1.5, 0.3, 1.5);
    glutSolidSphere(1, 30, 30);
    glPopMatrix();

    // ���κ� ��
    glColor3d(1, 1, 1); 
    glPushMatrix();
    glTranslated(0, 0.2, 0);
    glutSolidSphere(0.5, 30, 30);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    
}


void obstacle() { //��ֹ�

    // ���η� ������ Ÿ���� ��
    glPushMatrix();
    glColor3d(0.5, 0.5, 0.5);
    glTranslated(0, 0, 0);
    glScaled(1.0, 10.0, 1.0); 
    glutSolidSphere(0.5, 50, 50);
    glPopMatrix();

    glPushMatrix();
    glColor3d(0, 0, 0);
    glTranslated(0, 0, 5);
    glScaled(1.0, 20.0, 1.0); 
    glutSolidSphere(0.5, 50, 50);
    glPopMatrix();

    glPushMatrix();
    glColor3d(0.3, 0.3, 0.3);
    glTranslated(0, 0, 10);
    glScaled(1.5, 30.0, 1.5); 
    glutSolidSphere(0.5, 50, 50);
    glPopMatrix();

}

void obstacle2() { //��ֹ�2

    // ���η� ������ Ÿ���� ��2
    glPushMatrix();
    glColor3d(0.5, 0.5, 0.5);
    glTranslated(0, 0, 10);
    glScaled(1.0, 10.0, 1.0); 
    glutSolidSphere(0.5, 50, 50);
    glPopMatrix();

    glPushMatrix();
    glColor3d(0, 0, 0.1);
    glTranslated(0, 0, 15);
    glScaled(1.0, 20.0, 1.0); 
    glutSolidSphere(0.5, 50, 50);
    glPopMatrix();

    glPushMatrix();
    glColor3d(0.3, 0.3, 0.3);
    glTranslated(0, 0, 20);
    glScaled(1.5, 40.0, 1.5); 
    glutSolidSphere(0.5, 50, 50);
    glPopMatrix();

}
void starObstacle() { //����� ������
    const float outerRadius = 5;
    const float innerRadius = 2;
    const int numPoints = 5;  
   
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0, 0.0);  

    for (int i = 0; i <= numPoints * 2; i++) {
        float angle = i * (360.0 / (numPoints * 2)) * rad;
        float radius = (i % 2 == 0) ? outerRadius : innerRadius;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex2f(x, y);
    }

    glEnd();
}

void background(int n) {
    // ��
    glColor3d(0.5, 0.3, 0.3);
    glPushMatrix();
    glTranslated(0, 0, 0);
    glScaled(50, 1, 50);
    glutSolidCube(1);
    glPopMatrix();

    //�Դ°�(��)
    glColor3d(1, 1, 0.1);
    glutSolidTorus(10, 30, 30, 30);
    glPushMatrix();
    glTranslated(itemPosX[n], itemPosY[n], 0);

    glScaled(0.3, 0.3, 0.3);
    starObstacle();
    glPopMatrix();

    //��ֹ�
    glPushMatrix();
    glTranslated(-10, 0, 3);
    obstacle();
    glPopMatrix();

    glPushMatrix();
    glTranslated(10, 0, 3);
    obstacle2();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-3, 0, 3);
    obstacle2();
    glPopMatrix();

    glPushMatrix();
    glTranslated(2, 0, 0);
    obstacle();
    glPopMatrix();
}
bool checkCollision() { //������ �浹 ���� �Լ�
    for (int i = 0; i < 7; i++)
    {
        float distance = sqrt(pow(uX + itemPosX[i], 2) + pow(uY + itemPosY[i], 2)) + pow(uZ - starZ[i], 2); //�����۰� UFO�� �Ÿ� ���
        if (distance < 1.3) { //�Ÿ��� ���Ǻ��� ������ �浹 Ž��
            return true;
        }
    }
    
    return false;
    
}

int temp = 1000;
void draw() {
    /// UFO
    if (incX > 25) { //���Ⱑ ���������� ������ �ٽ� ������
        incX = 25;
    }
    if (incX < -25) {
        incX = -25;
    }
    if (incZ > 25) {
        incZ = 25;
    }
    if (incZ < -25) {
        incZ = -25;
    }
    glPushMatrix(); //UFO �׸���
    glTranslated(0, 1, 0);
    glRotated(90, 0, 1, 0);
    glRotated(5, 0, 0, 1);
    glRotated(incX, 1, 0, 0);
    glRotated(incY, 0, 1, 0);
    glRotated(incZ, 0, 0, 1);
    glScaled(0.45, 0.45, 0.45);
    UFO();
    glPopMatrix();

    if (uX >= 10)uX = 10; //���������� ���Ѿ���� ��
    if (uX <= -10)uX = -10;
    if (uY > 0.1)uY = 0.1;
    if (uY < -15)uY = -15;

    glPushMatrix(); //background �ݺ�
    glTranslated(uX, uY, uZ);
    background(0);
    glPopMatrix();

    for (int i = 0; i < 6; i++)
    {
        glPushMatrix();
        glTranslated(uX, uY, Z[i]);
        background(i+1);
        glPopMatrix();
    }
    uZ += speed; //UFO�� �ƴ� ����� �ڷΰ���
    for (int i = 0; i < 6; i++)
    {
        Z[i] += speed;
    }
    if (uZ >= 20)uZ = -110; //���� ������ ������ �ݺ����ش�
    for (int i = 0; i < 6; i++)
    {
        if (Z[i] >= 20) {
            Z[i] = -110;
        }
    }
    if (incX > 0)incX -= incBackFrac; //���⸦ �ڵ����� �����ش�
    if (incX < 0)incX += incBackFrac;
    if (incY > 0)incY -= incBackFrac;
    if (incY < 0)incY += incBackFrac;
    if (incZ > 0)incZ -= incBackFrac;
    if (incZ < 0)incZ += incBackFrac;

    speed += 0.0002;
    if (speed >= 1)speed = 1;
    // �� �浹Ž��, ���� �ø���
    if (checkCollision()) {
        if (temp - uZ > 5 || uZ -temp >5) {
            score++;
            temp = uZ;
        }
    }
}

void titleText(char* text, int x, int y, int z) //����
{
    glPushMatrix();
    glColor3d(1, 1, 1);
    glTranslatef(x-4, y + 7, z);
    glScalef(0.01f, 0.01f, z);

    for (const char* c = text; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }

    glPopMatrix();
}
void startText(char* text, int x, int y, int z) //���۾ȳ���
{
    glPushMatrix();
    glColor3d(1 ,1 ,1);
    glTranslatef(x-1, y+4, z);
    glScalef(0.005f, 0.005f, z);

    for (const char* c = text; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }

    glPopMatrix();
}

void drawStrokeText(const char* str, int x, int y, int z) //UFO�� ��ǥ, ���� ǥ��
{
    const char* c;
    glPushMatrix();
    glTranslatef(x, y + 8, z);
    glScalef(0.002f, 0.002f, z);

    for (c = str; *c != '\0'; c++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }
    glPopMatrix();
}
void reshape(int w, int h)
{
    float ratio = (float)w / (float)h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ratio, ratio, -1.0, 1.0, 2.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0, 4.5, 10.0,
        0, 4, 0,
        0, 1.0f, 0.0f);

    if (start) { //�����ϸ� 
        glPushMatrix();
        glTranslated(0, 0, 0);
        glScaled(3, 3, 3);
        draw();
        glPopMatrix();
        string t = to_string(score);
        drawStrokeText(("Score: " + t).c_str(), 3, 0, 0); //���� ǥ��
        string ufoCoordText = "UFO Coordinates: (" + to_string(uX) + ", " + to_string(uY) + ", " + to_string(uZ) + ")";
        drawStrokeText(ufoCoordText.c_str(), 3, -1, 0); //UFO ��ǥ ǥ��
    }
    else { //�޴�ȭ��
        glClearColor(0, 0, 0, 1);
        titleText("The UFO game", -1, -1, 0); //����
        startText("let's start with 'q'", -1, -1, 0); //q�� ������ ����
    }
    glColor3d(0, 0, 0);
   
    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y) //Ű �Է� ����
{
    float frac = 0.3; //����
    float incFrac = 1; //�յڰ���
    switch (key)
    {
    case 'z': //z�� ������ ����
        exit(0);
        break;
    case 'w':  //���� ����
        uY -= frac;
        incZ += incFrac;
        break;
    case 's': //�Ʒ��� ����
        uY += frac;
        incZ -= incFrac;
        break;
    case 'a': //���� �̵�
        uX += frac;
        incX -= incFrac * 2;
        incY += incFrac / 2;
        break;
    case 'd': //������ �̵�
        uX -= frac;
        incX += incFrac * 2;
        incY -= incFrac / 2;
        break;
    case 'q': //q�� ������ ����
        start = true;
        break;
    case 'e': //e�� ������ �޴�ȭ��
        start = false;
        break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

const GLfloat l_ambient[] = { 0, 0, 0, 1.0 }; //������
const GLfloat l_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat l_specular[] = { 1, 1, 1, 1.0 };
const GLfloat l_position[] = { 5, 5, 5, 0.2 };

const GLfloat m_ambient[] = { 0.6, 0.6, 0.6, 1.0 }; //��üǥ��
const GLfloat m_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
const GLfloat m_specular[] = { 1, 1, 1, 1.0 };
const GLfloat shininess[] = { 100.0 };


int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(1400, 750);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);

    glutCreateWindow("UFO game");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(0.8, 0.8, 1, 1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT, l_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, l_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, l_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, l_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT, m_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    glutMainLoop();

    return 0;
}

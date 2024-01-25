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
unsigned int ids[2]; //텍스쳐
AUX_RGBImageRec* tex[2]; //텍스쳐
float uX = 0, uY = 0, uZ = -8; //UFO의 좌표
float Z[] = { -20, -40, -60, -80, -100, -120 }; //background 좌표
float starZ[7] = { 0, 11, -97, -80, -57, -37, -17 }; //별 절대좌표
float incX = 0, incY = 0, incZ = 0; //UFO 기울기
float speed = 0.3; //속도
float incBackFrac = 0.2; //다시 돌려줄 기울기
bool start = false; //시작조건
float itemPosX[] = { 1,-2,6, 7, -5, 9, 3}; //별 상대위치 X
float itemPosY[] = { 2,3,10,6,7,4,1 ,10}; //별 상대위치 Y
int score = 0; //점수


void UFO() {
    // 메인 몸체
    glColor3d(1, 1, 1);
    tex[0] = auxDIBImageLoad("img.bmp");
    glGenTextures(2, &ids[0]); // 텍스쳐
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

    // 윗부분 돔
    glColor3d(1, 1, 1); 
    glPushMatrix();
    glTranslated(0, 0.2, 0);
    glutSolidSphere(0.5, 30, 30);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    
}


void obstacle() { //장애물

    // 세로로 길쭉한 타원형 집
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

void obstacle2() { //장애물2

    // 세로로 길쭉한 타원형 집2
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
void starObstacle() { //별모양 아이템
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
    // 땅
    glColor3d(0.5, 0.3, 0.3);
    glPushMatrix();
    glTranslated(0, 0, 0);
    glScaled(50, 1, 50);
    glutSolidCube(1);
    glPopMatrix();

    //먹는거(별)
    glColor3d(1, 1, 0.1);
    glutSolidTorus(10, 30, 30, 30);
    glPushMatrix();
    glTranslated(itemPosX[n], itemPosY[n], 0);

    glScaled(0.3, 0.3, 0.3);
    starObstacle();
    glPopMatrix();

    //장애물
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
bool checkCollision() { //아이템 충돌 감지 함수
    for (int i = 0; i < 7; i++)
    {
        float distance = sqrt(pow(uX + itemPosX[i], 2) + pow(uY + itemPosY[i], 2)) + pow(uZ - starZ[i], 2); //아이템과 UFO의 거리 계산
        if (distance < 1.3) { //거리가 조건보다 작으면 충돌 탐지
            return true;
        }
    }
    
    return false;
    
}

int temp = 1000;
void draw() {
    /// UFO
    if (incX > 25) { //기울기가 일정수준을 넘으면 다시 돌려줌
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
    glPushMatrix(); //UFO 그리기
    glTranslated(0, 1, 0);
    glRotated(90, 0, 1, 0);
    glRotated(5, 0, 0, 1);
    glRotated(incX, 1, 0, 0);
    glRotated(incY, 0, 1, 0);
    glRotated(incZ, 0, 0, 1);
    glScaled(0.45, 0.45, 0.45);
    UFO();
    glPopMatrix();

    if (uX >= 10)uX = 10; //일정공간을 못넘어가도록 함
    if (uX <= -10)uX = -10;
    if (uY > 0.1)uY = 0.1;
    if (uY < -15)uY = -15;

    glPushMatrix(); //background 반복
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
    uZ += speed; //UFO가 아닌 배경이 뒤로간다
    for (int i = 0; i < 6; i++)
    {
        Z[i] += speed;
    }
    if (uZ >= 20)uZ = -110; //일정 범위를 넘으면 반복해준다
    for (int i = 0; i < 6; i++)
    {
        if (Z[i] >= 20) {
            Z[i] = -110;
        }
    }
    if (incX > 0)incX -= incBackFrac; //기울기를 자동으로 돌려준다
    if (incX < 0)incX += incBackFrac;
    if (incY > 0)incY -= incBackFrac;
    if (incY < 0)incY += incBackFrac;
    if (incZ > 0)incZ -= incBackFrac;
    if (incZ < 0)incZ += incBackFrac;

    speed += 0.0002;
    if (speed >= 1)speed = 1;
    // 별 충돌탐지, 점수 올리기
    if (checkCollision()) {
        if (temp - uZ > 5 || uZ -temp >5) {
            score++;
            temp = uZ;
        }
    }
}

void titleText(char* text, int x, int y, int z) //제목
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
void startText(char* text, int x, int y, int z) //시작안내문
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

void drawStrokeText(const char* str, int x, int y, int z) //UFO의 좌표, 점수 표시
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

    if (start) { //시작하면 
        glPushMatrix();
        glTranslated(0, 0, 0);
        glScaled(3, 3, 3);
        draw();
        glPopMatrix();
        string t = to_string(score);
        drawStrokeText(("Score: " + t).c_str(), 3, 0, 0); //점수 표시
        string ufoCoordText = "UFO Coordinates: (" + to_string(uX) + ", " + to_string(uY) + ", " + to_string(uZ) + ")";
        drawStrokeText(ufoCoordText.c_str(), 3, -1, 0); //UFO 좌표 표시
    }
    else { //메뉴화면
        glClearColor(0, 0, 0, 1);
        titleText("The UFO game", -1, -1, 0); //제목
        startText("let's start with 'q'", -1, -1, 0); //q를 누르면 시작
    }
    glColor3d(0, 0, 0);
   
    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y) //키 입력 힘수
{
    float frac = 0.3; //상하
    float incFrac = 1; //앞뒤각도
    switch (key)
    {
    case 'z': //z를 누르면 종료
        exit(0);
        break;
    case 'w':  //위로 가기
        uY -= frac;
        incZ += incFrac;
        break;
    case 's': //아래로 가기
        uY += frac;
        incZ -= incFrac;
        break;
    case 'a': //왼쪽 이동
        uX += frac;
        incX -= incFrac * 2;
        incY += incFrac / 2;
        break;
    case 'd': //오른쪽 이동
        uX -= frac;
        incX += incFrac * 2;
        incY -= incFrac / 2;
        break;
    case 'q': //q를 누르면 시작
        start = true;
        break;
    case 'e': //e를 누르면 메뉴화면
        start = false;
        break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

const GLfloat l_ambient[] = { 0, 0, 0, 1.0 }; //빛광원
const GLfloat l_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat l_specular[] = { 1, 1, 1, 1.0 };
const GLfloat l_position[] = { 5, 5, 5, 0.2 };

const GLfloat m_ambient[] = { 0.6, 0.6, 0.6, 1.0 }; //물체표면
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

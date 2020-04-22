#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <iostream>
#include <stdlib.h>

boolean fscreen = false;
boolean checkJump = false;
boolean checkCap = false;
boolean CheckKey[256];
double SpeedCap=20;
double Speed = 0;
double rt = 0;
boolean cC = false;
int CoinPos = 1;
int Score=0;

int Square[] = { 100,500, 120,145,
400,1400,250,275,
0,380,400,425,
1300,1800,400,425,
1000,1400,600,625,
200,1350,800,825,};
int Stype[] = { 1, 2, 1, 2, 1, 3};
int Coin[] = { 1800,20, 400,845, 1300,645, 40,460, 300,170};
const double TWO_PI = 6.2831853;

GLsizei winWidth = 1920, winHeight = 1080;
GLuint regHex;
static GLfloat rotTheta = 1.0;

double i = 20;
double j = 200;
double alpha = 1.0;
double g = 10;

class scrPt
{
public:
	GLint x, y;
};

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, 1920.0, 0.0, 1080.0, -1.0, 1.0);

	scrPt hexVertex;
	GLdouble hexTheta;
	GLint k;
	double m = 1;
	glClearColor(1.0, 1.0, 1.0, 1.0);
	regHex = glGenLists(1);
	glNewList(regHex, GL_COMPILE);
	
	glBegin(GL_POLYGON);
	for (k = 0; k < 36; k++)
	{
		hexTheta = TWO_PI * k / 36;
		hexVertex.x = 20 * cos(hexTheta);
		hexVertex.y =  20 * sin(hexTheta);
		m = m - 0.04;
		glColor3f(m, m, 0.5);
		glVertex2i(hexVertex.x, hexVertex.y);
	}
	glEnd();
	glEndList();
}

void Scene(void)
{
	
	glClear(GL_COLOR_BUFFER_BIT);
	for (int l = 0; l < 6; l++)
	{
		glBegin(GL_POLYGON);
		if (Stype[l] == 1)
			glColor3f(1, 1, 0);
		if (Stype[l] == 2)
			glColor3f(0, 0, 0);
		if (Stype[l] == 3)
			glColor3f(0, 1, 1);
		glVertex3f(Square[l*4], Square[l*4+2], 0.0);
		glVertex3f(Square[l*4+1], Square[l*4+2], 0.0);
		glVertex3f(Square[l*4+1], Square[l*4+3], 0.0);
		glVertex3f(Square[l*4], Square[l*4+3], 0.0);
		glEnd();
	}
	glEnable(GL_POINT_SMOOTH);
	glPointSize(30);
	glBegin(GL_POINTS);
		glColor3f(1, 0.8, 0);
		glVertex3f(Coin[CoinPos*2], Coin[CoinPos*2 + 1], 0);
	glEnd();
	glDisable(GL_POINT_SMOOTH);
	glPushMatrix();
	glTranslated(i, j, 0.0);
	glRotated(rt, 0, 0, 1);
	glCallList(regHex);
	glPopMatrix();
	glutSwapBuffers();
	glFlush();
}

void reshape(int w, int h) 
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1920.0, 0.0, 1080.0, -1.0, 1.0);  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void gravity(void)
{
	if (j - g > 20)
		j = j - g;
	else
		j = 20;
	glutPostRedisplay();
}

//STANGA
void left(double multiplier){
	SpeedCap *= multiplier;
	if (Speed <= SpeedCap)
		Speed += 0.5 * multiplier;
	if (i >= 20)
		if (i - Speed * multiplier < 20) 
			i = 20;
		else 
			i -= Speed * multiplier;
	rt += Speed * multiplier;
	glutPostRedisplay();
}

//DREAPTA
void right(double multiplier) {
	SpeedCap *= multiplier;
	if (Speed <= SpeedCap)
		Speed += 0.5 * multiplier;
	if (i <= 1900)
		if (i + Speed * multiplier > 1900)
				i = 1900;
		else 
			i += Speed * multiplier;
		rt -= Speed*multiplier;
	glutPostRedisplay();
}

//JUMP-LEFT
void JumpLeft(double multiplier) {
	SpeedCap *= multiplier;
	if (Speed <= SpeedCap)
		Speed += 0.5 * multiplier;
	for (double power = 20; power >= 0; power--) {
		if (j + power >= 1060)
		{
			power = -26;
			j = 1060;
		}
		else
			j = j + power;
		if (i - 7 < 20)
			i = 20;
		else
			i -= 7;
		Scene();
		glutPostRedisplay();
	}
}

//JUMP-RIGHT
void JumpRight(double multiplier) {
	SpeedCap *= multiplier;
	if (Speed <= SpeedCap)
		Speed += 0.5 * multiplier;
	for (double power = 20; power >= 0; power--) {
		if (j + power >= 1060){
			j = 1060; power = -26;
		}else
			j += power;
		if (i + 7 > 1900)
			i = 1900;
		else
			i += 7;
		rt -= 7;
		Scene();
		glutPostRedisplay();
	}
}

//JUMP
void jump(double multiplier) {
	for (double power = 20; power > 0; power--){
		checkJump = true;
		j = j + power;
		Scene();
		glutPostRedisplay();
		checkJump = false;
	}
}


void checkCollision() {
	for (int l = 0; l < 6; l++) {
		if (i + 21 > Square[l * 4] && i - 21 < Square[l * 4 + 1] && j + 21 > Square[l * 4 + 2] && j - 21 < Square[l * 4 + 3]) {
			if (j - 21 < Square[l * 4 + 3]) {
				g = 0;
				j = Square[l * 4 + 3] + 20;
			}
			l = 6;
		}else{
			g = 10;
		}
		gravity();
	}
	if (i + 21 > Coin[CoinPos * 2] && i - 21 < Coin[CoinPos * 2] && j + 21 > Coin[CoinPos * 2 + 1] && j - 21 < Coin[CoinPos * 2 + 1]) {
		double cC=rand()%5;
		while (CoinPos == cC)
			cC = rand() % 5;
		CoinPos = cC;
		Score++;
		system("cls");
		std::cout << "Score"<< " "<<Score<<'\n';
		
	}
}

void keyboard(unsigned char key, int x, int y)
{
	CheckKey[int(key)] = true;
	//LEFT
	if (CheckKey[int('a')] && !CheckKey[int(' ')] && !CheckKey[int('d')])
		left(1);	

	//JUMP
	if (CheckKey[int(' ')] && !CheckKey[int('d')] && !CheckKey[int('a')]) {
		if(!checkJump)
			jump(1);
	}

	//RIGHT
	if (CheckKey[int('d')] && !CheckKey[int(' ')] && !CheckKey[int('a')])
		right(1);
	//JUMP RIGHT
	if (CheckKey[int('d')] && CheckKey[int(' ')] && !CheckKey[int('a')]) {
		JumpRight(1);
	}
	//JUMP LEFT
	if (!CheckKey[int('d')] && CheckKey[int(' ')] && CheckKey[int('a')]) {
		JumpLeft(1);
	}
	//FULLSCREEN
	if(CheckKey[int('f')])
		if(!fscreen){ 
			glutFullScreen();
			fscreen = true;
		}else {
			glutPositionWindow(0, 0);
			glutReshapeWindow(1920, 1080);
			fscreen = false;
		}
	glutPostRedisplay();
}
void keyboardUp(unsigned char key, int x, int y)
{
	CheckKey[int(key)]=false;
	if (!CheckKey[int(' ')])
		checkJump = false;
	if (!CheckKey[int('d')])
		Speed = 0;
	if (!CheckKey[int('a')])
		Speed = 0;
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Platform Game");
	init();
	glutDisplayFunc(Scene);
	glutReshapeFunc(reshape);
	glutIdleFunc(gravity);
	glutIdleFunc(checkCollision);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutMainLoop();
}
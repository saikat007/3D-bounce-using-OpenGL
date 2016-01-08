#include <windows.h>
#include <cmath>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <iostream>
#include "drawing.h"
#include "game.h"

using namespace std;
bool fullscreen = true;

float lastx=0, lasty=0;
float angle=0.0,deltaAngle = 0.0,ratio;
float x=73.0f,y=5.75f,z=-5.0f;
float lx=0.5f,ly=0.0f,lz=-1.0f;


int deltaMove = 0;
void moveMeFlat(int i);
void orientMe(float ang);


bool isLeftKeyPressed = false;
bool isRightKeyPressed = false;
bool isUpKeyPressed = false;
bool isDownKeyPressed = false;
bool isSpaceBarPressed = false;
float Time_of_jump = 25;
int tstangle = 0;


drawing *Drawing;

void changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	ratio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the clipping volume
	 gluPerspective(60.0f,(GLdouble)w/(GLdouble)h,1.0f,1250.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, y, z,
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);


}
drawing::~drawing()
{
    delete[] Drawing;
}

void initScene()
{

	glClearColor(0, 0, 0, 1.0f);							// Black Background
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);		// Really Nice Perspective Calculations
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
}
void orientMe(float ang) {


	lx = sin(ang);
	lz = -cos(ang);
	glLoadIdentity();
	gluLookAt(x, y, z,
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);
}


void moveMeFlat(int i) {
	x = x + i*(lx)*0.1;
	if(x<2)
	{
		x=2;
	}
	else if(x>124)
	{
		x=124;
	}
	z = z + i*(lz)*0.1;
	if(z>-2)
	{
		z=-2;
	}
	else if (z<-125*3)
	{
		z=-125*3;
	}
	glLoadIdentity();
	gluLookAt(x, y, z,
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);
}

void renderScene(void) {


	if(isLeftKeyPressed)
		Drawing->checkleftkey(tstangle);
	if(isRightKeyPressed)
		Drawing->checkrightkey(tstangle);
	if(isUpKeyPressed)
		Drawing->checkupkey(tstangle);
	if(isDownKeyPressed)
		Drawing->checkdownkey(tstangle);




	if (deltaMove)
		moveMeFlat(deltaMove);
	if (deltaAngle) {
		angle += deltaAngle;
		orientMe(angle);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	GLfloat lightColor[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat lightPos[] = {-0.2f, 0.3f, -1, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	Drawing->draw(tstangle);

}



void mouseMovement(int x, int y){

	 int diffx=x-lastx; //check the difference between the current x and the last x position

    lastx=x; //set lastx to the current x position
    angle += (float) diffx;
    orientMe(angle/200);
    tstangle = x/1365.0 * 360;

}
void pressKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT:
			isLeftKeyPressed = true;
		break;
		case GLUT_KEY_RIGHT :
			isRightKeyPressed = true;
		break;
		case GLUT_KEY_UP :
			deltaMove = 1;
			isUpKeyPressed = true;
			break;
		case GLUT_KEY_DOWN :
			deltaMove = -1;
			isDownKeyPressed = true;
			break;
	}
}
void update(int value)
{
	if(isSpaceBarPressed){
		Drawing->step(isSpaceBarPressed);}

	glutTimerFunc(Time_of_jump, update, 4);
	glutPostRedisplay();

}
void releaseKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT :isLeftKeyPressed = false;
		case GLUT_KEY_RIGHT :
			deltaAngle = 0.0f;
			isRightKeyPressed = false;
			break;
		case GLUT_KEY_UP :isUpKeyPressed = false;
		case GLUT_KEY_DOWN :
			deltaMove = 0;
			isDownKeyPressed = false;
			break;
	}
}
void processNormalKeys(unsigned char key, int x, int y) {

	switch(key){
	case 32:
		isSpaceBarPressed = true;
		PlaySound("sample.wav", NULL, SND_ASYNC);
	break;
	case 27:
		exit(0);
		break;
	}
}

void drawing::callBallConstructor()
{
		ball = new Ball();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,500);
	glutCreateWindow("bounce 3D");

	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=false;
	}

    if(fullscreen)
        glutFullScreen();

	initScene();
	Drawing= new drawing();
	Drawing->callBallConstructor();

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
    glutPassiveMotionFunc(mouseMovement);
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);

	glutReshapeFunc(changeSize);
    glutTimerFunc(Time_of_jump, update, 4);

	glutMainLoop();

	return(0);
}

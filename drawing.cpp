#include <windows.h>
#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include "drawing.h"
#include "loadBMP.h"
#include "game.h"

using namespace std;



namespace{
    GLfloat GREEN[] = {0, 1, 0};

	//The number of points used to approximate a circle in the barrier model
	const int NUM_BARRIER_POINTS = 30;
	//The number of points used to approximate a circle in the pole model
	const int NUM_POLE_POINTS = 6;
	//The height of the barrier model
	const float BARRIER_HEIGHT =42.0f;
	const float	BARRIER_SIZE=5.0f;
	//The radius of the pole model
	const float POLE_RADIUS = 0.75f;
	//The height of the center of a pole above the ground
	const float POLE_HEIGHT = 0.0f;
	const int slices = 16;
	const int stacks = 16;

	//amount of rotation of the ball
	float SpinAngle = 0.0f;

	//spin about x axis
	float SpinX;

	//spin anout z axis
	float SpinZ;







	GLuint loadBMP(Image* image)
	{

	GLuint textureId;
	//textureId=0;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
	}

}
drawing::drawing():CELL(126),PI(3.1415926535f),CELL2(126),POS(20.2)//Gamestart(true)
{
	srand(time(NULL));
	for(int i=0;i<6;i++)
		for(int j=0;j<6;j++)
			ran_pos[i][j]=rand()%15;

	//startwindow();
	drawRoom();
	setupGate();
	setupPole();
	drawcube();
	//drawFood();

	time_until_next_step = 0;
}
void drawing:: drawRoom()
	{

    Image* image1=loadBMP("floor.bmp");
	_floortextureID=loadBMP(image1);
	delete image1;

	Image* image2=loadBMP("water.bmp");
	_secondfloortextureID=loadBMP(image2);
	delete image2;

    Image* image3=loadBMP("wall.bmp");
	_walltextureID=loadBMP(image3);
	delete image3;


	Image* image4=loadBMP("celling.bmp");
	_cellingtextureID=loadBMP(image4);
	delete image4;
    roomDisplayList=glGenLists(1);							// Generate Lists
    glNewList(roomDisplayList,GL_COMPILE);

	// First Floor
	glEnable ( GL_TEXTURE_2D );

	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glBindTexture(GL_TEXTURE_2D,_floortextureID);//_floorwatertextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);
    glTexCoord2d(0,0);  glVertex3d(0,0,0);
    glTexCoord2d(20,0);  glVertex3d(CELL,0,0);
    glTexCoord2d(20,20);  glVertex3d(CELL,0,-CELL);
    glTexCoord2d(0,20);  glVertex3d(0,0,-CELL);
    glEnd();

	//second Floor
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glBindTexture(GL_TEXTURE_2D,_secondfloortextureID);//_floorwatertextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);
    glTexCoord2d(0,0);  glVertex3d(0,0,-CELL);
    glTexCoord2d(20,0);  glVertex3d(CELL,0,-CELL);
    glTexCoord2d(20,20);  glVertex3d(CELL,0,-CELL*3);
    glTexCoord2d(0,20);  glVertex3d(0,0,-CELL*3);
    glEnd();



	// Ceilling

	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glBindTexture(GL_TEXTURE_2D, _cellingtextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glBegin(GL_QUADS);
    glTexCoord2d(0,0);   glVertex3d(0,CELL/3,0);
    glTexCoord2d(2,0);  glVertex3d(CELL,CELL/3,0);
    glTexCoord2d(2,2);  glVertex3d(CELL,CELL/3,-CELL*3);
    glTexCoord2d(0,2);  glVertex3d(0,CELL/3,-CELL*3);
    glEnd();
/////////////////////////Wall Start///////////////////////////////////


	 //Back wall
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glBindTexture(GL_TEXTURE_2D, _walltextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glBegin(GL_QUADS);
    glTexCoord2d(0,0);  glVertex3d(0,0,0);
    glTexCoord2d(0,1);  glVertex3d(0,CELL/3,0);
    glTexCoord2d(4,1);  glVertex3d(CELL,CELL/3,0);
    glTexCoord2d(4,0);  glVertex3d(CELL,0,0);
    glEnd();

    //Left side Wall
	glBegin(GL_QUADS);
    glTexCoord2d(0,0);  glVertex3d(0,0,0);
    glTexCoord2d(0,1);  glVertex3d(0,CELL/3,0);
    glTexCoord2d(4,1);  glVertex3d(0,CELL/3,-CELL*3);
    glTexCoord2d(4,0);  glVertex3d(0,0,-CELL*3);
    glEnd();

    //right side wall
    glBegin(GL_QUADS);
    glTexCoord2d(0,0);glVertex3d(CELL,0,0);
    glTexCoord2d(0,1);glVertex3d(CELL,CELL/3,0);
    glTexCoord2d(4,1);glVertex3d(CELL,CELL/3,-CELL*3);
    glTexCoord2d(4,0);glVertex3d(CELL,0,-CELL*3);
    glEnd();

	//First middle wall
	glBegin(GL_QUADS);
	glTexCoord2d(0,0);  glVertex3d(0,0,-CELL);
    glTexCoord2d(2,0);  glVertex3d(CELL/3,0,-CELL);
    glTexCoord2d(2,1);  glVertex3d(CELL/3,CELL/3,-CELL);
    glTexCoord2d(0,1);  glVertex3d(0,CELL/3,-CELL);
    glEnd();


	glBegin(GL_QUADS);
	glTexCoord2d(0,0);  glVertex3d(126-CELL/3,0,-CELL);
    glTexCoord2d(2,0);  glVertex3d(CELL,0,-CELL);
    glTexCoord2d(2,1);  glVertex3d(CELL,CELL/3,-CELL);
    glTexCoord2d(0,1);  glVertex3d(126-CELL/3,CELL/3,-CELL);
    glEnd();



	//second middle wall


	glBegin(GL_QUADS);
	glTexCoord2d(0,0);  glVertex3d(0,0,-CELL*2);
    glTexCoord2d(2,0);  glVertex3d(2*CELL/3,0,-CELL*2);
    glTexCoord2d(2,1);  glVertex3d(2*CELL/3,CELL/3,-CELL*2);
    glTexCoord2d(0,1);  glVertex3d(0,CELL/3,-CELL*2);
    glEnd();



	//last middle wall
	glBegin(GL_QUADS);
	glTexCoord2d(0,0);  glVertex3d(0,0,-CELL*3);
    glTexCoord2d(2,0);  glVertex3d(CELL/3,0,-CELL*3);
    glTexCoord2d(2,1);  glVertex3d(CELL/3,CELL/3,-CELL*3);
    glTexCoord2d(0,1);  glVertex3d(0,CELL/3,-CELL*3);
    glEnd();


	glBegin(GL_QUADS);
	glTexCoord2d(0,0);  glVertex3d(126-CELL/3,0,-CELL*3);
    glTexCoord2d(2,0);  glVertex3d(CELL,0,-CELL*3);
    glTexCoord2d(2,1);  glVertex3d(CELL,CELL/3,-CELL*3);
    glTexCoord2d(0,1);  glVertex3d(126-CELL/3,CELL/3,-CELL*3);
    glEnd();
	glEndList();
	glDisable(GL_TEXTURE_2D);


}
void drawing::setupGate() {



	GLuint gateDisplayList_temp = glGenLists(1);
	glNewList(gateDisplayList_temp, GL_COMPILE);

	//Draw the top circle
	//glColor3f(0.8f,0.1f,0.4f);
	glNormal3f(0, 1, 0);
	glBegin(GL_TRIANGLE_FAN);
	//glTexCoord2f(0.5f, 0.5f);
	glVertex3f(0, BARRIER_HEIGHT, 0);
	for(int i = NUM_BARRIER_POINTS; i >= 0; i--) {
		float angle = 2 * PI * (float)i / (float)NUM_BARRIER_POINTS;
		//glTexCoord2f(-cos(angle) / 2 + 0.5f, sin(angle) / 2 + 0.5f);
		glVertex3f(BARRIER_SIZE * cos(angle),
				   BARRIER_HEIGHT,
				   BARRIER_SIZE * sin(angle));
	}
	glEnd();

	//Draw the bottom circle
	glNormal3f(0, -1, 0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 0);
	for(int i = 0; i <= NUM_BARRIER_POINTS; i++) {
		float angle = 2 * PI * (float)i / (float)NUM_BARRIER_POINTS;
		glVertex3f(BARRIER_SIZE * cos(angle), 0, BARRIER_SIZE * sin(angle));
	}
	glEnd();



	//Draw the cylinder part
	glBegin(GL_QUAD_STRIP);
	//glTexCoord2f(0.0f,0.0f);
	for(int i = 0; i <= NUM_BARRIER_POINTS; i++) {
		float angle = 2 * PI * ((float)i - 0.5f) / (float)NUM_BARRIER_POINTS;
		glNormal3f(cos(angle), 0, sin(angle));
		float angle2 = 2 * PI * (float)i / (float)NUM_BARRIER_POINTS;
		//glTexCoord2f(-cos(angle2) / 2 + 0.5f, sin(angle2) / 2 + 0.5f);
		glVertex3f(BARRIER_SIZE * cos(angle2), 0, BARRIER_SIZE * sin(angle2));
		//glTexCoord2f(-cos(angle) / 2 + 0.5f, sin(angle) / 2 + 0.5f);
		glVertex3f(BARRIER_SIZE * cos(angle2),
				   BARRIER_HEIGHT,
				   BARRIER_SIZE * sin(angle2));
	}
	glEnd();
	//glPopMatrix();
	glEndList();




	//Make a display list with four copies of the barrier
	gateDisplayList = glGenLists(1);
	glNewList(gateDisplayList, GL_COMPILE);
	//glDisable(GL_COLOR_MATERIAL);

	//Add a little specularity
	GLfloat materialSpecular[] = {1, 1, 1, 1};
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 15.0f);

	//for(int i = 1;i< 4; i++) {
			glPushMatrix();
			glTranslatef(5+CELL/3,0.01f,-CELL);
			glCallList(gateDisplayList_temp);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(121-CELL/3,0.01f,-CELL);
			glCallList(gateDisplayList_temp);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(5+2*CELL/3,0.01f,-CELL*2);
			glCallList(gateDisplayList_temp);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(CELL-5,0.01f,-CELL*2);
			glCallList(gateDisplayList_temp);
			glPopMatrix();


			glPushMatrix();
			glTranslatef(5+CELL/3,0.01f,-CELL*3);
			glCallList(gateDisplayList_temp);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(121-CELL/3,0.01f,-CELL*3);
			glCallList(gateDisplayList_temp);
			glPopMatrix();



	//glEnable(GL_COLOR_MATERIAL);

	//Disable specularity
	//GLfloat materialColor3[] = {1, 1, 1, 1};
	//GLfloat materialSpecular2[] = {0, 0, 0, 1};
	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor3);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular2);

	glEndList();
	//glDisable(GL_COLOR_MATERIAL);
}
void drawing::setupPole()
{

	GLuint poleDisplayList_temp = glGenLists(1);
	glNewList(poleDisplayList_temp, GL_COMPILE);
	//glDisable(GL_TEXTURE_2D);
	//glPushMatrix();
	//glColor3f(0.0f, 0.8f, 0.0f);

	//Draw the left circle
	glNormal3f(-1, 0, 0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(BARRIER_SIZE+10, POLE_HEIGHT, -POLE_RADIUS);
	for(int i = NUM_POLE_POINTS; i >= 0; i--) {
		float angle = 2 * PI * (float)i / (float)NUM_POLE_POINTS;
		glVertex3f(BARRIER_SIZE+10,
				   POLE_HEIGHT + POLE_RADIUS * cos(angle),
				   POLE_RADIUS * (sin(angle) - 1));
	}
	glEnd();

	//Draw the right circle
	glNormal3f(1, 0, 0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(1 - (BARRIER_SIZE+10), POLE_HEIGHT, -POLE_RADIUS);
	for(int i = 0; i <= NUM_POLE_POINTS; i++) {
		float angle = 2 * PI * (float)i / (float)NUM_POLE_POINTS;
		glVertex3f(1 - (BARRIER_SIZE+10),
				   POLE_HEIGHT + POLE_RADIUS * cos(angle),
				   POLE_RADIUS * (sin(angle) - 1));
	}
	glEnd();

	//Draw the cylinder part
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i <= NUM_POLE_POINTS; i++) {
		float angle = 2 * PI * ((float)i - 0.5f) / (float)NUM_POLE_POINTS;
		glNormal3f(0, cos(angle), sin(angle));
		float angle2 = 2 * PI * (float)i / (float)NUM_POLE_POINTS;
		glVertex3f(1 - (BARRIER_SIZE+10),
				   POLE_HEIGHT + POLE_RADIUS * cos(angle2),
				   POLE_RADIUS * (sin(angle2) - 1));
		glVertex3f(BARRIER_SIZE+10,
				   POLE_HEIGHT + POLE_RADIUS * cos(angle2),
				   POLE_RADIUS * (sin(angle2) - 1));
	}
	glEnd();
	glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();
	glEndList();

	poleDisplayList=glGenLists(1);
	glNewList(poleDisplayList, GL_COMPILE);
	//glPushMatrix();
	//glDisable(GL_COLOR_MATERIAL);

	for(float i=5;i<42;i+=5)
	{
		glPushMatrix();
	//glColor3f(1.0f,0.0f,0.0f,1.0f)
		glTranslatef(63.0f,i,-CELL);
	//glTranslatef(63.0f,0.0f,-126.0f);
		glCallList(poleDisplayList_temp);
		glPopMatrix();
	}
	for(float i=5;i<42;i+=5)
	{
		glPushMatrix();
		glTranslatef(105.0f,i,-CELL*2);
		glCallList(poleDisplayList_temp);
		glPopMatrix();
	}
	for(float i=5;i<42;i+=5)
	{
		glPushMatrix();
		glTranslatef(63.0f,i,-CELL*3);
		glCallList(poleDisplayList_temp);
		glPopMatrix();
	}
	//glPopMatrix();
	glEndList();

	//glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);


}
void drawing::drawcube()
{

		//glDisable(GL_COLOR_MATERIAL);
		Image* image1=loadBMP("cube.bmp");
		_cubetextureID=loadBMP(image1);
		delete image1;

		GLuint cubeDisplayList_temp= glGenLists(1);
		glNewList(cubeDisplayList_temp, GL_COMPILE);
		glEnable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glBindTexture(GL_TEXTURE_2D,_cubetextureID);//_floorwatertextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



		glPushMatrix();
		glScalef(2.0f,2.0f,2.0f);
		glBegin(GL_QUADS);

        glTexCoord2d(0.0f,0.0f);
        glVertex3f(2.0f,0.0f,0.0f);
        glTexCoord2d(1.0f,0.0f);
        glVertex3f(0.0f,0.0f,0.0f);
        glTexCoord2d(1.0f,1.0f);
        glVertex3f(0.0f,0.0f,-2.0f);
        glTexCoord2d(0.0f,1.0f);
        glVertex3f(2.0f,0.0f,-2.0f);


       // glBegin(GL_QUADS);
        glTexCoord2d(0.0f,0.0f);
        glVertex3f(2.0f,0.0f,0.0f);
        glTexCoord2d(0.0f,1.0f);
        glVertex3f(0.0f,0.0f,0.0f);
        glTexCoord2d(1.0f,1.0f);
        glVertex3f(0.0f,2.0f,0.0f);
        glTexCoord2d(1.0f,0.0f);
        glVertex3f(2.0f,2.0f,0.0f);


        glTexCoord2d(0.0f,0.0f);
        glVertex3f(0.0f,0.0f,0.0f);
        glTexCoord2d(0.0f,1.0f);
        glVertex3f(0.0f,2.0f,0.0f);
        glTexCoord2d(1.0f,1.0f);
        glVertex3f(0.0f,2.0f,-2.0f);
        glTexCoord2d(1.0f,0.0f);
        glVertex3f(0.0f,0.0f,-2.0f);

    //right side
    //glColor3f(0.0f, 1.0f, 1.0f);
        glTexCoord2d(0.0f,0.0f);
        glVertex3f(2.0f,0.0f,0.0f);
        glTexCoord2d(1.0f,0.0f);
        glVertex3f(2.0f,0.0f,-2.0f);
        glTexCoord2d(1.0f,1.0f);
        glVertex3f(2.0f,2.0f,-2.0f);
        glTexCoord2d(0.0f,1.0f);
        glVertex3f(2.0f,2.0f,0.0f);


    //back side
    //glColor3f(0.0f, 1.0f, 0.0f);
        glTexCoord2d(0.0f,0.0f);
        glVertex3f(0.0f,0.0f,-2.0f);
        glTexCoord2d(1.0f,0.0f);
        glVertex3f(2.0f,0.0f,-2.0f);
        glTexCoord2d(1.0f,1.0f);
        glVertex3f(2.0f,2.0f,-2.0f);
        glTexCoord2d(0.0f,1.0f);
        glVertex3f(0.0f,2.0f,-2.0f);


    //top side
        //glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2d(0.0f,0.0f);
        glVertex3f(0.0f,2.0f,0.0f);
        glTexCoord2d(1.0f,0.0f);
        glVertex3f(0.0f,2.0f,-2.0f);
        glTexCoord2d(1.0f,1.0f);
        glVertex3f(2.0f,2.0f,-2.0f);
        glTexCoord2d(0.0f,1.0f);
        glVertex3f(2.0f,2.0f,0.0f);
        glEnd();
        glPopMatrix();
		glEndList();


		cubeDisplayList= glGenLists(1);
		glNewList(cubeDisplayList, GL_COMPILE);
		glPushMatrix();
		glTranslatef(25.0f,0.01f,-25.0f);
		glScalef(1.0f,0.5f,0.5f);
		glCallList(cubeDisplayList_temp);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(25.0f,0.01,-27.0f);
		//glScalef(2.0f,2.0f,2.0f);
		glCallList(cubeDisplayList_temp);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(25.0f,0.01,-31.0f);
		glScalef(1.0f,2.0f,2.0f);
		glCallList(cubeDisplayList_temp);
		glPopMatrix();

		for(int k=1;k<3;k++)
		{
		for(int i=1;i<6;i++)
		{
			for(int j=1;j<6;j++)
			{
			glPushMatrix();
			//glScalef(2.0f,2.0f,2.0f);
			glTranslatef(POS*i+ran_pos[i][j],2+ran_pos[i][j],-(k*CELL+POS*j)+ran_pos[i][j]);
			glCallList(cubeDisplayList_temp);
			glPopMatrix();
			}
		}
		}
		glPushMatrix();
		glTranslatef(63.0f,2.0f,-CELL-5);
		glCallList(cubeDisplayList_temp);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(105.0f,2.0f,-CELL*2-5);
		glCallList(cubeDisplayList_temp);
		glPopMatrix();
		glEndList();
		glDisable(GL_TEXTURE_2D);


    }

void drawing::drawFood()
{
		foodDisplayList= glGenLists(1);
		glNewList(foodDisplayList, GL_COMPILE);
		glPushMatrix();
		//glColor3f(1.0f,0,0);
		glTranslatef(25.0f,10.0f,-31.0f);
		//glutSolidTorus(0.2,0.8,slices,stacks)
		//glDisable(GL_COLOR_MATERIAL);
		glPopMatrix();
		glEndList();


}


void drawing::draw(float angle)
{
	glClearColor(0.7f, 0.9f, 1.0f, 1);
	glCallList(roomDisplayList);
	glCallList(gateDisplayList);
	glCallList(poleDisplayList);
	glCallList(cubeDisplayList);
	drawBall(angle);
	glutSwapBuffers();

}


void drawing::drawBall(float angle)
{
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, GREEN);
	glTranslatef(ball->x(), ball->y(), ball->z());
	glRotatef(-SpinAngle, SpinX, 0, SpinZ);
	glutSolidSphere(ball->radius(), 50, 50);
	glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();
}

void drawing::checkrightkey(float angle)
{
    SpinAngle ++;
    SpinX = cos(angle);
	ball->checkrightkey(angle);
}

void drawing::checkleftkey(float angle)
{
	ball->checkleftkey(angle);
}

void drawing::checkupkey(float angle)
{
	ball->checkupkey(angle);
}

void drawing::checkdownkey(float angle)
{
	ball->checkdownkey(angle);
}

void drawing::step(bool & value)
{
	ball->stepup(value);
}


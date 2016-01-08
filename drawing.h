#include <GL/glut.h>
#include <iostream>
//#include <stdio.h>
//#include <math.h>
#include "loadBMP.h"
class Ball;
typedef struct
{
int X;
int Y;
int Z;
double U;
double V;
}VERTICES;


class drawing
{
private:
	Ball *ball;


	GLuint _cellingtextureID;
	GLuint _walltextureID;
	GLuint _floortextureID;
	GLuint _secondfloortextureID;
	GLuint _cubetextureID;
	GLuint _gatetextureID;
	GLint CELL;
	GLint poleDisplayList;
	GLint gateDisplayList;
	GLint roomDisplayList;
	GLint cubeDisplayList;
	GLint foodDisplayList;
	GLuint menuDisplayList;

	const int PI;
	GLfloat CELL2;
	GLfloat POS;
	int ran_pos[6][6];
	bool Gamestart;

	float time_until_next_step;

	void startwindow();
	void drawRoom();
	void drawBall(float);
	void setupPole();
	void setupGate();
	void drawcube();
	void drawFood();
	//void drawGate();
	void drawPole_and_Gate();
	void setupLighting();
	//int random_num(int);
public:
	drawing();
	void callBallConstructor();
	void draw(float);
	~drawing();

	void checkleftkey(float);
	void checkrightkey(float);
	void checkupkey(float );
	void checkdownkey(float);
	void step(bool &);
};

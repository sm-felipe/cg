// A Simple OpenGL Project
// Author: Michael Hall
//
// This C++ code and project are provided "as is" without warranty of any kind.
//
// Copyright 2010 XoaX - For personal use only, not for distribution
#include "Cube.h"
#include <glut.h>
#include <math.h>

// ESTADOS //
int JOGANDO = 0;
int DESCENDO = 1;
int VOLTANDO =  2;
int SOLTANDO = 3;

int estado = 0;

// POSICAO DA GARRA //
float _x;
float _y = 5.0f;
float _z;
Cube* cube = new Cube(1.0f);

// OBJETOS //
//TODO: fazer um vetor com os objetos

// OUTROS //
float velocidade = 0.0007f;
int windWidth = 640;
int windHeight = 480;



typedef struct PONTO {
	float x;
	float y;
	float z;
};

void updateCubePos(PONTO* mouse, PONTO* object, float cubeLateralSize){
	float halfSide = cubeLateralSize /2.0f;

	//halfSide só funciona pra cubos, preciso receber esses dados melhor
	float objTop = object->y + halfSide;
	float objBottom = object->y - halfSide;
	float objLeft = object->x - halfSide;
	float objRight = object->x + halfSide;
	/*float objFront = object->z + halfSide;
	float objBack = object->z - halfSide; */

	float newX = object->x;
	float newY = object->y;
	/*float newZ = object->z;*/

	
	BOOL verticalInvasion = mouse->y < objTop && mouse->y > objBottom;
	BOOL horizontalInvasion = mouse->x > objLeft && mouse->x < objRight;
	/*BOOL depthInvasion = mouse->z > objBack && mouse->z < objFront;*/


	BOOL hasInvasion = verticalInvasion && horizontalInvasion/* && depthInvasion*/;

	if(hasInvasion){

		float topDistance = abs(objTop - mouse->y);
		float bottomDistance = abs(objBottom - mouse->y);
		float leftDistance = abs(mouse->x - objLeft);
		float rightDistance = abs( objRight - mouse->x);
		/*float frontDistance = abs(objFront - mouse->z);
		float backDistance = abs(objBack - mouse->z);*/

		BOOL deCima = topDistance < bottomDistance && topDistance < leftDistance && topDistance < rightDistance/* && topDistance < frontDistance && topDistance < backDistance*/;
		BOOL deBaixo = bottomDistance < topDistance && bottomDistance < leftDistance && bottomDistance < rightDistance/* && bottomDistance < frontDistance && bottomDistance < backDistance*/;
		BOOL daDireita = rightDistance < topDistance && rightDistance < bottomDistance && rightDistance < leftDistance /*&& rightDistance < frontDistance && rightDistance < backDistance*/;
		BOOL daEsquerda = leftDistance < topDistance && leftDistance < bottomDistance && leftDistance < rightDistance /*&& leftDistance < frontDistance && leftDistance < backDistance*/;
		/*BOOL daFrente = frontDistance < topDistance && frontDistance < bottomDistance && frontDistance < rightDistance && frontDistance < leftDistance && frontDistance < backDistance;
		BOOL deTras =  backDistance < topDistance && backDistance < bottomDistance && backDistance < rightDistance && backDistance < leftDistance && backDistance < frontDistance;*/
		
		if(deCima){
			newY = newY - (objTop - mouse->y);
		}
		if(deBaixo){
			newY = newY + (mouse->y - objBottom);
		}
		if(daDireita){
			newX = newX - (objRight - mouse->x);
		}
		if(daEsquerda){
			newX = newX + (mouse->x - objLeft);
		}
		/*if(daFrente){
			newZ = newZ - (objFront - mouse->z);
		}
		if(deTras){
			newZ = newZ + (mouse->z - objBack);
		}*/

	}

	object->x = newX;
	object->y = newY;
	/*object->z = newZ;*/
}

void changeCameraPos(){
	glLoadIdentity();
	gluLookAt(0.0f, 5.0f, -8.0,
			0.0f, 0.0f,  0.0f,
			0.0f, 1.0f,  0.0f);
}

void detectaColisao(){
	//TODO implementar um while que itera pelos objetos em busca de colisão. O chão também é um objeto
	
	bool haColisao = _y <= 1.0f;

	if(haColisao){
		//pega objeto
		estado = VOLTANDO;
	}

}

void desceCubo(){
	if(_y >= 1.0f){
		_y -= 1 * velocidade;
	}
}

void voltaCuboEObjeto(){
	if(_y < 5.0f){
		_y += velocidade;
	}else if(_x < 5.2f){
		_x += velocidade;
	}else if(_z > -6.0f){
		_z -= velocidade;
	}else{
		estado = JOGANDO;
	}
}

void Draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer

	
	changeCameraPos();

	if(estado == JOGANDO){//trocar por switch
		cube->move(_x, _y, _z);
	}else if(estado == DESCENDO){
		desceCubo();
		cube->move(_x, _y, _z);
		detectaColisao();
	}else if(estado == VOLTANDO){
		voltaCuboEObjeto();
		cube->move(_x, _y, _z);
	}
	cube->draw();

	


	changeCameraPos();
	glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-6, 0, -6);
		glVertex3f(-6, 0, 6);
		glVertex3f(6, 0, 6);
		glVertex3f(6, 0, -6);
	glEnd();

	glFlush();
}



void Initialize(int width, int height) {
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0, 0.0, 0.0, 0.5f);					// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	glTranslatef(0.0f, 0.0f, -8.0);
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
}

void processMouseClick(int button, int state, int x, int y) {
	if(estado == JOGANDO){
		if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
			estado = DESCENDO;
		}
	}
}

void processMouseMotion(int x, int z) {
	x -= windWidth / 2;
	z -= windHeight/ 2;
	
	if(estado == JOGANDO){
		_x = (windHeight/35.0f - x) /35.0f;
		_z = (windHeight/35.0f - z) /35.0f;
	}
}

int main(int iArgc, char** cppArgv) {
	

	glutInit(&iArgc, cppArgv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB |GLUT_DEPTH);
	glutInitWindowSize(windWidth, windHeight);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("CG");
	Initialize(windWidth, windHeight);
	glutDisplayFunc(Draw);
	glutIdleFunc(Draw);

	glutMouseFunc(processMouseClick);
	glutPassiveMotionFunc(processMouseMotion);
	glutMotionFunc(processMouseMotion);
	glutMainLoop();
	return 0;
}

/*
#define RED 1
#define GREEN 2
#define BLUE 3
#define WHITE 4

float angleX = 0.0;
float angle = 0.0, deltaAngle = 1.0;
float red=1.0, blue=1.0, green=1.0;
float width,height;
int specialKey;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	// we're storing these values for latter use in the 
	// mouse motion functions
	width = w;
	height = h;
	float ratio = 1.0* w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0,0.0,5.0, 
		      0.0,0.0,-1.0,
			  0.0f,1.0f,0.0f);


}

void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(angle,0.0,1.0,0.0);
	glRotatef(angleX,1.0,0.0,0.0);
	glColor3f(red,green,blue);

	glBegin(GL_TRIANGLES);
		glVertex3f(-0.5,-0.5,0.0);
		glVertex3f(0.5,0.0,0.0);
		glVertex3f(0.0,0.5,0.0);
	glEnd();
	glPopMatrix();
	angle+=deltaAngle;
	glutSwapBuffers();
}

void processMouse(int button, int state, int x, int y) {


	specialKey = glutGetModifiers();
	// if both a mouse button, and the ALT key, are pressed  then
	if ((state == GLUT_DOWN) && (specialKey == GLUT_ACTIVE_ALT)) {

		// set the color to pure red for the left button
		if (button == GLUT_LEFT_BUTTON) {
			red = 1.0; green = 0.0; blue = 0.0;
		}
		// set the color to pure green for the middle button
		else if (button == GLUT_MIDDLE_BUTTON) {
			red = 0.0; green = 1.0; blue = 0.0;
		}
		// set the color to pure blue for the right button
		else {
			red = 0.0; green = 0.0; blue = 1.0;
		}
	}
}

void processMouseActiveMotion(int x, int y) {

	// the ALT key was used in the previous function
	if (specialKey != GLUT_ACTIVE_ALT) {
		// setting red to be relative to the mouse 
		// position inside the window
		if (x < 0)
			red = 0.0;
		else if (x > width)
			red = 1.0;
		else
			red = ((float) x)/height;
		// setting green to be relative to the mouse 
		// position inside the window
		if (y < 0)
			green = 0.0;
		else if (y > width)
			green = 1.0;
		else
			green = ((float) y)/height;
		// removing blue from the color.
		blue = 0.0;
	}
}

void processMousePassiveMotion(int x, int y) {

	// User must press the SHIFT key to change the 
	// rotation in the X axis
	if (specialKey != GLUT_ACTIVE_SHIFT) {

		// setting the angle to be relative to the mouse 
		// position inside the window
		if (x < 0)
			angleX = 0.0;
		else if (x > width)
			angleX = 180.0;
		else
			angleX = 180.0 * ((float) x)/height;
	}
}

void processMouseEntry(int state) {
	if (state == GLUT_LEFT)
		deltaAngle = 0.0;
	else
		deltaAngle = 1.0;
}

void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27) 
		exit(0);
}

void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("SnowMen@Lighthouse3D");
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(processNormalKeys);

	//adding here the mouse processing callbacks
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutPassiveMotionFunc(processMousePassiveMotion);
	glutEntryFunc(processMouseEntry);
	glutMainLoop();
}*/
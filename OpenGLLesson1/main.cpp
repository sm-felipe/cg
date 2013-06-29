// A Simple OpenGL Project
// Author: Michael Hall
//
// This C++ code and project are provided "as is" without warranty of any kind.
//
// Copyright 2010 XoaX - For personal use only, not for distribution


#include "Cube.h"
#include <glut.h>
#include <math.h>

#define PI 3.14159265





// ESTADOS //
const int JOGANDO = 0;
const int DESCENDO = 1;
const int VOLTANDO =  2;
const int SOLTANDO = 3;
int estado = 0;

// POSICAO DA GARRA //
float _x;
float _y = 5.0f;
float _z;


float oldX = 0;
float oldY = 0;

// CUBOS PARA DEBUG //
Cube* esqCube = new Cube(0.1f);
Cube* dirCube = new Cube(0.1f);


// OBJETOS //
//TODO: objetos deveria ser objects[x][3] onde 
//[x][1] é o topo da bounding box
//[x][2] é o baixo da bounding box
//[x][3] é o objeto propriamente dito
Cube* objects[1];

//COLISÃO
int indiceColisao = -1;
bool leftTouch = false;
bool rightTouch = false;



// controle da camera
float angle = 180;
float hipotenusa = 8;
float velocidadeRotacao = 0.1;
bool rotateCam = false;


// OUTROS //
float velocidade = 0.0007f;
int windWidth = 720;
int windHeight = 540;
float yMin = 1.4f;

int sizeOfObjects(){
	return sizeof(objects) / sizeof(Cube*);
}


bool haColisao(PONTO* pontaGarra, PONTO* objCenter, float cubeLateralSize){
	float halfSide = cubeLateralSize /2.0f;

	//halfSide só funciona pra cubos, preciso receber esses dados melhor
	float objTop = objCenter->y + halfSide;
	float objBottom = objCenter->y - halfSide;
	float objLeft = objCenter->x - halfSide;
	float objRight = objCenter->x + halfSide;
	float objFront = objCenter->z + halfSide;
	float objBack = objCenter->z - halfSide; 

/*	float newX = objCenter->x;
	float newY = objCenter->y;
	float newZ = objCenter->z;*/

	BOOL verticalInvasion = pontaGarra->y < objTop && pontaGarra->y > objBottom;
	BOOL horizontalInvasion = pontaGarra->x > objLeft && pontaGarra->x < objRight;
	BOOL depthInvasion = pontaGarra->z > objBack && pontaGarra->z < objFront;

	BOOL hasInvasion = verticalInvasion && horizontalInvasion && depthInvasion;

	if(hasInvasion){
		return true;
	}

	return false;
}



void detectaColisao(){
		
	int i = 0;
	Cube* obj;

	bool leftColidiu = false;//colisao esquerda
	for(i; i < sizeOfObjects(); i++){
		obj = objects[i];
		leftColidiu = haColisao(esqCube->getPos(), obj->getPos(), obj->getEdgeSize());
		if(leftColidiu){
			break;
		}
	}

	bool rightColidiu = false;//colisao direita
	rightColidiu = haColisao(dirCube->getPos(), obj->getPos(), obj->getEdgeSize());
	if(leftColidiu && rightColidiu){
		indiceColisao = i;
	}

	bool bateuChao = _y <= yMin;

	bool haColisao = bateuChao || leftColidiu || rightColidiu ;

	if(haColisao){
		estado = VOLTANDO;
	}
}

void voltaCuboEObjeto(){
	Cube* moveObj = new Cube(0);
	bool colisao = indiceColisao != -1;

	if(colisao){
		moveObj = objects[indiceColisao];
	}

	PONTO* objPos = moveObj->getPos();

	if(_y < 5.0f){
		_y += velocidade;
		if(colisao)	moveObj->move(objPos->x, _y - moveObj->halfEdgeSize, objPos->z);
	}else if(_x < 5.2f){
		_x += velocidade;
		if(colisao) moveObj->move(_x, objPos->y, objPos->z);
	}else if(_z > -6.0f){
		_z -= velocidade;
		if(colisao) moveObj->move(objPos->x, objPos->y, _z);
	}else if(moveObj->getPos()->y > -10.0f && colisao){
		if(colisao) moveObj->move(objPos->x, objPos->y - velocidade, objPos->z);
	}else{
		estado = JOGANDO;
		indiceColisao = -1;
	}
}











void changeCameraPos(){
	glLoadIdentity();
	
	float x = sin(angle * PI / 180.0) * hipotenusa;
	float z = cos(angle * PI / 180.0) * hipotenusa;
	
	gluLookAt(x,5.0f, z,
		0.0f, 0.0f,  0.0f,
		0.0f, 1.0f,  0.0f);

	/*gluLookAt(5.0f, 2, -5,
		0.0f, 2.0f,  0.0f,
		0.0f, 1.0f,  0.0f);*/
}

void atualizaAnguloCamera(){
	if(rotateCam){
		if(angle < 270){
			angle += velocidadeRotacao;
		}
	}else{
		if(angle > 180){
			angle -= velocidadeRotacao;
		}
	}
}

void getZKey(unsigned char key, int x, int y) {
	if (key == 122) 
		rotateCam = true;
}

void releasedZKey(unsigned char key, int x, int y){
	if (key == 122) 
		rotateCam = false;
}

void desceCubo(){
	if(_y >= yMin){
		_y -= 1 * velocidade;
	}
}

void drawGarra(){

	//draw center
	changeCameraPos();
	glColor3f(0.7,0.7,0.7);
	glTranslatef(_x, _y + 0.5, _z);
	glRotatef(90.0, 1, 0, 0);
    glBegin(GL_POLYGON);
		GLUquadricObj *obj = gluNewQuadric();
		gluCylinder(obj, 0.6, 0.6, 0.5, 30, 30);
    glEnd();

	float coneHeight = 1;

	//draw left
	changeCameraPos();
	glColor3f(1,0.7,0.7);
	glTranslatef(_x + 1.2, _y - 0.4, _z);
	glRotatef(270.0, 1, 0, 0);
	glRotatef(-50.0, 0, 1, 0);
	glutSolidCone(0.2, coneHeight, 30, 30);

	changeCameraPos();
	glTranslatef(_x + 1.2, _y - 0.3, _z);
	glRotatef(270.0, 1, 0, 0);
	glRotatef(-180.0, 0, 1, 0);
	glutSolidCone(0.2, coneHeight, 30, 30);
	esqCube->move(_x + 1.2, _y - 0.3 - coneHeight, _z);//DEBUG deveria ser um ponto

	//draw right
	changeCameraPos();
	glColor3f(1,0.7,0.7);
	glTranslatef(_x - 1.2, _y - 0.4, _z);
	glRotatef(-90.0, 1, 0, 0);
	glRotatef(50.0, 0, 1, 0);
	glutSolidCone(0.2, coneHeight, 30, 30);

	changeCameraPos();
	glTranslatef(_x - 1.2, _y - 0.3, _z);
	glRotatef(-90.0, 1, 0, 0);
	glRotatef(180.0, 0, 1, 0);
	glutSolidCone(0.2, coneHeight, 30, 30);
	dirCube->move(_x - 1.2, _y - 0.3 - coneHeight, _z);//DEBUG deveria ser um ponto
}

void Draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	
	atualizaAnguloCamera();

	drawGarra();

	changeCameraPos();
	switch (estado)	{
		case JOGANDO:
			break;

		case DESCENDO:
			desceCubo();
			detectaColisao();
			break;

		case VOLTANDO:
			voltaCuboEObjeto();
			break;

		default:
			break;
	}

	//objetos
	for(Cube* cubo : objects){
		changeCameraPos();
		cubo->draw();
	}

	//chao
	changeCameraPos();
	glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-6, 0, -6);
		glVertex3f(-6, 0, 6);
		glVertex3f(6, 0, 6);
		glVertex3f(6, 0, -6);
	glEnd();

	//debug
	changeCameraPos();
	esqCube->draw();

	changeCameraPos();
	dirCube->draw();

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

	float deltax = 0;
	float deltaz = 0;

	
	if(estado == JOGANDO){
			deltax = ((windWidth/40.0f - x) / 40.0f) - _x;
			deltaz = ((windHeight/40.0f - z) / 40.0f) - _z;

			_x += deltax;
			_z += deltaz;
	}
}

void initializeObjects(){
	objects[0] = new Cube(2.78);
	objects[0]->move(3, 0.3f, -3);
	//TODO adicionar outros objetos.
}


int main(int iArgc, char** cppArgv) {

	initializeObjects();	
	
	glutInit(&iArgc, cppArgv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB |GLUT_DEPTH);
	glutInitWindowSize(windWidth, windHeight);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("CG");
	Initialize(windWidth, windHeight);
	glutDisplayFunc(Draw);
	glutIdleFunc(Draw);

	glutKeyboardFunc(getZKey);
	glutKeyboardUpFunc(releasedZKey);
	glutMouseFunc(processMouseClick);
	glutPassiveMotionFunc(processMouseMotion);

	glutMotionFunc(processMouseMotion);
	glutMainLoop();
	FreeConsole();
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
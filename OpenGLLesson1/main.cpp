#include <Windows.h>
#include "ObjectClasses.h"
#include <stdio.h>
#include <math.h>
#include "SOIL.h"

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

float yMin = 1.4f;

float velocidadeMov = 0.005f;

float garraAngle = 0;
float garraHipotenusa = 1.2f;
float garraVelocidadeRotacao = 0.2f;

Cube* esqCube = new Cube(0.1f);
Cube* dirCube = new Cube(0.1f);

// OBJETOS //
const int numObjetos = 4;
Parallelepiped* boxes[numObjetos];
Shape* shapes[numObjetos];


//COLISÃO //
int indiceColisao = -1;
bool leftTouch = false;
bool rightTouch = false;


// CONTROLE DA CAMERA //
float angle = 180;
float hipotenusa = 8;
float velocidadeRotacao = 0.2;
bool rotateCam = false;

//TEXTURA
GLuint      chao;
GLuint		wood;
GLuint		metal;

//AMBIENTE//
Parallelepiped* pilares[4];

//LUZ //
GLfloat LightAmbient[]= { 0.4f, 0.4f, 0.2f, 1.0f };
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f }; 
GLfloat LightPosition[]={ 0.0f, 6.0f, -3.0f, 1.0f };


// OUTROS //
int windWidth = 720;
int windHeight = 540;


int sizeOfBoxes(){
	return sizeof(boxes) / sizeof(Cube*);
}

int loadTextures(){

	chao = SOIL_load_OGL_texture("floor.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if(chao == 0)
		return false;
	glBindTexture(GL_TEXTURE_2D, chao);

	wood = SOIL_load_OGL_texture("wood.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if(wood == 0)
		return false;
	glBindTexture(GL_TEXTURE_2D, wood);

	metal = SOIL_load_OGL_texture("metal.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if(metal == 0)
		return false;
	glBindTexture(GL_TEXTURE_2D, metal);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    return true; 
}

bool haColisao(PONTO* pontaGarra, Parallelepiped* box){
	float xHalfSize = box->getWidth()/2.0f;
	float yHalfSize = box->getHeight()/2.0f;
	float zHalfSize = box->getDepth()/2.0f;

	PONTO* objCenter = box->getPos();

	float objTop = objCenter->y + yHalfSize;
	float objBottom = objCenter->y - yHalfSize;
	float objLeft = objCenter->x - xHalfSize;
	float objRight = objCenter->x + xHalfSize;
	float objFront = objCenter->z + zHalfSize;
	float objBack = objCenter->z - zHalfSize; 

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
	
	int leftI = 0;
	Parallelepiped* leftBox;
	bool leftColidiu = false;
	for(leftI; leftI < sizeOfBoxes(); leftI++){
		leftBox = boxes[leftI];
		leftColidiu = haColisao(esqCube->getPos(), leftBox);
		if(leftColidiu){
			break;
		}
	}

	int rightI = 0;
	Parallelepiped* rightBox;
	bool rightColidiu = false;
	for(rightI; rightI < sizeOfBoxes(); rightI++){
		rightBox = boxes[rightI];
		rightColidiu = haColisao(dirCube->getPos(), rightBox);
		if(rightColidiu){
			break;
		}
	}

	if(leftColidiu && rightColidiu && rightI == leftI){
		indiceColisao = rightI;
	}

	bool bateuChao = _y <= yMin;

	bool haColisao = bateuChao || leftColidiu || rightColidiu ;

	if(haColisao){
		estado = VOLTANDO;
	}
}

void centralizaObj(){
	if(indiceColisao != -1){
		boxes[indiceColisao]->move(_x, _y, _z);
	}
}

void changeCameraPos(){
	glLoadIdentity();
	
	float x = sin(angle * PI / 180.0) * hipotenusa;
	float z = cos(angle * PI / 180.0) * hipotenusa;
	
	gluLookAt(x,5.0f, z,
		0.0f, 0.0f,  0.0f,
		0.0f, 1.0f,  0.0f);
}

void voltaGarraEObjeto(){
	Parallelepiped* moveObj = new Parallelepiped(0, 0 ,0);
	bool colisao = indiceColisao != -1;
	PONTO* objCenter;

	if(colisao){
		moveObj = boxes[indiceColisao];
		objCenter = moveObj->getPos();
	}

	if(_y < 5.0f){
		_y += velocidadeMov;
		if(colisao)	moveObj->move(objCenter->x, _y - (moveObj->getHeight()/2.0f), objCenter->z );
	}else if(_x < 4.3f){
		_x += velocidadeMov;
		if(colisao) moveObj->move(_x, objCenter->y, objCenter->z);
	}else if(_z > -7.8f){
		_z -= velocidadeMov;
		if(colisao) moveObj->move(objCenter->x, objCenter->y, _z);
	}else if(moveObj->getPos()->y > -10.0f && colisao){
		if(colisao) moveObj->move(objCenter->x, objCenter->y - velocidadeMov, objCenter->z);
	}else{
		estado = JOGANDO;
		shapes[indiceColisao]->show = false;
		indiceColisao = -1;
	}
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

void desceGarra(){
	if(_y >= yMin){
		_y -= 1 * velocidadeMov;
	}
}

void atualizaAnguloGarra(){
	if(estado == VOLTANDO){
		garraAngle += garraVelocidadeRotacao;
	}else{
		if( (garraAngle / 360.0 ) > 0){
			garraAngle += garraVelocidadeRotacao;

			if(((int) garraAngle) % 360 == 0){
				garraAngle = 0;
			}
		}
	}
}

void drawGarra(){
	
	//draw center
	changeCameraPos();
	glColor3f(0.7,0.7,0.7);
	glTranslatef(_x, _y + 0.5, _z);
	glRotatef(90.0, 1, 0, 0);
    glBegin(GL_POLYGON);
		glBindTexture(GL_TEXTURE_2D, metal);
		GLUquadricObj *obj = gluNewQuadric();
		gluQuadricTexture(obj, 1);
		gluCylinder(obj, 0.6, 0.6, 0.5, 40, 30);
    glEnd();

	float coneHeight = 1;
	float xRotate = cos(garraAngle * PI / 180.0) * garraHipotenusa;
	float zRotate = sin(garraAngle * PI / 180.0) * garraHipotenusa;

	//draw left
	changeCameraPos();
	glColor3f(0.7,0.7,0.7);
	glTranslatef(_x + xRotate, _y - 0.4, _z + zRotate);
	glRotatef(-garraAngle, 0, 1, 0);
	glRotatef(270.0, 1, 0, 0);
	glRotatef(-50.0, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, -1);
	glutSolidCone(0.2, coneHeight, 30, 30);

	changeCameraPos();
	glTranslatef(_x + xRotate, _y - 0.3, _z + zRotate);
	glRotatef(270.0, 1, 0, 0);
	glRotatef(-180.0, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, -1);
	glutSolidCone(0.2, coneHeight, 30, 30);
	esqCube->move(_x + xRotate, _y - 0.3 - coneHeight, _z + + zRotate);

	//draw right
	changeCameraPos();
	glColor3f(0.7,0.7,0.7);
	glTranslatef(_x - xRotate, _y - 0.4, _z - zRotate);
	glRotatef(-garraAngle, 0, 1, 0);
	glRotatef(-90.0, 1, 0, 0);
	glRotatef(50.0, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, -1);
	glutSolidCone(0.2, coneHeight, 30, 30);

	changeCameraPos();
	glTranslatef(_x - xRotate, _y - 0.3, _z - zRotate);
	glRotatef(-90.0, 1, 0, 0);
	glRotatef(180.0, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, -1);
	glutSolidCone(0.2, coneHeight, 30, 30);
	dirCube->move(_x - xRotate, _y - 0.3 - coneHeight, _z - zRotate);
}


void drawEnvironment(){
	//chao
	glBindTexture(GL_TEXTURE_2D, chao);
	glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);glVertex3f(-6, 0, -6);
		glTexCoord2f(1.0f, 0.0f);glVertex3f(-6, 0, 6);
		glTexCoord2f(1.0f, 1.0f);glVertex3f(6, 0, 6);
		glTexCoord2f(0.0f, 1.0f);glVertex3f(6, 0, -6);
	glEnd();

	//pilares
	changeCameraPos();
	pilares[0]->move( 6.2, 5, 6.2);
	pilares[0]->setTexture(wood);
	pilares[0]->draw();

	changeCameraPos();
	pilares[1]->move( 6.2, 5,-6.2);
	pilares[1]->setTexture(wood);
	pilares[1]->draw();

	changeCameraPos();
	pilares[2]->move(-6.2, 5, 6.2);
	pilares[2]->setTexture(wood);
	pilares[2]->draw();

	changeCameraPos();
	pilares[3]->move(-6.2, 5,-6.2);
	pilares[3]->setTexture(wood);
	pilares[3]->draw();

	//fio
	changeCameraPos();
	glTranslatef(7, 6.3, -6.2);
	glRotatef(-45.0, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, metal);
    glBegin(GL_POLYGON);
		GLUquadricObj *obj = gluNewQuadric();
		gluQuadricTexture(obj, 1);
		gluCylinder(obj, 0.06, 0.06, 17, 20, 20);
    glEnd();

	//túnel de saída
	changeCameraPos();
	glTranslatef(4.2, -5, -8);

	float x = 2;
	float y = 5;
	float z = 2;

	glBegin(GL_QUADS);
		// Front Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, -y,  z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( x, -y,  z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( x,  y,  z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,  y,  z);
		// Back Face
		glNormal3f( 0.0f, 0.0f,-1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y, -z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y, -z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y, -z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y, -z);
		// Right face
		glNormal3f( 1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( x, -y, -z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( x,  y, -z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y,  z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y,  z);
		// Left Face
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, -y, -z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y,  z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y,  z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,  y, -z);
	glEnd();
}

void drawLightSource(){
	GLUquadricObj* Sphere;
	glBindTexture(GL_TEXTURE_2D, -1);
	glTranslatef(LightPosition[0],LightPosition[1] ,LightPosition[2] +3.7);
	Sphere=gluNewQuadric();
	gluSphere(Sphere,0.3,30,30);
	gluDeleteQuadric(Sphere);
}

void Draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	atualizaAnguloCamera();
	atualizaAnguloGarra();

	drawGarra();

	changeCameraPos();
	switch (estado)	{
		case JOGANDO:
			break;

		case DESCENDO:
			desceGarra();
			detectaColisao();
			centralizaObj();
			break;

		case VOLTANDO:
			voltaGarraEObjeto();
			break;

		default:
			break;
	}

	//objetos
	for(int i = 0; i < sizeOfBoxes(); i++){
		if(shapes[i]->show){
			changeCameraPos();
			Parallelepiped* parall = boxes[i];
			if(estado == VOLTANDO && i == indiceColisao){
				parall->setAngle(-garraAngle);
			}
			//parall->draw(); para ver as bounding box
			parall->calculateDraw();
			shapes[i]->draw(0);
		}
	}

	changeCameraPos();
	drawEnvironment();

	changeCameraPos();
	drawLightSource();

	changeCameraPos();
	esqCube->draw();

	changeCameraPos();
	dirCube->draw();

	glFlush();
}

bool Initialize(int width, int height) {
	if(!loadTextures()){
		return false;
	}else{
		esqCube->setTexture(metal);
		dirCube->setTexture(metal);
	}

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);							
	glClearColor(0.0, 0.0, 0.0, 0.5f);					
	glClearDepth(1.0f);									
	glEnable(GL_DEPTH_TEST);							
	glDepthFunc(GL_LEQUAL);								
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	
	glEnable(GL_LIGHT1);								
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	glTranslatef(0.0f, 0.0f, -8.0);
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();

	return true;
}

GLvoid adjustSize(GLsizei width, GLsizei height){
	if (height==0){
		height=1;									
	}

	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();									
	glViewport(0,0,width,height);						
	float ratio = 1.0* width / height;
	gluPerspective(45.0f,ratio,0.1f,100.0f);
	glTranslatef(0.0f, 0.0f, -8.0);
	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();									

	windWidth = width;
	windHeight = height;
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

	float minX = -4.8f;
	float maxX = 4.8f;
	float minZ = -6.0f;
	float maxZ = 6.0f;
	
	if(estado == JOGANDO){
			float conversionFactor = 13.5f;

			float xMouseFactor = windWidth / conversionFactor;
			float zMouseFactor = windHeight / conversionFactor;

			deltax = ((windWidth/xMouseFactor - x) / xMouseFactor) - _x;
			deltaz = ((windHeight/zMouseFactor - z) / zMouseFactor) - _z;

			float tempX = _x + deltax;
			float tempZ = _z + deltaz;

			if(tempX >= minX && tempX <= maxX){
				_x = tempX;
			}
			if(tempZ >= minZ && tempZ <= maxZ){
				_z = tempZ;
			}
	}
}

void initializeObjects(){
	Shape* consts = new Shape(0);

	boxes[0] = new Parallelepiped(1.5, 3, 1.95);
	boxes[0]->move(3, 0.75, -3);
	shapes[0] = new Shape(consts->TEAPOT);

	boxes[1] = new Parallelepiped(2.8, 2.8, 2.8);
	boxes[1]->move(-3, 1.38, -4);
	shapes[1] = new Shape(consts->SPHERE);

	boxes[2] = new Parallelepiped(2.7, 2.7, 2.7);
	boxes[2]->move(-3, 1.38, 4);
	shapes[2] = new Shape(consts->CUBE);

	boxes[3] = new Parallelepiped(3.2, 3.2, 3.2);
	boxes[3]->move(3, 1.6, 3);
	shapes[3] = new Shape(consts->CONE);
}

void initializaEnvironment(){
	for(int i = 0; i < (sizeof(pilares) / sizeof(Parallelepiped*)); i++){
		pilares[i] = new Parallelepiped(10, 0.5, 0.5);
	}
}

int main(int iArgc, char** cppArgv) {

	initializeObjects();	
	initializaEnvironment();
	
	glutInit(&iArgc, cppArgv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB |GLUT_DEPTH);
	glutInitWindowSize(windWidth, windHeight);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("CG");
	if(!Initialize(windWidth, windHeight)){
		return 1;
	}
	glutDisplayFunc(Draw);
	glutIdleFunc(Draw);
	glutReshapeFunc(adjustSize);

	glutKeyboardFunc(getZKey);
	glutKeyboardUpFunc(releasedZKey);
	glutMouseFunc(processMouseClick);
	glutPassiveMotionFunc(processMouseMotion);

	glutMotionFunc(processMouseMotion);
	glutMainLoop();
	FreeConsole();
	return 0;
}
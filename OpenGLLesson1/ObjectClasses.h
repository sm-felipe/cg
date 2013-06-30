#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <glut.h>

struct PONTO {
	float x;
	float y;
	float z;
};

class ObjectClass {

public:
	void move(float x, float y, float z);

	void draw();

};


class Cube : public ObjectClass
{
public:
	float x;
	float y;
	float z;

	Cube(float edgeSize);


	void move(float x, float y, float z);

	void getPos(float* x, float* y, float* z);

	PONTO* getPos();

	float getEdgeSize();

	void setTexture(GLuint textureID);

	void draw();

private:
	float halfEdgeSize;
	float edgeSize;
	GLuint textureID;


};

class Parallelepiped : public ObjectClass
{
public:

	PONTO* center;
	

	Parallelepiped(PONTO* start, PONTO* end);

	Parallelepiped(float height, float width, float depth);

	void move(float x, float y, float z);

	void getPos(float* x, float* y, float* z);

	PONTO* getPos();

	float getHeight();

	float getWidth();

	float getDepth();

	void setAngle(float angle);

	void draw();

	void calculateDraw();

private:
	float height;
	float width;
	float depth;
	float angle;
	
};

class Garra : ObjectClass
{

public:

	float x;
	float y;
	float z;

	Garra(float x, float y, float z, void (*callback)(void));

	void move(float x, float y, float z);

	PONTO* getPos();

	PONTO* getLeft();

	PONTO* getRight();

	void draw();

private:
	
	PONTO* left;
	PONTO* right;
	void (*changeCameraFunc)();

};

class Shape : ObjectClass {
public:

	Shape(int type);

	int type;

	bool show;

	void draw(GLfloat angle);

	const static int TEAPOT = 1;
	const static int CUBE = 2;
	const static int SPHERE = 3;
	const static int CONE = 4;


private:
	
};
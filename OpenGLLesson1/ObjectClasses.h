#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

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

	void draw();

private:
	float height;
	float width;
	float depth;

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
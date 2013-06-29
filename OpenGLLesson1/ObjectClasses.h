#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

struct PONTO {
	float x;
	float y;
	float z;
};

class ObjectClass {

};


class Cube : ObjectClass
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

	void draw();

private:
	float halfEdgeSize;
	float edgeSize;

};

class Parallelepiped : ObjectClass
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
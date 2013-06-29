#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

typedef struct PONTO {
	float x;
	float y;
	float z;
};

class Cube
{
public:
	float halfEdgeSize;
	float edgeSize;
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

};
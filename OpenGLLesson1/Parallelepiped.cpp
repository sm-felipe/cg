#include "ObjectClasses.h"
#include <math.h>

Parallelepiped::Parallelepiped(PONTO* start, PONTO* end){
	height = abs(start->y - end->y);
	width = abs(start->x - end->x);
	depth = abs(start->z - end->z);

	this->center = new PONTO();
	center->x = width/2.0f + start->x;
	center->y = height/2.0f + start->y;
	center->z = depth/2.0f + start->z;
}

Parallelepiped::Parallelepiped(float height, float width, float depth){
	this->height = height;
	this->width = width;
	this->depth = depth;

	this->center = new PONTO();
	center->x = 0;
	center->y = 0;
	center->z = 0;
}

void Parallelepiped::move(float x, float y, float z){
	center->x = x;
	center->y = y;
	center->z = z;
}

void Parallelepiped::getPos(float* x, float* y, float* z){
	*x = center->x;
	*y = center->y;
	*z = center->z;
}

PONTO* Parallelepiped::getPos(){
	return center;
}

float Parallelepiped::getHeight(){
	return height;
}

float Parallelepiped::getWidth(){
	return width;
}

float Parallelepiped::getDepth(){
	return depth;
}

void Parallelepiped::draw(){

	float x = width / 2.0f;
	float y = height / 2.0f;
	float z = depth / 2.0f;

	//glLoadIdentity();							// Reset The Current Modelview Matrix
	glTranslatef(center->x, center->y, center->z);
	glBegin(GL_QUADS);							// Draw A Quad								
		glColor3f(0.0f,1.0f,0.0f);				// Set The Color To Green
		glVertex3f( x, y,-z);					// Top Right Of The Quad (Top)
		glVertex3f(-x, y,-z);					// Top Left Of The Quad (Top)
		glVertex3f(-x, y, z);					// Bottom Left Of The Quad (Top)
		glVertex3f( x, y, z);					// Bottom Right Of The Quad (Top)
		glColor3f(1.0f,0.5f,0.0f);				// Set The Color To Orange
		glVertex3f( x,-y, z);					// Top Right Of The Quad (Bottom)
		glVertex3f(-x,-y, z);					// Top Left Of The Quad (Bottom)
		glVertex3f(-x,-y,-z);					// Bottom Left Of The Quad (Bottom)
		glVertex3f( x,-y,-z);					// Bottom Right Of The Quad (Bottom)
		glColor3f(1.0f,0.0f,0.0f);				// Set The Color To Red
		glVertex3f( x, y, z);					// Top Right Of The Quad (Front)
		glVertex3f(-x, y, z);					// Top Left Of The Quad (Front)
		glVertex3f(-x,-y, z);					// Bottom Left Of The Quad (Front)
		glVertex3f( x,-y, z);					// Bottom Right Of The Quad (Front)
		glColor3f(1.0f,1.0f,0.0f);				// Set The Color To Yellow
		glVertex3f( x,-y,-z);					// Top Right Of The Quad (Back)
		glVertex3f(-x,-y,-z);					// Top Left Of The Quad (Back)
		glVertex3f(-x, y,-z);					// Bottom Left Of The Quad (Back)
		glVertex3f( x, y,-z);					// Bottom Right Of The Quad (Back)
		glColor3f(0.0f,0.0f,1.0f);				// Set The Color To Blue
		glVertex3f(-x, y, z);					// Top Right Of The Quad (Left)
		glVertex3f(-x, y,-z);					// Top Left Of The Quad (Left)
		glVertex3f(-x,-y,-z);					// Bottom Left Of The Quad (Left)
		glVertex3f(-x,-y, z);					// Bottom Right Of The Quad (Left)
		glColor3f(1.0f,0.0f,1.0f);				// Set The Color To Violet
		glVertex3f( x, y,-z);					// Top Right Of The Quad (Right)
		glVertex3f( x, y, z);					// Top Left Of The Quad (Right)
		glVertex3f( x,-y, z);					// Bottom Left Of The Quad (Right)
		glVertex3f( x,-y,-z);					// Bottom Right Of The Quad (Right)
	glEnd();									// Done Drawing The Quad
}

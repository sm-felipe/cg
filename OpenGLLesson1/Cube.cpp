#include "Cube.h"

Cube::Cube(float edgeSize){
	this->halfEdgeSize = edgeSize/2.0f;
	x = 0;
	y = 0;
	z = 0;
}

void Cube::move(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}

void Cube::draw(){

	float l = halfEdgeSize;

	glLoadIdentity();							// Reset The Current Modelview Matrix
	glTranslatef(x, y, z);
	glBegin(GL_QUADS);							// Draw A Quad								
		glColor3f(0.0f,1.0f,0.0f);				// Set The Color To Green
		glVertex3f( l, l,-l);					// Top Right Of The Quad (Top)
		glVertex3f(-l, l,-l);					// Top Left Of The Quad (Top)
		glVertex3f(-l, l, l);					// Bottom Left Of The Quad (Top)
		glVertex3f( l, l, l);					// Bottom Right Of The Quad (Top)
		glColor3f(1.0f,0.5f,0.0f);				// Set The Color To Orange
		glVertex3f( l,-l, l);					// Top Right Of The Quad (Bottom)
		glVertex3f(-l,-l, l);					// Top Left Of The Quad (Bottom)
		glVertex3f(-l,-l,-l);					// Bottom Left Of The Quad (Bottom)
		glVertex3f( l,-l,-l);					// Bottom Right Of The Quad (Bottom)
		glColor3f(1.0f,0.0f,0.0f);				// Set The Color To Red
		glVertex3f( l, l, l);					// Top Right Of The Quad (Front)
		glVertex3f(-l, l, l);					// Top Left Of The Quad (Front)
		glVertex3f(-l,-l, l);					// Bottom Left Of The Quad (Front)
		glVertex3f( l,-l, l);					// Bottom Right Of The Quad (Front)
		glColor3f(1.0f,1.0f,0.0f);				// Set The Color To Yellow
		glVertex3f( l,-l,-l);					// Top Right Of The Quad (Back)
		glVertex3f(-l,-l,-l);					// Top Left Of The Quad (Back)
		glVertex3f(-l, l,-l);					// Bottom Left Of The Quad (Back)
		glVertex3f( l, l,-l);					// Bottom Right Of The Quad (Back)
		glColor3f(0.0f,0.0f,1.0f);				// Set The Color To Blue
		glVertex3f(-l, l, l);					// Top Right Of The Quad (Left)
		glVertex3f(-l, l,-l);					// Top Left Of The Quad (Left)
		glVertex3f(-l,-l,-l);					// Bottom Left Of The Quad (Left)
		glVertex3f(-l,-l, l);					// Bottom Right Of The Quad (Left)
		glColor3f(1.0f,0.0f,1.0f);				// Set The Color To Violet
		glVertex3f( l, l,-l);					// Top Right Of The Quad (Right)
		glVertex3f( l, l, l);					// Top Left Of The Quad (Right)
		glVertex3f( l,-l, l);					// Bottom Left Of The Quad (Right)
		glVertex3f( l,-l,-l);					// Bottom Right Of The Quad (Right)
	glEnd();									// Done Drawing The Quad
}
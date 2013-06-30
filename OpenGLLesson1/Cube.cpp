#include "ObjectClasses.h"

Cube::Cube(float edgeSize){
	this->edgeSize = edgeSize;
	this->halfEdgeSize = edgeSize/2.0f;
	x = 0;
	y = 0;
	z = 0;

	this->textureID = -1;
}

void Cube::move(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}

void Cube::getPos(float* x, float* y, float* z){
	*x = this->x;
	*y = this->y;
	*z = this->z;
}

PONTO* Cube::getPos(){
	PONTO* p = new PONTO();
	p->x = this->x;
	p->y = this->y;
	p->z = this->z;
	return p;
}


float Cube::getEdgeSize(){
	return edgeSize;
}

void Cube::setTexture(GLuint textureID){
	this->textureID = textureID;
}

void Cube::draw(){

	float l = halfEdgeSize;
	bool useTexture = textureID != -1;

	//glLoadIdentity();							// Reset The Current Modelview Matrix
	glTranslatef(x, y, z);
		
	if(useTexture){
		glBindTexture(GL_TEXTURE_2D, textureID);

		glBegin(GL_QUADS);
			// Front Face
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-l, -l,  l);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( l, -l,  l);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( l,  l,  l);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-l,  l,  l);
			// Back Face
			glNormal3f( 0.0f, 0.0f,-1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-l, -l, -l);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-l,  l, -l);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( l,  l, -l);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( l, -l, -l);
			// Top Face
			glNormal3f( 0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-l,  l, -l);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-l,  l,  l);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( l,  l,  l);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( l,  l, -l);
			// Bottom Face
			glNormal3f( 0.0f,-1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-l, -l, -l);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( l, -l, -l);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( l, -l,  l);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-l, -l,  l);
			// Right face
			glNormal3f( 1.0f, 0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( l, -l, -l);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( l,  l, -l);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( l,  l,  l);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( l, -l,  l);
			// Left Face
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-l, -l, -l);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-l, -l,  l);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-l,  l,  l);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-l,  l, -l);
		glEnd();
	}else{

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
}
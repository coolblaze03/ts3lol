
#ifndef SKYBOX_H_INCLUDED
#define SKYBOX_H_INCLUDED

#include <math.h>
#include <GL\glut.h>
#include "Vector3.h"
#include "CZAIL.h"


#define SKYFRONT 0						// Give Front ID = 0
#define SKYBACK  1						// Give Back  ID = 1
#define SKYLEFT  2						// Give Left  ID = 2
#define SKYRIGHT 3						// Give Right ID = 3
#define SKYUP    4						// Give Up    ID = 4
#define SKYDOWN  5						// Give Down  ID = 5


class SkyBox{

  public:
    Vector3 position;
    Vector3 size;

    SkyBox();
    void RenderSkyBox();
    void RenderSkyBox(Vector3 CamPos);
    void SetPosition(Vector3 Pos);
    void SetSize(Vector3 Siz);
    void LoadSkyBoxTexture(const char * filename, int location, bool inverttex, bool grayscale);


    private:
     GLuint _SkyBox[6];

};

#endif

SkyBox::SkyBox(){

     position = Vector3(0,0,-4);
     size = Vector3(50,50,50);

}

void SkyBox::SetPosition(Vector3 Pos){
     position = Pos;
}

void SkyBox::SetSize(Vector3 Siz){
     size =  Siz;
}

//Think about loading this in a thread later.
void SkyBox::LoadSkyBoxTexture(const char * filename, int location, bool inverttex = false, bool grayscale = false){
     _SkyBox[location] = CZAIL::BuildTexture(filename, inverttex, grayscale);
}

void SkyBox::RenderSkyBox()
{

    GLfloat width = size.vec[0];
    GLfloat height = size.vec[1];
    GLfloat length = size.vec[2];

    GLfloat x = position.vec[0] - width  / 2;
	GLfloat y = position.vec[1] - height / 2;
	GLfloat z = position.vec[2] - length / 2;
// djoubert187 _at_ hotmail.com
	// Begin DrawSkybox
	glColor4f(1.0, 1.0, 1.0,1.0f);
	//glColor4f(1.0, 1.0, 0.0,1.0f);

	// Save Current Matrix
	glPushMatrix();

	// Second Move the render space to the correct position (Translate)
    //glTranslatef(position.vec[0],position.vec[1],position.vec[2]);

	// First apply scale matrix
	//glScalef(size.vec[0],size.vec[1],size.vec[2]);

	float cz = -0.0f,cx = 1.0f;
	float r = 1.0f;

  	glPushAttrib(GL_ENABLE_BIT);

     // Render the front quad
    glBindTexture(GL_TEXTURE_2D, _SkyBox[SKYFRONT]);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height, z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height, z+length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z+length);
	glEnd();

	// Draw Back side
	glBindTexture(GL_TEXTURE_2D, _SkyBox[SKYBACK]);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height, z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
	glEnd();

	// Draw Left side
	glBindTexture(GL_TEXTURE_2D, _SkyBox[SKYLEFT]);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z+length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z);
	glEnd();

	// Draw Right side
	glBindTexture(GL_TEXTURE_2D, _SkyBox[SKYRIGHT]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height,	z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height,	z);
	glEnd();

	// Draw Up side
	glBindTexture(GL_TEXTURE_2D, _SkyBox[SKYUP]);
	glBegin(GL_QUADS);
		//glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+height, z);
		//glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+height, z+length);
		//glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z+length);
		//glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);

		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+height, z+length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y+height,	z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z);
	glEnd();

	// Draw Down side
	glBindTexture(GL_TEXTURE_2D, _SkyBox[SKYDOWN]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y,		z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y,		z);
	glEnd();

    glPopAttrib();
	// Load Saved Matrix
	glPopMatrix();

}

void SkyBox::RenderSkyBox(Vector3 CamPos)
{

    GLfloat width = size.vec[0];
    GLfloat height = size.vec[1];
    GLfloat length = size.vec[2];

    GLfloat x = CamPos.vec[0] - width  / 2;
	GLfloat y = CamPos.vec[1] - height / 2;
	GLfloat z = CamPos.vec[2] - length / 2;
// djoubert187 _at_ hotmail.com
	// Begin DrawSkybox
	glColor4f(1.0, 1.0, 1.0,1.0f);
	//glColor4f(1.0, 1.0, 0.0,1.0f);

	// Save Current Matrix
	glPushMatrix();

	// Second Move the render space to the correct position (Translate)
    //glTranslatef(position.vec[0],position.vec[1],position.vec[2]);

	// First apply scale matrix
	//glScalef(size.vec[0],size.vec[1],size.vec[2]);

	float cz = -0.0f,cx = 1.0f;
	float r = 1.0f;

  	glPushAttrib(GL_ENABLE_BIT);

     // Render the front quad
    glBindTexture(GL_TEXTURE_2D, _SkyBox[SKYFRONT]);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height, z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height, z+length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z+length);
	glEnd();

	// Draw Back side
	glBindTexture(GL_TEXTURE_2D, _SkyBox[SKYBACK]);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height, z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
	glEnd();

	// Draw Left side
	glBindTexture(GL_TEXTURE_2D, _SkyBox[SKYLEFT]);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z+length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z);
	glEnd();

	// Draw Right side
	glBindTexture(GL_TEXTURE_2D, _SkyBox[SKYRIGHT]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height,	z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height,	z);
	glEnd();

	// Draw Up side
	glBindTexture(GL_TEXTURE_2D, _SkyBox[SKYUP]);
	glBegin(GL_QUADS);
		//glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+height, z);
		//glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+height, z+length);
		//glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z+length);
		//glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);

		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+height, z+length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y+height,	z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z);
	glEnd();

	// Draw Down side
	glBindTexture(GL_TEXTURE_2D, _SkyBox[SKYDOWN]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y,		z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y,		z);
	glEnd();

    glPopAttrib();
	// Load Saved Matrix
	glPopMatrix();

};

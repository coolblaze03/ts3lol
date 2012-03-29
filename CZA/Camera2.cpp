
#include <math.h>
#include <GL/glut.h>
#include "Vector3.h"

class Camera2{

      public:

        float angle,ratio;
        float x,y,z;
        float lx,ly,lz;

        Camera2();

        void orientMe(float ang);
        void moveMeFlat(int direction);
        Vector3 GetPosition();
        void moveVert(int direction);
        void Init();
        void SetPosition(GLdouble xx,GLdouble yy,GLdouble zz);


};

Camera2::Camera2(){

	    angle=0.0;
        x=0.0f;
        y=1.75f;
        z=5.0f;
        lx=0.0f;
        ly=0.0f;
        lz=-1.0f;
        //gluLookAt(x, y, z, x + lx,y + ly,z + lz, 0.0f,1.0f,0.0f);


}

Vector3 Camera2::GetPosition(){
        return Vector3(x, y, z);
}


void Camera2::SetPosition(GLdouble xx,GLdouble yy,GLdouble zz){

 glLoadIdentity();
    glTranslatef(xx,yy,zz);

			  // gluLookAt(x, y, z, x + lx,y + ly,z + lz, 0.0f,1.0f,0.0f);

			   x = xx;
               y = yy;
               z = zz;

}

void Camera2::orientMe(float ang) {

	         lx = sin(ang);
             lz = -cos(ang);
// glLoadIdentity();
            glRotatef(ang,1.0,0.0,1.0);  //rotate our camera on teh
            // glRotatef(yrot,0.0,1.0,0.0);

             angle = ang;



         }

void Camera2::moveMeFlat(int direction) {
        	x = x + direction*(lx)*1;
        	z = z + direction*(lz)*1;
        	glLoadIdentity();
        	glTranslatef(x,y,z);
}

void Camera2::moveVert(int direction) {
        	y += (float)direction;
        	glLoadIdentity();
        	glTranslatef(x,y,z);

}

void Camera2::Init(){


}

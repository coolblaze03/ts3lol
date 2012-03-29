#pragma once

#include <math.h>
#include <GL/glut.h>
#include "Vector3.h"
#include "Terrain.h"
#include "TerrainT.h"
#define HEIGHT_OFFSET   4

class FPSCamera:public Object{

      private:
        float xrot, lastx;
        float yrot, lasty;
        float world_length, world_width;
        Terrain* terrain;
        TerrainT* terraint;
        bool stopmotion;
        void SetCurrentPos(){
            SetCurrentx(x);
            SetCurrenty(y);
            SetCurrentz(z);
        }

        void AfterCollisionDetected(Object* collisionObject){
            stopmotion = true;
        }



      public:

        float angle,ratio, yangle;
        float x,y,z;
        float lx,ly,lz;
        float centerx,centery,centerz;

        FPSCamera(Terrain * terrain);
        FPSCamera(TerrainT * terrain);

        void orientMe(float ang);
        void CirclePoint(float ang);
        void moveMeFlat(int direction);
        void Strafe(int direction);
        void SetPosition(GLdouble eyeX,GLdouble eyeY,GLdouble eyeZ,
			  GLdouble centerX,GLdouble centerY,GLdouble centerZ,
			  GLdouble upX,GLdouble upY,GLdouble upZ );
        Vector3 GetPosition();
        void moveVert(int direction);
        void Init();
        void SetPosition(GLdouble xx,GLdouble yy,GLdouble zz);
        void CursorChange(int x, int y);

        bool mode;

         void ResetLast(){
            lastx = -999;
            lasty = -999;
        }

        float xAngle(){
            return angle;
        }
        float yAngle(){
            return yangle;
        }
        //0 - normal
        //1 - follow



};

FPSCamera::FPSCamera(Terrain* terrain){

	    angle=0.0;
	    yangle = 0.0;
        x=0.0f;
        y=1.75f;
        z=5.0f;

        lx=0.0f;
        ly=0.0f;
        lz=-1.0f;
        centerx = x + lx;
        centery = y + ly;
        centerz = z + lz;
        gluLookAt(x + lx, y + ly, z + lz,
               x ,y ,z,
               0.0f,1.0f,0.0f);
        xrot = 0;
        yrot = 0;
        lastx = -999;
        lasty = -999;
        this->terrain = terrain;
        this->world_width = terrain->width_starting();
        this->world_length = terrain->length_starting();
        stopmotion = false;


}

FPSCamera::FPSCamera(TerrainT* terrain){

	    angle=0.0;
	    yangle = 0.0;
        x=0.0f;
        y=1.75f;
        z=5.0f;
        lx=0.0f;
        ly=0.0f;
        lz=-1.0f;
        centerx = x + lx;
        centery = y + ly;
        centerz = z + lz;
        gluLookAt(x + lx, y + ly, z + lz,
               x ,y ,z,
               0.0f,1.0f,0.0f);
        xrot = 0;
        yrot = 0;
        lastx = -999;
        lasty = -999;
        this->terraint = terrain;
        this->radius0 = 1;

        SetCurrentPos();

}



Vector3 FPSCamera::GetPosition(){
        return Vector3(x, y, z);
}

  /*eyeX,	eyeY, eyeZ
			  Specifies the	position of the	eye point.

	  centerX, centerY, centerZ
			  Specifies the	position of the	reference
			  point.

	  upX, upY, upZ	  Specifies the	direction of the up vector.
*/

void FPSCamera::SetPosition(GLdouble eyeX,
			  GLdouble eyeY,
			  GLdouble eyeZ,
			  GLdouble centerX,
			  GLdouble centerY,
			  GLdouble centerZ,
			  GLdouble upX,
			  GLdouble upY,
			  GLdouble upZ ){

      gluLookAt(eyeX,eyeY,eyeZ,
			   centerX,centerY,centerZ,
			   upX,upY,upZ);

			  // gluLookAt(x, y, z, x + lx,y + ly,z + lz, 0.0f,1.0f,0.0f);

			   x = eyeX;
               y = eyeY;
               z = eyeZ;
			   lx = centerX - x;
               ly = centerY - y;
               lz = centerZ - y;
			   //upX;upY;upZ;
			   centerx = centerX;
               centery = centerY;
               centerz = centerZ;
               SetCurrentPos();
}

void FPSCamera::SetPosition(GLdouble xx,GLdouble yy,GLdouble zz){

      x = xx;
      y = yy;
      z = zz;


      glLoadIdentity();
      //gluLookAt(x,y,z,
		//	   x+lx,y+ly,z+lz,
		//	    0.0f,1.0f,0.0f);

      gluLookAt(x + lx, y + ly, z + lz,
               x ,y ,z,
               0.0f,1.0f,0.0f);

			  // gluLookAt(x, y, z, x + lx,y + ly,z + lz, 0.0f,1.0f,0.0f);



			   //upX;upY;upZ;
			   centerx = x + lx;
                centery = y + ly;
                centerz = z + lz;
                SetCurrentPos();
}

void FPSCamera::orientMe(float ang) {

	         lx = sin(ang);
             lz = -cos(ang);

             angle = ang;

         	 glLoadIdentity();
         	 centerx = x + lx;
             centery = y + ly;
             centerz = z + lz;
         	 gluLookAt(x + lx, y + ly, z + lz,
               x ,y ,z,
               0.0f,1.0f,0.0f);

             SetCurrentPos();
}

 void FPSCamera::CirclePoint(float ang) {

     float radius = 2;

     lx = sin(ang) * radius;
     lz = -cos(ang) * radius;

     angle = ang;



     glLoadIdentity();
     centerx = x + lx;
     centery = y + ly;
     centerz = z + lz;
     gluLookAt(x + lx, y + ly, z + lz,
               x ,y ,z,
               0.0f,1.0f,0.0f);

    SetCurrentPos();
/*x = radius * Math.Sin(angle)
y = 0
z = radius * Math.Cos(angle)

y = scrollvalue

'GL.gluLookAt(0, 0, 0, centerx, centery, centerz, 0.0, 1.0, 0.0)
'GL.gluLookAt(0, 0, 0, centerx, centery, centerz, 0.0, 1.0, 0.0)
' GL.gluLookAt(centerx, centery, centerz, 0, 0, 0, 0.0, 1.0, 0.0)
GL.gluLookAt(x, y, z, 0, 0, 0, 0.0, 1.0, 0.0)*/
 }

void FPSCamera::moveMeFlat(int direction) {

    if (stopmotion == true){
                stopmotion = false;
                return;
            }

        	x = x - direction*(lx)*1;
        	z = z - direction*(lz)*1;

           //y = (terrain->terrainScale() * terrain->heightAt( x / terrain->terrainScale(), z / terrain->terrainScale()) ) + HEIGHT_OFFSET;
            // y = 1.75000936f + y;
        	//y = terrain->heightAt(x + (terrain->width_starting()/2),z + (terrain->length_starting()/2)) + HEIGHT_OFFSET;

            y = terrain->heightAt(x,z) + HEIGHT_OFFSET;

        	glLoadIdentity();
        	centerx = x + lx;
            centery = y + ly;
            centerz = z + lz;
        	gluLookAt(x + lx, y + ly, z + lz,
               x ,y ,z,
               0.0f,1.0f,0.0f);
          SetCurrentPos();
}

void FPSCamera::Strafe(int direction) {

            if (stopmotion == true){
                stopmotion = false;
                return;
            }

            float tx = cos(angle);
            float tz = sin(angle);
        	x = x - direction*(tx)*1;
        	z = z - direction*(tz)*1;

        	//y = terrain->heightAt(x + (terrain->width_starting()/2),z + (terrain->length_starting()/2)) + HEIGHT_OFFSET;
            //y = (terrain->terrainScale() * terrain->heightAt( x / terrain->terrainScale(), z / terrain->terrainScale()) ) + HEIGHT_OFFSET;

            y = terrain->heightAt(x,z) + HEIGHT_OFFSET;

        	glLoadIdentity();
        	centerx = x + lx;
            centery = y + ly;
            centerz = z + lz;
        	gluLookAt(x + lx, y + ly, z + lz,
               x ,y ,z,
               0.0f,1.0f,0.0f);

        SetCurrentPos();
}

void FPSCamera::moveVert(int direction) {

    if (stopmotion == true){
                stopmotion = false;
                return;
            }

        	y += (float)direction;
        	glLoadIdentity();
        	centerx = x + lx;
            centery = y + ly;
            centerz = z + lz;
        	gluLookAt(x + lx, y + ly, z + lz,
               x ,y ,z,
               0.0f,1.0f,0.0f);

        SetCurrentPos();
}

void FPSCamera::CursorChange(int xnew, int ynew){

    if (lastx == -999 && lasty == -999){
        lastx = xnew;
        lasty = ynew;
    }

    int diffx=xnew-lastx; //check the difference between the current x and the last x position
    int diffy=ynew-lasty; //check the difference between the current y and the last y position


    xrot += (float) diffx; //set the xrot to xrot with the additionof the difference in the y position
    yrot += (float) diffy;    //set the xrot to yrot with the additionof the difference in the x position

    lastx = xnew;
    lasty = ynew;


    /*glLoadIdentity();
    centerx = x + lx;
    centery = y + ly;
    centerz = z + lz;
    gluLookAt(x, y, z,
              x + lx,y + ly,z + lz,
              0.0f,1.0f,0.0f);*/

    //from orient above is from flat

     lx = sin(xrot/100);
     lz = -cos(xrot/100);

     angle = xrot/100;

     ly = sin(yrot/100);

     yangle = yrot/100;

     glLoadIdentity();
     centerx = x + lx;
     centery = y + ly;
     centerz = z + lz;
     gluLookAt(x + lx, y + ly, z + lz,
               x ,y ,z,
               0.0f,1.0f,0.0f);

    SetCurrentPos();

}

void FPSCamera::Init(){

                    centerx = x + lx;
                    centery = y + ly;
                    centerz = z + lz;
                    glLoadIdentity();
                    gluLookAt(x + lx, y + ly, z + lz,
                       x ,y ,z,
                       0.0f,1.0f,0.0f);

                SetCurrentPos();
}

#ifndef _MAIN_H
#define _MAIN_H


#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")


#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include <fstream>
#include <vector>									
#include <crtdbg.h>
using namespace std;




//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////
#define MAXTEXTURE 100
#define CAMERASPEED	0.05f					// The Camera Speed
#define MAXBOX 5							// number of objects/boxes in your world
#define DELTABOX 2.5f						// make the size of the boundingbox larger or smaller

//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////

//////////////////////////////////////
//The tVector3 Struct
//////////////////////////////////////
typedef struct tVector3					// expanded 3D vector struct
{			
	tVector3() {}	// constructor
	tVector3 (float new_x, float new_y, float new_z) // initialize constructor	 
	{x = new_x; y = new_y; z = new_z;}
	// overload + operator so that we easier can add vectors
	tVector3 operator+(tVector3 vVector) {return tVector3(vVector.x+x, vVector.y+y, vVector.z+z);}
	// overload - operator that we easier can subtract vectors
	tVector3 operator-(tVector3 vVector) {return tVector3(x-vVector.x, y-vVector.y, z-vVector.z);}
	// overload * operator that we easier can multiply by scalars
	tVector3 operator*(float number)	 {return tVector3(x*number, y*number, z*number);}
	// overload / operator that we easier can divide by a scalar
	tVector3 operator/(float number)	 {return tVector3(x/number, y/number, z/number);}
	
	float x, y, z;						// 3D vector coordinates
}tVector3;

//////////////////////////////////////
//The tVector2 Struct
//////////////////////////////////////											
struct tVector2 
{
	float x, y;							// 2D vector coordinates
};


//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////

//////////////////////////////////////
//The tFace Struct
//////////////////////////////////////
struct tBoundingBox
{
	tVector3	max;
	tVector3	min;

};
//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////


//////////////////////////////////////
//The Camera Class
//////////////////////////////////////
class CCamera 
{
	public:
		
		tVector3 mPos;							
		tVector3 mView;							
		tVector3 mUp;				

		int mWindowWidth;
		int mWindowHeight;

		void Mouse_Move();
		void Rotate_View(float x, float y, float z);
		void Move_Camera(float cameraspeed);
		void Rotate_Around_Point(tVector3 vCenter, float x, float y, float z);
		void Position_Camera(float pos_x, float pos_y,float pos_z,
							 float view_x, float view_y, float view_z,
							 float up_x,   float up_y,   float up_z);
};



//////////////////////////////////////
//The Global Variables
//////////////////////////////////////
extern	HDC			hDC;			// Device Context
extern	HGLRC		hRC;			// Permanent Rendering Context
extern	HWND		hWnd;			// Holds Our Window Handle
extern	HINSTANCE	hInstance;		// Holds The Instance Of The Application

//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////

extern tBoundingBox bbArray[5];		// BoundingBox Array
extern int gMaxBox;

//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////

//////////////////////////////////////
//	MAIN FUNCTIONS
//////////////////////////////////////
void Keyboard_Input();



#endif

//Ronny André Reierstad
//www.morrowland.com
//apron@morrowland.com
/************************************************************
*	APRON TUTORIAL PRESENTED BY MORROWLAND					*
*************************************************************
*	Web Page				: www.morrowland.com			*
*	E-Mail					: apron@morrowland.com			*
************************************************************/

#include "main.h"


/////////////////////////////////////////////////////////////////////////////////////////////////
//										THE CCAMERA POSITION CAMERA
/////////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::Position_Camera(float pos_x,  float pos_y,  float pos_z,
							  float view_x, float view_y, float view_z,
							  float up_x,   float up_y,   float up_z)
{
	tVector3 vPos	= tVector3(pos_x,  pos_y,  pos_z);
	tVector3 vView	= tVector3(view_x, view_y, view_z);
	tVector3 vUp	= tVector3(up_x,   up_y,   up_z);
	
	mPos  = vPos;							// set the position
	mView = vView;							// set the view
	mUp   = vUp;							// set the up vector
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//										THE CCAMERA MOVE CAMERA
/////////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::Move_Camera(float cameraspeed)
{
	tVector3 vVector = tVector3(0,0,0);		// init a new view vector
	vVector = mView - mPos;					// get the view vector
	
	// forward positive cameraspeed and backward negative -cameraspeed.
	mPos.x  = mPos.x  + vVector.x * cameraspeed;
	mPos.z  = mPos.z  + vVector.z * cameraspeed;
	mView.x = mView.x + vVector.x * cameraspeed;
	mView.z = mView.z + vVector.z * cameraspeed;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//										THE CCAMERA ROTATE VIEW
/////////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::Rotate_View(float x, float y, float z)
{
	tVector3 vVector = mView - mPos;

	if(x) 
	{
		mView.z = (float)(mPos.z + sin(x)*vVector.y + cos(x)*vVector.z);
		mView.y = (float)(mPos.y + cos(x)*vVector.y - sin(x)*vVector.z);
	}
	if(y) 
	{
		mView.z = (float)(mPos.z + sin(y)*vVector.x + cos(y)*vVector.z);
		mView.x = (float)(mPos.x + cos(y)*vVector.x - sin(y)*vVector.z);
	}
	if(z) 
	{
		mView.x = (float)(mPos.x + sin(z)*vVector.y + cos(z)*vVector.x);		
		mView.y = (float)(mPos.y + cos(z)*vVector.y - sin(z)*vVector.x);
	}
}



//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////
//										THE CCAMERA ROTATE_AROUND_POINT
/////////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::Rotate_Around_Point(tVector3 vCenter, float x, float y, float z)
{	
	tVector3 vVector = mPos - vCenter;

	if(x)
	{
		mPos.z = (float)(vCenter.z + sin(x)*vVector.y + cos(x)*vVector.z);
		mPos.y = (float)(vCenter.y + cos(x)*vVector.y - sin(x)*vVector.z);
	}
	if(y) 
	{
		mPos.z = (float)(vCenter.z + sin(y)*vVector.x + cos(y)*vVector.z);
		mPos.x = (float)(vCenter.x + cos(y)*vVector.x - sin(y)*vVector.z);
	}
	if(z)
	{
		mPos.x = (float)(vCenter.x + sin(z)*vVector.y + cos(z)*vVector.x);		
		mPos.y = (float)(vCenter.y + cos(z)*vVector.y - sin(z)*vVector.x);
	}

}

//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////
//										THE CCAMERA MOUSE MOVE
/////////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::Mouse_Move()
{
	POINT mousePos;	
	int mid_x = mWindowWidth  >> 1;	
	int mid_y = mWindowHeight >> 1;	
	float angle_y  = 0.0f;				
	float angle_z  = 0.0f;							
	
	GetCursorPos(&mousePos);				// Get the mouse cursor 2D x,y position					
	
	if( (mousePos.x == mid_x) && (mousePos.y == mid_y) ) return;

	SetCursorPos(mid_x, mid_y);	// Set the mouse cursor in the middle of the window						

	// Get the direction from the mouse, and bring the number down to a reasonable amount
	angle_y = (float)( (mid_x - mousePos.x) ) / 1000;		
	angle_z = (float)( (mid_y - mousePos.y) ) / 1000;

	// The higher the number(acceleration) is the faster the camera looks around.
	mView.y += angle_z * 50;

	// limit the rotation around the x-axis
	if(mView.y > 120.0f)		mView.y = 120.0f;
	
	if(mView.y < -10.4f)		mView.y = -10.4f;
	
	Rotate_View(0, -angle_y, 0);
}



//Ronny André Reierstad
//www.morrowland.com
//apron@morrowland.com
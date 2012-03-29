/************************************************************
*	APRON TUTORIAL PRESENTED BY MORROWLAND					*
*************************************************************
*	Project Name			: BoundingBox Collision			*
*	Project Description		: Collision Made Easy			*
*	Project Type			: OpenGL						*
*	Author					: Ronny André Reierstad			*
*	Web Page				: www.morrowland.com			*
*	E-Mail					: apron@morrowland.com			*
*	Version					: English (UK)					*
*	Date					: 22.10.2002					*
************************************************************/

#include "main.h"
#include "3dsLoader.h"


HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default


C3dsLoader obj3dsLoader;
CCamera objCamera; 

//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////

tBoundingBox bbArray[MAXBOX];

void Draw_BoundingBox();

void Draw_Character();

bool Inside_BoundingBox(int box, tVector3 point);

int Collision_BoundingBox(int maxbox, tVector3 point);
//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////



LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc




/////////////////////////////////////////////////////////////////////////////////////////////////
//										THE RESIZE GL SCENE
/////////////////////////////////////////////////////////////////////////////////////////////////
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1500.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix


	objCamera.mWindowWidth  = width;
	objCamera.mWindowHeight = height;

}



/////////////////////////////////////////////////////////////////////////////////////////////////
//										THE OPENGL INIT
/////////////////////////////////////////////////////////////////////////////////////////////////
int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	
	
//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////

	           // Position			View(target)  Up
	objCamera.Position_Camera(0, 50.0f, 50.0f,	0, 50.0f, 0,   0, 1.0f, 0);

	obj3dsLoader.Init_3ds("data/model/world.3ds");

	obj3dsLoader.Calculate_BoundingBox();	// create the boundingboxes

//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////




	return TRUE;										// Initialization Went OK
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//										THE OPENGL RENDER
/////////////////////////////////////////////////////////////////////////////////////////////////
int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
	

	gluLookAt(objCamera.mPos.x,  objCamera.mPos.y,  objCamera.mPos.z,	
			  objCamera.mView.x, objCamera.mView.y, objCamera.mView.z,	
			  objCamera.mUp.x,   objCamera.mUp.y,   objCamera.mUp.z);



//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////

	// Floor
	glBegin(GL_QUADS);
		glColor3ub(150.0f,150.0f,150.0f);
		glVertex3f(-300.0f,0,-300.0f);
		glVertex3f( 300.0f,0,-300.0f);
		glVertex3f( 300.0f,0, 300.0f);
		glVertex3f(-300.0f,0, 300.0f);
	glEnd();


	glDisable(GL_TEXTURE_2D);

	Draw_BoundingBox();									// Draw the boundingboxes


	glEnable(GL_TEXTURE_2D);

	obj3dsLoader.Render_3ds();


//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////




	return TRUE;										// Keep Going
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//										THE KILL GL WINDOW
/////////////////////////////////////////////////////////////////////////////////////////////////
GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/



/////////////////////////////////////////////////////////////////////////////////////////////////
//										THE CREATE GL WINDOW
/////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","OPENGL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//										THE WINDOW PROCEDURE
/////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:

		  switch(wParam) 
		  {		  
			case VK_ESCAPE:
				PostQuitMessage(0);						
				break;
		  }
		  break;


		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////
		case WM_DESTROY:							// Destroy the window
		{
			obj3dsLoader.Unload_3ds();				// Unload the 3ds file
			return 0;
		}
//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////


	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//										THE WINMAIN
/////////////////////////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow("APRON TUTORIAL",640,480,16,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{

			Keyboard_Input();
			objCamera.Mouse_Move();


			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if ((active && !DrawGLScene()) || keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
			{
				done=TRUE;							// ESC or DrawGLScene Signalled A Quit
			}
			else									// Not Time To Quit, Update Screen
			{
				SwapBuffers(hDC);					// Swap Buffers (Double Buffering)
			}

			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1]=FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow("NeHe's Solid Object Tutorial",640,480,16,fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}



//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////
void Draw_BoundingBox()
{
	for(int i = 0; i < 5; i++)
	{
	glColor3f(0,0,1.0f);

	glBegin(GL_LINE_LOOP);
	glVertex3f( bbArray[i].min.x, bbArray[i].min.y, bbArray[i].min.z );
	glVertex3f( bbArray[i].max.x, bbArray[i].min.y, bbArray[i].min.z );
	glVertex3f( bbArray[i].max.x, bbArray[i].min.y, bbArray[i].max.z );
	glVertex3f( bbArray[i].min.x, bbArray[i].min.y, bbArray[i].max.z );
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f( bbArray[i].min.x, bbArray[i].max.y, bbArray[i].min.z );
	glVertex3f( bbArray[i].max.x, bbArray[i].max.y, bbArray[i].min.z );
	glVertex3f( bbArray[i].max.x, bbArray[i].max.y, bbArray[i].max.z );
	glVertex3f( bbArray[i].min.x, bbArray[i].max.y, bbArray[i].max.z );
	glEnd();

	glBegin(GL_LINES);
	glVertex3f( bbArray[i].min.x, bbArray[i].min.y, bbArray[i].min.z );
	glVertex3f( bbArray[i].min.x, bbArray[i].max.y, bbArray[i].min.z );

	glVertex3f( bbArray[i].max.x, bbArray[i].min.y, bbArray[i].min.z );
	glVertex3f( bbArray[i].max.x, bbArray[i].max.y, bbArray[i].min.z );

	glVertex3f( bbArray[i].max.x, bbArray[i].min.y, bbArray[i].max.z );
	glVertex3f( bbArray[i].max.x, bbArray[i].max.y, bbArray[i].max.z );

	glVertex3f( bbArray[i].min.x, bbArray[i].min.y, bbArray[i].max.z );
	glVertex3f( bbArray[i].min.x, bbArray[i].max.y,bbArray[i]. max.z );
	glEnd();
	}
}
//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////





/////////////////////////////////////////////////////////////////////////////////////////////////
//										THE KEYBOARD INPUT
/////////////////////////////////////////////////////////////////////////////////////////////////
void Keyboard_Input()
{
	if((GetKeyState(VK_UP) & 0x80) || (GetKeyState('W') & 0x80))
	{

//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////
		if (Collision_BoundingBox(MAXBOX, objCamera.mView) != -1)
		{
			static char str[20];
			sprintf(str,"collision box: %d",Collision_BoundingBox(MAXBOX, objCamera.mView));
			SetWindowText(hWnd,str);
		}
		else if (Collision_BoundingBox(MAXBOX, objCamera.mView) == -1)
		{
			SetWindowText(hWnd,"no collision");
			objCamera.Move_Camera(CAMERASPEED);	
		}		
//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////

	}

	if((GetKeyState(VK_DOWN) & 0x80) || (GetKeyState('S') & 0x80))
	{		
		
//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////
		if (Collision_BoundingBox(MAXBOX, objCamera.mPos) != -1)
		{
			static char str[20];
			sprintf(str,"collision box: %d",Collision_BoundingBox(MAXBOX, objCamera.mPos));
			SetWindowText(hWnd,str);
		}
		else if (Collision_BoundingBox(MAXBOX, objCamera.mPos) == -1)
		{
			SetWindowText(hWnd,"no collision");
			objCamera.Move_Camera(-CAMERASPEED);	
		}		
//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////

	}
	
	if((GetKeyState(VK_LEFT) & 0x80) || (GetKeyState('A') & 0x80))
	{			
		objCamera.Rotate_View( 0, -CAMERASPEED, 0);
	}

	if((GetKeyState(VK_RIGHT) & 0x80) || (GetKeyState('D') & 0x80))
	{				
		objCamera.Rotate_View( 0, CAMERASPEED, 0);
	}
}






//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////

int Collision_BoundingBox(int maxbox, tVector3 point)
{
	for (int i = 0; i < maxbox; i++)
	{
		if (Inside_BoundingBox(i,point) == 1) return i;
	}

	return -1;
}

bool Inside_BoundingBox(int box, tVector3 point)
{
	if( bbArray[box].max.x < point.x )
		return false;
	if( bbArray[box].min.x > point.x )
		return false;
	if( bbArray[box].max.z < point.z )
		return false;
	if( bbArray[box].min.z > point.z )
		return false;

	return true;	
}

//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////




/*
*****************************************************************************
	NOTES:
*****************************************************************************

Almost every 3D game today uses some kind of collision test in their game engine.
For example, you want to decide if a missile has hit a character, or a character
is colliding with a wall or trip-wire. The first goal when creating a collision
test is to decide what kind of collision test to use. You need to keep a lot of
things in mind when making this choice: high frame rate, collision accuracy, terrain, etc.
I can’t tell you what kind of collision test is best for your engine, but in
this tutorial you will learn how to use one type of the "BoundingBox Collision Test".

What this app does is test if a 3D point (view or position) is inside a BoundingBox.
This tutorial is not a complete collision test solution, because I have tried to keep this
tutorial as easy to understand as possible. So if you move against a box and twist
the camera you will eventually "break through". If you want to avoid this and create a better
test you must add more test points. One solution is to add a "box" around your camera.

MAXBOX is the amount of objects in your 3ds file.
If you cange the 3ds file then you must change this to.

DELTABOX is used to change the sizes of the boundingboxes.
-----------------------------------------------------------------------------
*/
//Regards
//Ronny André Reierstad
//www.morrowland.com
//apron@morrowland.com
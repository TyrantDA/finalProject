//OPENGL 3.2 DEMO FOR RENDERING OBJECTS LOADED FROM OBJ FILES

//includes areas for keyboard control, mouse control, resizing the window
//and draws a spinning rectangle

#include <windows.h>		// Header File For Windows
#include "gl/glew.h"
#include "gl/wglew.h"
#pragma comment(lib, "glew32.lib")

#include "shaders/Shader.h"   // include shader header file, this is not part of OpenGL


#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"
#include "backWheels.h"
#include "ground.h"
#include "Camera.h"
#include "FirstWheel.h"
#include "HandleBar.h"
#include "Chassis.h"

glm::mat4 objectRotation;
glm::quat q;

Shader* myShader;  ///shader object 
Shader* myBasicShader;

//MODEL LOADING
#include "3DStruct\threeDModel.h"
#include "Obj\OBJLoader.h"



//
glm::mat4 ProjectionMatrix; // matrix for the orthographic projection


//
int	mouse_x=0, mouse_y=0;
bool LeftPressed = false;
int screenWidth=600, screenHeight=600;
bool keys[256];
float spin=180;

float speed = 0;
float negSpeed = 0;
float posSpeed = 0;
float steer = 0;
float lean = 0;
float speedOfrotation = 0;
float steerAnimate = 0;
float leanAnimate = 0;


__int64 prevTime = 0;
double deltaT = 0;
double timerFrequencyRecip = 0.000003;

FirstWheel fw;
backWheels bw(0, -7500);
HandleBar h;
Chassis c;

ground g;
ground g2(0,0,550000);
ground g3(0, 550000, 0);
ground g4(0, 550000,550000);

//OPENGL FUNCTION PROTOTYPES
void display();				//called in winmain to draw everything to the screen
void reshape(int width, int height);				//called when the window is resized
void init();				//called in winmain when the program starts.
void processKeys();         //called in winmain to process keyboard input
void update();				//called in winmain to update variables


/*************    START OF OPENGL FUNCTIONS   ****************/
void display()									
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	
	Camera::cameraView(fw.getPos(),fw.getRot(),fw.getRo());

	bw.draw();
	fw.draw();
	h.draw();
	c.draw();

	g.draw();
	/*g2.draw();
	g3.draw();
	g4.draw();*/

	glFlush();
}

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth=width; screenHeight = height;           // to ensure the mouse coordinates match 
														// we will use these values to set the coordinate system

	glViewport(0,0,width,height);						// Reset The Current Viewport

	//Set the projection matrix
	ProjectionMatrix = glm::perspective(60.0f, (GLfloat)screenWidth/(GLfloat)screenHeight, 1.0f, 1000000.0f);
	bw.projection(ProjectionMatrix);
	fw.projection(ProjectionMatrix);
	h.projection(ProjectionMatrix);
	c.projection(ProjectionMatrix);

	g.projection(ProjectionMatrix);
	/*g2.projection(ProjectionMatrix);
	g3.projection(ProjectionMatrix);
	g4.projection(ProjectionMatrix);*/
}
void init()
{
	glClearColor(1.0,1.0,1.0,0.0);						//sets the clear colour to yellow
														//glClear(GL_COLOR_BUFFER_BIT) in the display function
														//will clear the buffer to this colour
	glEnable(GL_DEPTH_TEST);

	myShader = new Shader;
	//if(!myShader->load("BasicView", "glslfiles/basicTransformationsWithDisplacement.vert", "glslfiles/basicTransformationsWithDisplacement.frag"))
    if(!myShader->load("BasicView", "glslfiles/basicTransformations.vert", "glslfiles/basicTransformations.frag"))
	{
		cout << "failed to load shader" << endl;
	}		

	myBasicShader = new Shader;
	if(!myBasicShader->load("Basic", "glslfiles/basic.vert", "glslfiles/basic.frag"))
	{
		cout << "failed to load shader" << endl;
	}		

	glUseProgram(myShader->handle());  // use the shader

	glEnable(GL_TEXTURE_2D);
	
	bw.shader(myShader);
	bw.loadObj();

	fw.shader(myShader);
	fw.loadObj();

	g.shader(myShader);
	g.loadObj();

	h.shader(myShader);
	h.loadObj();

	c.shader(myShader);
	c.loadObj();

	/*g2.shader(myShader);
	g2.loadObj();

	g3.shader(myShader);
	g3.loadObj();

	g4.shader(myShader);
	g4.loadObj();*/


	
}
void processKeys()
{
	if (keys[VK_UP])
	{
		posSpeed += 0.01f;
	}
	else if (!keys[VK_UP])
	{
		if (posSpeed > 0 && speed > 0)
		{
			posSpeed -= 0.1f;
		}
		if (speed < 0)
		{
			posSpeed = 0;
		}
	}
	if (keys[VK_DOWN])
	{
		negSpeed -= 0.01f;
	}
	else if (!keys[VK_DOWN])
	{
		if (negSpeed < 0 && speed < 0)
		{
			negSpeed += 0.1f;
		}
		if (speed > 0)
		{
			negSpeed = 0;
		}
	}

	if (keys[VK_LEFT])
	{
		steer += 0.1f;
		steerAnimate += 0.1f;
	}
	else if (!keys[VK_LEFT])
	{
		if (steerAnimate > 0)
		{
			steerAnimate -= 0.5f;
		}
	}
	if (keys[VK_RIGHT])
	{
		steer -= 0.1f;
		steerAnimate -= 0.1f;
	}
	else if (!keys[VK_RIGHT])
	{
		if (steerAnimate < 0)
		{
			steerAnimate += 0.5f;
		}
	}
	if (keys['A'])
	{
		lean -= 0.1f;
		leanAnimate -= 0.1f;
	}
	else if (!keys['A'])
	{
		if (leanAnimate < 0)
		{
			leanAnimate += 0.5f;
		}
	}

	if (keys['D'])
	{
		lean += 0.1f;
		leanAnimate += 0.1f;
	}
	else if (!keys['D'])
	{
		if (leanAnimate > 0)
		{
			leanAnimate -= 0.5f;
		}
	}

	speed += posSpeed + negSpeed;

	if (speed > 200)
	{
		speed = 200;
	}
}

void TimeSimulation()
{
	// Get the current time
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	__int64 currentTime = t.QuadPart;

	__int64 ticksElapsed = currentTime - prevTime;					// Ticks elapsed since the previous time step
	deltaT = double(ticksElapsed) * timerFrequencyRecip;		// Convert to second
																	//cout << ticksElapsed << " " << deltaT << endl;

	// Advance timer
	prevTime = currentTime;					// use the current time as the previous time in the next step
}

void update()
{
	glm::vec3 rl = glm::vec3(0, (-lean * 3.1415f) / 180, 0);
	glm::quat rotLean = glm::quat(rl);

	glm::vec3 rs = glm::vec3(0, (steer * 3.1415f) / 180, 0);
	glm::quat bst = glm::quat(rs);

	if (leanAnimate > 40)
	{
		leanAnimate = 40;
	}
	else if (leanAnimate < -40)
	{
		leanAnimate = -40;
	}

	if (steerAnimate > 45)
	{
		steerAnimate = 45;
	}
	else if (steer < -45)
	{
		steerAnimate = -45;
	}
	
	glm::vec3 forwardVec = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 l = glm::vec3(0.0f, 0.0f, 0.0f);
	
	glm::vec3 rad = glm::vec3(0, (steerAnimate * 3.1415f) / 180, 0);

	l = glm::vec3(0, 0, (leanAnimate * 3.1415f) / 180);
	
	glm::quat bikeSteer = glm::quat(rad);
	glm::quat bikeLean = glm::quat(l);

	speedOfrotation += (1.0f / 1650.0f) * speed;
	

	glm::vec3 sp = glm::vec3(speedOfrotation, 0, 0);
	glm::quat wheelSpin = glm::quat(sp);

	float forwardDist = speed * deltaT;

	/*std::cout << fw.getPos().x << " " << fw.getPos().y << " " << fw.getPos().z << std::endl;*/

	forwardVec = rotLean * bst *  glm::vec3(0, 0, forwardDist);

	glm::quat temp = glm::quat(glm::vec3(0, 0, 0));

	fw.update(bikeLean, bikeSteer, wheelSpin, rotLean, bst, forwardVec);

	if (fw.getPos().z - g.getPos().z > g.getEdge() || fw.getPos().x - g.getPos().x > g.getEdge())
	{
		glm::vec3 temp = fw.getPos();
		temp.y = 0;
		g.setPos(temp);
		
	}


	glm::vec3 tP(0, 0, 0);
	tP.x = fw.getPos().x;
	tP.y = fw.getPos().y;
	tP.z = fw.getPos().z;

	glm::vec3 tempPos = tP + rotLean * bst * glm::vec3(0, 0, -7500);

	bw.update(bikeLean, wheelSpin, rotLean, bst, tempPos);

	
	glm::vec3 tempPos2 = tP + rotLean * bst * glm::vec3(0, 2350, -400);

	h.update(bikeLean, bikeSteer, rotLean, bst, tempPos2);

	glm::vec3 tempPos3 = tP + rotLean * bst * glm::vec3(0, 850, -5500);

	c.update(bikeLean, rotLean, bst, tempPos3);

	glm::vec3 v = glm::vec3(0, (20 * 3.1415f) / 180, 0);

	glm::quat vv = glm::quat(v);

	glm::vec3 question = vv * glm::vec3(0, 0, 300);

	glm::vec3 answer = glm::vec3(0, 0, 0);

	answer += question;

	std::cout << answer.x << " " << answer.y << " " << answer.z << std::endl;

	/*std::cout << h.getPos().x << " " << h.getPos().y << " " << h.getPos().z << std::endl;*/
}
/**************** END OPENGL FUNCTIONS *************************/

//WIN32 functions
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void KillGLWindow();									// releases and destroys the window
bool CreateGLWindow(char* title, int width, int height); //creates the window
int WINAPI WinMain(	HINSTANCE, HINSTANCE, LPSTR, int);  // Win32 main function

//win32 global variabless
HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


/******************* WIN32 FUNCTIONS ***************************/
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	bool	done=false;								// Bool Variable To Exit Loop

	AllocConsole();
	FILE* stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);

	// Create Our OpenGL Window
	if (!CreateGLWindow("OpenGL Win32 Example",screenWidth,screenHeight))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=true;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if(keys[VK_ESCAPE])
				done = true;

			processKeys();			//process keyboard
			
			display();					// Draw The Scene
			update();					// update variables
			TimeSimulation();
			SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
		}
	}

	
	// Shutdown
	KillGLWindow();									// Kill The Window
	return (int)(msg.wParam);						// Exit The Program
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}
		break;

		case WM_SIZE:								// Resize The OpenGL Window
		{
			reshape(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
		break;

		case WM_LBUTTONDOWN:
			{
	            mouse_x = LOWORD(lParam);          
				mouse_y = screenHeight - HIWORD(lParam);
				LeftPressed = true;
			}
		break;

		case WM_LBUTTONUP:
			{
	            LeftPressed = false;
			}
		break;

		case WM_MOUSEMOVE:
			{
	            mouse_x = LOWORD(lParam);          
				mouse_y = screenHeight  - HIWORD(lParam);
			}
		break;
		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = true;					// If So, Mark It As TRUE
			if (keys[wParam] == keys['C'])
			{
				Camera::changeCamera();
			}
			return 0;								// Jump Back
		}
		break;
		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = false;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}
		break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void KillGLWindow()								// Properly Kill The Window
{
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
 *	height			- Height Of The GL Window Or Fullscreen Mode			*/
 
bool CreateGLWindow(char* title, int width, int height)
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
		return false;											// Return FALSE
	}
	
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	
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
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24Bit Z-Buffer (Depth Buffer)  
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
		return false;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	HGLRC tempContext = wglCreateContext(hDC);
	wglMakeCurrent(hDC, tempContext);

	glewInit();

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		0
	};
	
    if(wglewIsSupported("WGL_ARB_create_context") == 1)
    {
		hRC = wglCreateContextAttribsARB(hDC,0, attribs);
		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(tempContext);
		wglMakeCurrent(hDC, hRC);
	}
	else
	{	//It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
		hRC = tempContext;
		cout << " not possible to make context "<< endl;
	}

	//Checking GL version
	const GLubyte *GLVersionString = glGetString(GL_VERSION);

	cout << GLVersionString << endl;

	//OpenGL 3.2 way of checking the version
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

	cout << OpenGLVersion[0] << " " << OpenGLVersion[1] << endl;

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	reshape(width, height);					// Set Up Our Perspective GL Screen

	init();
	
	return true;									// Success
}




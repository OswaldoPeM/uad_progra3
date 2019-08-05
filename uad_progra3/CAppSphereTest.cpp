

#include "stdafx.h"

#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include "Include/Globals.h"
#include "CAppSphereTest.h"
#include "Include/C3DModel.h"
#include "Include/CWideStringHelper.h"

/* */
CAppSphereTest::CAppSphereTest() :
	CAppSphereTest(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT)
{
	cout << "Constructor: CAppSphereTest()" << endl;
}

/* */
CAppSphereTest::CAppSphereTest(int window_width, int window_height) :
	CApp(window_width, window_height)
{
	cout << "Constructor: CAppEmpty(int window_width, int window_height)" << endl;

	// Initialize class member variables here
	// ======================================
	//
	// ======================================
}

/* */
CAppSphereTest::~CAppSphereTest()
{
	cout << "Destructor: ~CAppEmpty()" << endl;

	// Free memory allocated in this class instance here
	// =================================================
	//
	// =================================================
}

/* */
void CAppSphereTest::initialize()
{
	// Initialize app-specific stuff here
	// ==================================
	//
	// ==================================
}

/* */
void CAppSphereTest::run()
{
	// Check if CGameWindow object AND Graphics API specific Window library have been initialized
	if (canRun())
	{
		// Create the Window 
		// Note: The following create() method creates the Window itself. Do NOT make any OpenGL calls before this line, your app WILL crash.
		if (getGameWindow()->create(CAPP_PROGRA3_EMPTYAPP_WINDOW_TITLE))
		{
			initialize();

			// Set initial clear screen color
			getOpenGLRenderer()->setClearScreenColor(0.25f, 0.0f, 0.75f);

			// Initialize window width/height in the renderer
			
			//getOpenGLRenderer()->setFramebufferWidth(getGameWindow()->getWidth());
			//getOpenGLRenderer()->setFramebufferHeight(getGameWindow()->getHeight());

			// Enter main loop
			cout << "Entering Main loop" << endl;
			getGameWindow()->mainLoop(this);
		}
	}
}

/* */
void CAppSphereTest::update(double deltaTime)
{
	// Do not update if delta time is < 0
	if (deltaTime <= 0.0f)
	{
		return;
	}

	// Update app-specific stuff here
	// ===============================
	//
	// ===============================
}

/* */
void CAppSphereTest::render()
{
	CGameMenu *menu = getMenu();

	// If menu is active, render menu
	if (menu != NULL
		&& menu->isInitialized()
		&& menu->isActive())
	{
		//...
	}
	else // Otherwise, render app-specific stuff here...
	{
		// =================================
		//
		// =================================
	}
}

/* */
void CAppSphereTest::onMouseMove(float deltaX, float deltaY)
{
	// Update app-specific stuff when mouse moves here 
	// ===============================================
	//
	// ===============================================
}

/* */
void CAppSphereTest::executeMenuAction()
{
	if (getMenu() != NULL)
	{
		// Execute app-specific menu actions here
		// ======================================
		//
		// ======================================
	}
}
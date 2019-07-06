#include "CAppGrid.h"



bool CAppGrid::initializeMenu()
{
	cout << "CAppGrid::initializeMenu()" << endl;

	std::wstring wresourceFilenameVS;
	std::wstring wresourceFilenameFS;
	std::wstring wresourceFilenameTexture;
	std::string resourceFilenameVS;
	std::string resourceFilenameFS;
	std::string resourceFilenameTexture;

	// If resource files cannot be found, return
	if (!CWideStringHelper::GetResourceFullPath(VERTEX_SHADER_MENU, wresourceFilenameVS, resourceFilenameVS) ||
		!CWideStringHelper::GetResourceFullPath(FRAGMENT_SHADER_MENU, wresourceFilenameFS, resourceFilenameFS) ||
		!CWideStringHelper::GetResourceFullPath(MENU_TEXTURE_FILE, wresourceFilenameTexture, resourceFilenameTexture))
	{
		cout << "ERROR: Unable to find one or more resources: " << endl;
		cout << "  " << VERTEX_SHADER_MENU << endl;
		cout << "  " << FRAGMENT_SHADER_MENU << endl;
		cout << "  " << MENU_TEXTURE_FILE << endl;
		return false;
	}

	if (getMenu() != NULL)
	{
		CGameMenu *const menu = getMenu();

		unsigned int menuShaderProgramId = 0;
		unsigned int textureObjectId = 0;
		unsigned int vaoMenuItemId = 0;
		int colorUniformLocation = -1;
		int textureUniformLocation = -1;
		bool generatedMenuItemGeometry = false;
		float currentX = 0.65f, currentY = 0.8f;
		float menuItemHeight = 0.2f;
		float deltaY = 0.25f;
		float UV[8];

		// Create a shader program to use for the menu
		if (!getOpenGLRenderer()->createShaderProgram(
			&menuShaderProgramId,
			resourceFilenameVS.c_str(),
			resourceFilenameFS.c_str()))
		{
			return false;
		}

		// Set the generated shader program in the menu object
		menu->setShaderProgramId(menuShaderProgramId);

		// Read texture file and generate an OpenGL texture object
		if (loadTexture(resourceFilenameTexture.c_str(), &textureObjectId))
		{
			// Set the generated texture object in the menu object
			menu->setTextureObjectId(textureObjectId);
		}
		else
		{
			return false;
		}

		std::vector<std::string> menuOptions;
		menuOptions.push_back("Load Grid");
		menuOptions.push_back("Options");
		menuOptions.push_back("Exit");

		UV[0] = 0.01f;  // top left U
		UV[1] = 1.0f;   // top left V
		UV[2] = 0.99f;  // top right U
		UV[3] = 1.0f;   // top right V
		UV[4] = 0.01f;  // bottom left U
		UV[5] = 0.75f;  // bottom left V
		UV[6] = 0.99f;  // bottom right U
		UV[7] = 0.75f;  // bottom right V

		for (size_t i = 0; i < menuOptions.size(); i++)
		{
			// Allocate graphics memory for menu item
			generatedMenuItemGeometry = getOpenGLRenderer()->allocateGraphicsMemoryForMenuItem(
				currentX,
				currentY,
				menuItemHeight,
				UV,
				&menuShaderProgramId,
				&vaoMenuItemId
			);

			// If operation failed
			if (!generatedMenuItemGeometry)
			{
				menu->cleanupGraphicsObjects(getOpenGLRenderer());
				return false;
			}

			menu->addMenuItem(menuOptions[i].c_str(), currentX, currentY, vaoMenuItemId);
			currentY -= deltaY;

			UV[1] -= 0.25f;
			UV[3] -= 0.25f;
			UV[5] -= 0.25f;
			UV[7] -= 0.25f;

			// Custom hack, texture used is the same for the exam, but the "exit" option is the 4th, not 3rd, so increment twice
			if (i == 1)
			{
				UV[1] -= 0.25f;
				UV[3] -= 0.25f;
				UV[5] -= 0.25f;
				UV[7] -= 0.25f;
			}
		}

		return true;
	}

	return false;
}


CAppGrid::CAppGrid()
{
}

CAppGrid::CAppGrid(int window_width, int window_height) :
	CApp(window_width, window_height)
{
	cout << "Constructor: CAppGrid(int window_width, int window_height)" << endl;
}


CAppGrid::~CAppGrid()
{
	cout << "Destructor: ~CAppGrid()" << endl;
}

void CAppGrid::update(double deltaTime)
{
}

void CAppGrid::run()
{// Check if CGameWindow object and third-party Window library have been initialized
	if (canRun())
	{
		// Create the Window 
		// Note: The following create() method creates the Window itself. Do NOT make any OpenGL calls before this line, your app WILL crash.
		if (getGameWindow()->create(CAPP_PROGRA3_OBJ_LOADER_WINDOW_TITLE))
		{
			initialize();

			// Set initial clear screen color
			getOpenGLRenderer()->setClearScreenColor(0.15f, 0.75f, 0.75f);
			// Initialize window width/height in the renderer
			getOpenGLRenderer()->setWindowWidth(getGameWindow()->getWidth());
			getOpenGLRenderer()->setWindowHeight(getGameWindow()->getHeight());

			// Create our menu (add all menu items)
			if (!initializeMenu())
			{
				return;
			}

			// Enter main loop
			cout << "Entering Main loop" << endl;
			getGameWindow()->mainLoop(this);
		}
	}
}

void CAppGrid::render()
{
}

void CAppGrid::onMouseMove(float deltaX, float deltaY)
{
	if (deltaX < 100.0f && deltaY < 100.0f)
	{
		float moveX = -deltaX * DEFAULT_CAMERA_MOVE_SPEED;
		float moveZ = -deltaY * DEFAULT_CAMERA_MOVE_SPEED;

		float currPos[3];
		//m_objectPosition.getValues(currPos);
		currPos[0] += moveX;
		currPos[2] += moveZ;
		//m_objectPosition.setValues(currPos);
	}
}

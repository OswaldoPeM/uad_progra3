#include "../stdafx.h"

#include <iostream>
using namespace std;

#include <assert.h>
#include "../Include/Globals.h"
#include "../Include/CAppGeometricFigures.h"
#include "../Include/CWideStringHelper.h"
#define _USE_MATH_DEFINES
#include <math.h>

/* */
CAppGeometricFigures::CAppGeometricFigures() :
	CAppGeometricFigures(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT)
{
}

/* */
CAppGeometricFigures::CAppGeometricFigures(int window_width, int window_height) :
	CApp(window_width, window_height),
	m_currentDeltaTime{ 0.0 },
	m_objectRotation{ 0.0 },
	m_objectPosition{ -1.5f, 0.0f, 0.0f },
	m_rotationSpeed{ DEFAULT_ROTATION_SPEED },//DEFAULT_ROTATION_SPEED
	m_pyramidVertexArrayObject{ 0 },
	m_numFacesPyramid{ 0 },
	m_renderPolygonMode{ 0 }
	
{
	cout << "Constructor: CAppGeometricFigures(int window_width, int window_height)" << endl;

	// Initialize class member variables here
	// ======================================
	//
	// ======================================
}

/* */
CAppGeometricFigures::~CAppGeometricFigures()
{
	cout << "Destructor: ~CAppGeometricFigures()" << endl;

	// Free memory allocated in this class instance here
	// =================================================
	//
	if (m_textureID > 0)
	{
		getOpenGLRenderer()->deleteTexture(&m_textureID);
	}

	if (m_pyramidVertexArrayObject > 0)
	{
		getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_pyramidVertexArrayObject);
	}
	// =================================================
}

/* */
void CAppGeometricFigures::initialize()
{
	// Initialize app-specific stuff here
	// ==================================
	//
	std::wstring wresourceFilenameVS, wresourceFilenameFS, wresourceFilenameTexture;
	std::string resourceFilenameVS,resourceFilenameFS, resourceFilenameTexture;

	// Color Shader
	// Load shader file, create OpenGL Shader Object for it
	// -------------------------------------------------------------------------------------------------------------

	// Check shader for the color-only object exists
	if (!CWideStringHelper::GetResourceFullPath(VERTEX_SHADER_3D_OBJECT, wresourceFilenameVS, resourceFilenameVS) ||
		!CWideStringHelper::GetResourceFullPath(FRAGMENT_SHADER_3D_OBJECT, wresourceFilenameFS, resourceFilenameFS))
	{
		cout << "ERROR: Unable to find one or more resources: " << endl;
		cout << "  " << VERTEX_SHADER_3D_OBJECT << endl;
		cout << "  " << FRAGMENT_SHADER_3D_OBJECT << endl;

		return;
	}

	if (!getOpenGLRenderer()->createShaderProgram(
		&m_colorModelShaderId,
		resourceFilenameVS.c_str(),
		resourceFilenameFS.c_str()))
	{
		cout << "ERROR: Unable to load color shader" << endl;
		return;
	}

	// Texture Shader
	// Load shader file, create OpenGL Shader Object for it
	// -------------------------------------------------------------------------------------------------------------

	// Check shader for the textured object exists
	wresourceFilenameFS.clear();
	wresourceFilenameVS.clear();
	resourceFilenameFS.clear();
	resourceFilenameVS.clear();
	if (!CWideStringHelper::GetResourceFullPath(VERTEX_SHADER_TEXTURED_3D_OBJECT, wresourceFilenameVS, resourceFilenameVS) ||
		!CWideStringHelper::GetResourceFullPath(FRAGMENT_SHADER_TEXTURED_3D_OBJECT, wresourceFilenameFS, resourceFilenameFS))
	{
		cout << "ERROR: Unable to find one or more resources: " << endl;
		cout << "  " << VERTEX_SHADER_TEXTURED_3D_OBJECT << endl;
		cout << "  " << FRAGMENT_SHADER_TEXTURED_3D_OBJECT << endl;

		return;
	}

	if (!getOpenGLRenderer()->createShaderProgram(
		&m_texturedModelShaderId,
		resourceFilenameVS.c_str(),
		resourceFilenameFS.c_str()))
	{
		cout << "ERROR: Unable to load texture shader" << endl;
		return;
	}

	// Texture
	// Load texture file, create OpenGL Texture Object
	// -------------------------------------------------------------------------------------------------------------

	// Check texture file exists for the textured cube
	if (!CWideStringHelper::GetResourceFullPath(MC_LEAVES_TEXTURE, wresourceFilenameTexture, resourceFilenameTexture))
	{
		cout << "ERROR: Unable to find one or more resources: " << endl;
		cout << "  " << MC_LEAVES_TEXTURE << endl;
		return;
	}

	// Initialize the texture
	m_textureID = 0;
	if (!loadTexture(resourceFilenameTexture.c_str(), &m_textureID))
	{
		cout << "ERROR: Unable load texture:" << endl;
		cout << "  " << MC_LEAVES_TEXTURE << endl;
		return;
	}

	m_initialized = true;
	//createPyramidGeometry();
	createIcosaedroGeometry();
	//createSphere();
}

/* */
void CAppGeometricFigures::run()
{
	// Check if CGameWindow object AND Graphics API specific Window library have been initialized
	if (canRun())
	{
		// Create the Window 
		if (getGameWindow()->create(CAPP_PROGRA3_GEOMETRIC_WINDOW_TITLE))
		{
			initialize();

			// Set initial clear screen color
			getOpenGLRenderer()->setClearScreenColor(0.25f, 0.0f, 0.75f);

			// Initialize window width/height in the renderer
			getOpenGLRenderer()->setWindowWidth(getGameWindow()->getWidth());
			getOpenGLRenderer()->setWindowHeight(getGameWindow()->getHeight());

			if (m_initialized)
			{
				getOpenGLRenderer()->setWireframePolygonMode();

				// Enter main loop
				cout << "Entering Main loop" << endl;
				getGameWindow()->mainLoop(this);
			}
		}
	}
}

/* */
void CAppGeometricFigures::update(double deltaTime)
{
	// Do not update if delta time is < 0
	if (deltaTime <= 0.0f)
	{
		return;
	}

	// Update app-specific stuff here
	// ===============================
	//
	double degreesToRotate = 0.0;

	// Save current delta time
	m_currentDeltaTime = deltaTime;

	// Calculate degrees to rotate
	// ----------------------------------------------------------------------------------------------------------------------------------------
	// degrees = rotation speed * delta time 
	// deltaTime is expressed in milliseconds, but our rotation speed is expressed in seconds (convert delta time from milliseconds to seconds)
	degreesToRotate = m_rotationSpeed * (deltaTime / 1000.0);
	// accumulate rotation degrees
	m_objectRotation += degreesToRotate;

	// Reset rotation if needed
	while (m_objectRotation > 360.0)
	{
		m_objectRotation -= 360.0;
	}
	if (m_objectRotation < 0.0)
	{
		m_objectRotation = 0.0;
	}
	// ===============================
}

/* */
void CAppGeometricFigures::render()
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
		// White 
		// Colors are in the 0..1 range, if you want to use RGB, use (R/255, G/255, G/255)
		float color[3] = { 1.0f, 1.0f, 1.0f };
		unsigned int noTexture = 0;
	
		// convert total degrees rotated to radians;
		double totalDegreesRotatedRadians = m_objectRotation * 3.1459 / 180.0;

		// Get a matrix that has both the object rotation and translation
		MathHelper::Matrix4 modelMatrix = MathHelper::ModelMatrix((float)totalDegreesRotatedRadians, m_objectPosition);

		if (m_pyramidVertexArrayObject > 0 && m_numFacesPyramid > 0)
		{
			CVector3 pos2 = m_objectPosition;
			pos2 += CVector3(3.0f, 0.0f, 0.0f);
			MathHelper::Matrix4 modelMatrix2 = MathHelper::ModelMatrix((float)totalDegreesRotatedRadians, pos2);

			// Render pyramid in the first position, using the color shader
			getOpenGLRenderer()->renderObject(
				&m_colorModelShaderId,
				&m_pyramidVertexArrayObject,
				&noTexture,
				m_numFacesPyramid,
				color,
				&modelMatrix,
				COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
				false
			);

			// Render same pyramid (same vertex array object identifier), in a second position, but this time with a texture
			getOpenGLRenderer()->renderObject(
				&m_texturedModelShaderId,
				&m_pyramidVertexArrayObject,
				&m_textureID,
				m_numFacesPyramid,
				color,
				&modelMatrix2,
				COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
				false
			);
		}

		// =================================
	}
}

/* */
void CAppGeometricFigures::onMouseMove(float deltaX, float deltaY)
{
	// Update app-specific stuff when mouse moves here 
	// ===============================================
	//
	// ===============================================
}

/* */
void CAppGeometricFigures::createPyramidGeometry()
{
	float height = 2.25f;
	float sideHalfX = 0.75f;
	float sideHalfZ = 1.0f;
	bool loaded = false;
	float v1[3], v2[3], v3[3], v1v2[3], v1v3[3], norm[3];

	float vData[15] = {
		0.0, height, 0.0,                // TOP
		-sideHalfX,  0.0,   sideHalfZ,   // BOTTOM LEFT, FRONT
	 	 sideHalfX,  0.0,   sideHalfZ,   // BOTTOM RIGHT, FRONT
		-sideHalfX,  0.0,  -sideHalfZ,   // BOTTOM LEFT, BACK
		 sideHalfX,  0.0,  -sideHalfZ    // BOTTOM RIGHT, BACK
	};

	float vertexUVs[10] =
	{
		0.5f,  0.11f, // TOP 
		0.25f, 0.99f, // BOTTOM LEFT, FRONT
		0.75f, 0.99f, // BOTTOM RIGHT, FRONT
		0.11f, 0.40f, // BOTTOM LEFT, BACK
		0.99f, 0.40f  // BOTTOM RIGHT, BACK 
	};

	m_numFacesPyramid = 6;

	unsigned short tIndices[18] = {
		0, 1, 2,    
		0, 2, 4,    
		0, 4, 3,    
		0, 3, 1,    
		1, 3, 2,
		2, 3, 4
	};

	float nData[18] = {
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0
	};

	unsigned short nIndices[18] = {
		0, 0, 0,
		1, 1, 1,
		2, 2, 2,
		3, 3, 3,
		4, 4, 4,
		5, 5, 5
	};

	for (int i = 0; i < m_numFacesPyramid; i++)
	{
		// Vertex 1
		v1[0] = vData[tIndices[i * 3]];
		v1[1] = vData[tIndices[i * 3] + 1];
		v1[2] = vData[tIndices[i * 3] + 2];

		// Vertex 2
		v2[0] = vData[tIndices[(i * 3) + 1]];
		v2[1] = vData[tIndices[(i * 3) + 1] + 1];
		v2[2] = vData[tIndices[(i * 3) + 1] + 2];

		// Vertex 3
		v3[0] = vData[tIndices[(i * 3) + 2]];
		v3[1] = vData[tIndices[(i * 3) + 2] + 1];
		v3[2] = vData[tIndices[(i * 3) + 2] + 2];

		// Vector from v2 to v1
		v1v2[0] = v1[0] - v2[0];
		v1v2[1] = v1[1] - v2[1];
		v1v2[2] = v1[2] - v2[2];

		// Vector from v2 to v3
		v1v3[0] = v3[0] - v2[0];
		v1v3[1] = v3[1] - v2[1];
		v1v3[2] = v3[2] - v2[2];

		normcrossprod(v1v2, v1v3, norm);

		nData[i * 3]       = norm[0];
		nData[(i * 3) + 1] = norm[1];
		nData[(i * 3) + 2] = norm[2];
	}

	// Allocate graphics memory for object
	loaded = getOpenGLRenderer()->allocateGraphicsMemoryForObject(
		&m_colorModelShaderId,
		&m_pyramidVertexArrayObject,
		vData,
		5,        // Numero de vertices, internamente el codigo multiplica sizeof(float) * numVertices * 3
		nData,     
		6,
		vertexUVs,
		5,
		tIndices,
		6,        // Numero de indices, internamente el codigo multiplica sizeof(unsigned short) * numIndicesVert * 3
		nIndices,
		6,
		tIndices,
		6
	);

	if (!loaded)
	{
		m_numFacesPyramid = 0;

		if (m_pyramidVertexArrayObject > 0)
		{
			getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_pyramidVertexArrayObject);
			m_pyramidVertexArrayObject = 0;
		}
	}
}
/* */
void CAppGeometricFigures::createIcosaedroGeometry()
{
	float radio = 0.5f; //radio
	// constants
	const float PI = 3.1415926f;
	const float H_ANGLE = PI / 180 * 72;    // 72 degree = 360 / 5
	const float V_ANGLE = atanf(1.0f / 2);  // elevation = 26.565 degree

	int i1, i2;                             // indices
	float z, xy;                            // coords
	float hAngle1 = -PI / 2 - H_ANGLE / 2;  // start from -126 deg at 1st row
	float hAngle2 = -PI / 2;                // start from -90 deg at 2nd row

	bool loaded = false;
	
	/**/
	float vData[36],v1[3], v2[3], v3[3], v1v2[3], v1v3[3], norm[3];

	vData[0] = 0;
	vData[1] = 0;
	vData[2] = radio;
	for (int i = 1; i <= 5; ++i)
	{
		i1 = i * 3;         // index for 1st row
		i2 = (i + 5) * 3;   // index for 2nd row

		z = radio * sinf(V_ANGLE);            // elevaton
		xy = radio * cosf(V_ANGLE);            // length on XY plane

		vData[i1] = xy * cosf(hAngle1);      // x
		vData[i2] = xy * cosf(hAngle2);
		vData[i1 + 1] = xy * sinf(hAngle1);  // y
		vData[i2 + 1] = xy * sinf(hAngle2);
		vData[i1 + 2] = z;                   // z
		vData[i2 + 2] = -z;

		// next horizontal angles
		hAngle1 += H_ANGLE;
		hAngle2 += H_ANGLE;
	}

	vData[33] = 0;
	vData[34] = 0;
	vData[35] = -radio;

	/*
	float vData[36] = {
		0.0, height, 0.0,                // TOP

		-sideHalfX,  stepY + stepY,  -sideStepZ,   // SemiTop Left semiback
		 -sideHalfX + sideStepX,  stepY + stepY,   sideHalfZ,   // SemiTop SemiLeft Front
		sideHalfX - sideStepX,  stepY + stepY,  sideHalfZ,   // SemiTop SemiRigth Front
		 sideHalfX,  stepY + stepY,	 -sideStepZ,   // semiTop rigth semiback
		 0.0,	stepY + stepY,	-sideHalfZ, //semiTop Center Back

		 0.0,	stepY,	8+sideHalfZ, // semiBase center front
		 sideHalfX,	stepY,	sideStepZ, //semiBase right semifront
		 sideHalfX-sideStepX,	stepY,	sideHalfZ, //semiBase semiRigth back
		 -sideHalfX+sideStepX, stepY,	sideHalfZ,	//semiBase semiLeft Back
		 -sideHalfX,	stepY,	-sideStepZ,// semiBase left semifront

		 0.0,0.0,0.0 //base

	};	
	*/


	float vertexUVs[24] =
	{
		0.5f,  0.90f, // TOP 
		0.25f, 0.50f, // BOTTOM LEFT, FRONT
		0.75f, 0.99f, // BOTTOM RIGHT, FRONT
		0.11f, 0.40f, // BOTTOM LEFT, BACK
		0.99f, 0.40f,  // BOTTOM RIGHT, BACK 
		0.5f,  0.11f, // TOP 
		0.25f, 0.90f, // BOTTOM LEFT, FRONT
		0.75f, 0.99f, // BOTTOM RIGHT, FRONT
		0.11f, 0.40f, // BOTTOM LEFT, BACK
		0.99f, 0.40f, // BOTTOM RIGHT, FRONT
		0.11f, 0.40f, // BOTTOM LEFT, BACK
		0.99f, 0.40f  // BOTTOM RIGHT, BACK
	};

	m_numFacesPyramid = 20;

	unsigned short tIndices[60] = { 
		0, 2, 1,//top
		0, 3, 2,
		0, 4, 3,
		0, 5, 4,
		0, 1, 5,//top

		1,2,6,
		2,6,7,
		2,3,7,
		3,7,8,
		3,8,4,
		4,8,9,
		4,9,5,
		5,9,10,
		5,10,1,
		1,10,6,
		
		
		//1,2,10, //middle
		//2,6,10,
		//2,3,6,
		//3,7,6,
		//3,4,7,
		//4,8,7,
		//4,5,8,
		//5,9,8,
		//5,1,9,
		//1,10,9, //middle

		10,6,11, 
		6,7,11,
		7,8,11,
		8,9,11,
		9,10,11


		/*0, 1, 2,
		0, 2, 4,
		0, 4, 3,
		0, 3, 1,
		1, 3, 2,
		2, 3, 4*/
	};

	float nData[60] = {
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0

	};

	unsigned short nIndices[60] = {
		0, 0, 0,
		1, 1, 1,
		2, 2, 2,
		3, 3, 3,
		4, 4, 4,
		5, 5, 5,
		6, 6, 6,
		7, 7, 7,
		8, 8, 8,
		9, 9, 9,
		10,10,10,
		11,11,11,
		12,12,12,
		13,13,13,
		14,14,14,
		15,15,15,
		16,16,16,
		17,17,17,
		18,18,18,
		19,19,19
	};

	for (int i = 0; i < m_numFacesPyramid; i++)
	{
		// Vertex 1
		v1[0] = vData[tIndices[i * 3]];
		v1[1] = vData[tIndices[i * 3] + 1];
		v1[2] = vData[tIndices[i * 3] + 2];

		// Vertex 2
		v2[0] = vData[tIndices[(i * 3) + 1]];
		v2[1] = vData[tIndices[(i * 3) + 1] + 1];
		v2[2] = vData[tIndices[(i * 3) + 1] + 2];

		// Vertex 3
		v3[0] = vData[tIndices[(i * 3) + 2]];
		v3[1] = vData[tIndices[(i * 3) + 2] + 1];
		v3[2] = vData[tIndices[(i * 3) + 2] + 2];

		// Vector from v2 to v1
		v1v2[0] = v1[0] - v2[0];
		v1v2[1] = v1[1] - v2[1];
		v1v2[2] = v1[2] - v2[2];

		// Vector from v2 to v3
		v1v3[0] = v3[0] - v2[0];
		v1v3[1] = v3[1] - v2[1];
		v1v3[2] = v3[2] - v2[2];

		normcrossprod(v1v2, v1v3, norm);

		nData[i * 3] = norm[0];
		nData[(i * 3) + 1] = norm[1];
		nData[(i * 3) + 2] = norm[2];
	}

	// Allocate graphics memory for object
	loaded = getOpenGLRenderer()->allocateGraphicsMemoryForObject(
		&m_colorModelShaderId,
		&m_pyramidVertexArrayObject,
		vData,
		12,        // Numero de vertices, internamente el codigo multiplica sizeof(float) * numVertices * 3
		nData,
		20,
		vertexUVs,
		12,
		tIndices,
		20,        // Numero de indices, internamente el codigo multiplica sizeof(unsigned short) * numIndicesVert * 3
		nIndices,
		20,
		tIndices,
		20
	);

	if (!loaded)
	{
		m_numFacesPyramid = 0;

		if (m_pyramidVertexArrayObject > 0)
		{
			getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_pyramidVertexArrayObject);
			m_pyramidVertexArrayObject = 0;
		}
	}
	/**/
}

void CAppGeometricFigures::createSphere()
{
	float radio = 1.0f;
	bool loaded = false;
	const int stack = 10;
	const int sector = 10;


	int ind1 = 3, ind2 = 0, ind3;
	float v1[3], v2[3], v3[3], v1v2[3], v1v3[3], norm[3],x,y,z,xy;
	float vData[(stack*sector*3) + 6];
	double stepSec = 2 * M_PI / sector;
	double stepSk = M_PI / stack;
	vData[0] = 0;
	vData[1] = 0;
	vData[2] = radio;
	for (int i = 1; i < stack; i++)
	{
		double latitud = M_PI / 2 - i * stepSk;
		xy = radio * cosf(latitud);
		z = radio * sinf(latitud);
		for (int j = 0;j<=sector;j++)
		{
			double longitud = stepSec * j;
			x = xy * cosf(longitud);
			y = xy * sinf(longitud);
			vData[ind1++] = x;
			vData[ind1++] = y;
			vData[ind1++] = z;
		}
	}
	vData[(stack*sector * 3) + 3] = 0;
	vData[(stack*sector * 3) + 4] = 0;
	vData[(stack*sector * 3) + 5] = -radio;

	float vertexUVs[(((1 + stack)*sector) * 2) * 2];
	for (size_t i = 0; i < (((1 + stack)*sector) * 2)*2; i++)
	{
		vertexUVs[i] = 0.5f;
		vertexUVs[i++] = 0.11f;
	}

	m_numFacesPyramid = (((1 + stack)*sector) * 2);
	unsigned short tIndices[(((1 + stack)*sector) * 2) * 3];
	ind1 = 0;
	ind2 = 1;
	for (int i = 0; i < sector*3;i++)
	{
		tIndices[i] = 0;

		tIndices[++i] = ind2;

		tIndices[++i] = ++ind2;
	}
	ind3 = (sector * 3);

	for (int i = 1; i < stack+1; i++)
	{
		ind1 = i * (sector + 1);
		ind2 = ind1 + sector + 1;
		for (int j = 0; j < sector; j++,ind3++,ind1++,ind2++)
		{
			/*tIndices[ind3] = ind1;
			tIndices[++ind3] = ind2;
			tIndices[++ind3] = ind1 + 1;
			tIndices[++ind3] = ind1 + 1;
			tIndices[++ind3] = ind2;
			tIndices[++ind3] = ind2 + 1;*/
			tIndices[ind3] = 0;
			tIndices[++ind3] = 0;
			tIndices[++ind3] = 0;
			tIndices[++ind3] = 0;
			tIndices[++ind3] = 0;
			tIndices[++ind3] = 0;
		}
	}
	for (int i = 0; i < sector; i++)
	{
		/*tIndices[ind3++] = ind2;

		tIndices[ind3++] = 0;

		tIndices[ind3++] = ind2 + 1;*/
		tIndices[ind3++] = 0;

		tIndices[ind3++] = 0;

		tIndices[ind3++] = 0;
	}

	float nData[(((1 + stack)*sector) * 2) * 3];
	for (int i = 0; i < (((1 + stack)*sector) * 2) * 3; i++)
	{
		nData[i] = 0.0f;
	}

	ind1 = 0;
	unsigned short nIndices[(((1 + stack)*sector) * 2) * 3];
	for (int i = 0; i < (((1 + stack)*sector) * 2); i++,ind1++)
	{
		nIndices[ind1] = i;
		nIndices[++ind1] = i;
		nIndices[++ind1] = i;
	}

	for (int i = 0; i < m_numFacesPyramid; i++)
	{
		// Vertex 1
		v1[0] = vData[tIndices[i * 3]];
		v1[1] = vData[tIndices[i * 3] + 1];
		v1[2] = vData[tIndices[i * 3] + 2];

		// Vertex 2
		v2[0] = vData[tIndices[(i * 3) + 1]];
		v2[1] = vData[tIndices[(i * 3) + 1] + 1];
		v2[2] = vData[tIndices[(i * 3) + 1] + 2];

		// Vertex 3
		v3[0] = vData[tIndices[(i * 3) + 2]];
		v3[1] = vData[tIndices[(i * 3) + 2] + 1];
		v3[2] = vData[tIndices[(i * 3) + 2] + 2];

		// Vector from v2 to v1
		v1v2[0] = v1[0] - v2[0];
		v1v2[1] = v1[1] - v2[1];
		v1v2[2] = v1[2] - v2[2];

		// Vector from v2 to v3
		v1v3[0] = v3[0] - v2[0];
		v1v3[1] = v3[1] - v2[1];
		v1v3[2] = v3[2] - v2[2];

		normcrossprod(v1v2, v1v3, norm);

		nData[i * 3] = norm[0];
		nData[(i * 3) + 1] = norm[1];
		nData[(i * 3) + 2] = norm[2];
	}

	// Allocate graphics memory for object
	loaded = getOpenGLRenderer()->allocateGraphicsMemoryForObject(
		&m_colorModelShaderId,
		&m_pyramidVertexArrayObject,
		vData,
		(sector*stack)+2,        // Numero de vertices, internamente el codigo multiplica sizeof(float) * numVertices * 3
		nData,
		(((1 + stack)*sector) * 2),
		vertexUVs,
		(sector*stack) + 2,
		tIndices,
		(((1 + stack)*sector) * 2),        // Numero de indices, internamente el codigo multiplica sizeof(unsigned short) * numIndicesVert * 3
		nIndices,
		(((1 + stack)*sector) * 2),
		tIndices,
		(((1 + stack)*sector) * 2)
	);

	if (!loaded)
	{
		m_numFacesPyramid = 0;

		if (m_pyramidVertexArrayObject > 0)
		{
			getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_pyramidVertexArrayObject);
			m_pyramidVertexArrayObject = 0;
		}
	}
}

/* */
void CAppGeometricFigures::onF2(int mods)
{
}

/* */
void CAppGeometricFigures::onF3(int mods)
{
	if (m_renderPolygonMode == 0)
	{
		getOpenGLRenderer()->setFillPolygonMode();
		m_renderPolygonMode = 1;
	}
	else
	{
		getOpenGLRenderer()->setWireframePolygonMode();
		m_renderPolygonMode = 0;
	}
}

/* */
void CAppGeometricFigures::normcrossprod(float v1[3], float v2[3], float out[3])
{
	out[0] = v1[1] * v2[2] - v1[2] * v2[1];
	out[1] = v1[2] * v2[0] - v1[0] * v2[2];
	out[2] = v1[0] * v2[1] - v1[1] * v2[0];

	normalize(out);
}

/* */
void CAppGeometricFigures::normalize(float v[3])
{
	float d = sqrtf(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	if (d == 0.0f)
	{
		return;
	}
	v[0] /= d;
	v[1] /= d;
	v[2] /= d;
}
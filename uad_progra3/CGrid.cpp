#include "CGrid.h"
#include "stdafx.h"

#include <iostream>
using namespace std;

#include <assert.h>
#include "Include/Globals.h"
#include "Include/CAppGeometricFigures.h"
#include "Include/CWideStringHelper.h"

#define _USE_MATH_DEFINES
#include <math.h>



void CGrid::reset()
{
	m_row = 0;
	m_col = 0;
	if (vData != nullptr) {
		delete[] vData;
		vData = nullptr;
	}
	if (vertexUVs != nullptr) {
		delete[] vertexUVs;
		vertexUVs = nullptr;
	}
	if (nData != nullptr) {
		delete[] nData;
		nData = nullptr;
	}
	if (tIndices != nullptr) {
		delete[] tIndices;
		tIndices = nullptr;
	}
	if (nIndices != nullptr) {
		delete[] nIndices;
		nIndices = nullptr;
	}
	// Free memory allocated in this class instance here
	// =================================================
	//
	if (m_textureID > 0)
	{
		getOpenGLRenderer()->deleteTexture(&m_textureID);
	}

	if (m_graphicsMemoriObjectId > 0)
	{
		getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_graphicsMemoriObjectId);
	}
	// =================================================

	if (m_grid != nullptr) {
		for (int i = 0; i < m_col; i++)
		{
			delete[] m_grid[i];
		}
		delete[] m_grid;
		m_grid = nullptr;
	}

}

void CGrid::setTIndicesSize()
{
	tIndices = new unsigned short[m_numFacesGrid * 3];
	m_numIndicesVert = m_numFacesGrid;
	m_numIndicesUVCoords = m_numFacesGrid;
}

void CGrid::setNDataSize()
{
	nData = new float[m_numFacesGrid * 3];
	for (int i = 0; i < m_numFacesGrid * 3; i++)
	{
		nData[i] = 0.0f;
	}
	m_numNormals = m_numFacesGrid;
}

void CGrid::setNIndices()
{
	nIndices = new unsigned short[m_numFacesGrid * 3];
	for (int i = 0; i < m_numFacesGrid; i++)
	{
		nIndices[(i * 3)] = i;
		nIndices[(i * 3)+1] = i;
		nIndices[(i * 3)+2] = i;
	}
	m_numIndicesNormal = m_numFacesGrid;
}

void CGrid::setVDataSize()
{
	vData = new float[m_col*m_row * 3 * 6];
	m_numVertices = m_col * m_row * 6;
}

void CGrid::setVertexUVsSize()
{
	vertexUVs = new float[m_col*m_row * 2 * 6];
	m_nummUVCoord = m_col * m_row * 6;
}

void CGrid::addVData(int &index, CVector3 *vertex)
{
	vData[index++] = vertex[0].getX();
	vData[index++] = vertex[0].getY();
	vData[index++] = vertex[0].getZ();
	vData[index++] = vertex[1].getX();
	vData[index++] = vertex[1].getY();
	vData[index++] = vertex[1].getZ();
	vData[index++] = vertex[2].getX();
	vData[index++] = vertex[2].getY();
	vData[index++] = vertex[2].getZ();
	vData[index++] = vertex[3].getX();
	vData[index++] = vertex[3].getY();
	vData[index++] = vertex[3].getZ();
	vData[index++] = vertex[4].getX();
	vData[index++] = vertex[4].getY();
	vData[index++] = vertex[4].getZ();
	vData[index++] = vertex[5].getX();
	vData[index++] = vertex[5].getY();
	vData[index++] = vertex[5].getZ();

}

void CGrid::addVertexUVs()
{
	int length = m_col * m_row * 2 * 6;
	for (int i = 0; i < length; i++)
	{
		vertexUVs[i++] = 0.5f;
		vertexUVs[i] = 0.60f;
	}
}

void CGrid::addTInices(int i, int j, int &index)
{
	tIndices[index++] = (m_col * 6 * i) + (j * 6);
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 2;
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 1;

	tIndices[index++] = (m_col * 6 * i) + (j * 6);
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 2;
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 5;

	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 2;
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 3;
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 5;

	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 3;
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 5;
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 4;
}

void CGrid::normcrossprod(float v1[3], float v2[3], float out[3])
{
	out[0] = v1[1] * v2[2] - v1[2] * v2[1];
	out[1] = v1[2] * v2[0] - v1[0] * v2[2];
	out[2] = v1[0] * v2[1] - v1[1] * v2[0];

	normalize(out);
}

void CGrid::normalize(float v[3])
{
	float d = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	if (d == 0.0f)
	{
		return;
	}
	v[0] /= d;
	v[1] /= d;
	v[2] /= d;
}


CGrid::CGrid()
{
}

CGrid::CGrid(int window_width, int window_height) :
	CApp(window_width, window_height)
{
	vData = nullptr, vertexUVs = nullptr, nData = nullptr,tIndices = nullptr, nIndices = nullptr;

	cout << "Constructor: CAppGrid(int window_width, int window_height)" << endl;
}


CGrid::~CGrid()
{
	reset();
}

void CGrid::initialize()
{
}

bool CGrid::readWorld(const char * const filename)
{
	std::wstring wresourceFilenameVS;
	std::wstring wresourceFilenameFS;
	std::string resourceFilenameVS;
	std::string resourceFilenameFS;

	char *vertexShaderToLoad = VERTEX_SHADER_3D_OBJECT;
	char *fragmentShaderToLoad = FRAGMENT_SHADER_3D_OBJECT;
	reset();

	std::string stdFilename(filename);
	size_t dotIndex = stdFilename.rfind('.', stdFilename.length());
	if (dotIndex != string::npos)
	{
		std::string fileExtension = stdFilename.substr(dotIndex + 1, stdFilename.length() - dotIndex);

		// Convert to lowercase
		// NOTE: ::tolower works on single bytes, which can be a problem for multi-byte encoding, like UTF8
		std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);

		cout << "File extension: " << fileExtension << endl;

		// Now check the file type and see if it's a supported type
		if (!fileExtension.compare("json"))
		{
			cout << "Loading JSON ..." << endl;
			readJson(filename);
		
		}
		
	}
	else
	{
		cout << "ERROR: Cannot determine the file type" << endl;
		return nullptr;
	}
}

void CGrid::readJson(const char * const filename)
{
	ifstream infile;
	json J;
	string orientacion;
	infile.open(filename);
	int m_numObj;

	J << infile;
	m_col = J["HexGrid"]["numCols"];
	m_row = J["HexGrid"]["numRows"];
	m_cellSize=J["HexGrid"]["cellSize"];
	orientacion = J["HexGrid"]["orientation"].get<string>();
	if (orientacion == "pointy")m_orientation = true;
	if (orientacion == "flat")m_orientation = false;
	m_numObj = J["HexGrid"]["3DObj"].size();
	/*for (int i = 0; i < m_numObj; i++)
	{*/
	/*	CObjInstance obj;
		C3DModel *OBJ;
		string aber = "C:\\Users\\Oswal\\Documents\\uad_progra3\\uad_progra3\\Resources\\MEDIA\\MODELS\\TF2\\" + J["HexGrid"]["3DObj"]["ObjA"]["fileName"].get<string>();

		OBJ = C3DModel::load(aber.c_str());
		obj.Obj = OBJ;
		obj.cords = CVector3(
			J["HexGrid"]["3DObj"]["ObjA"]["cords"]["X"].get<float>(),
			J["HexGrid"]["3DObj"]["ObjA"]["cords"]["Y"].get<float>(),
			J["HexGrid"]["3DObj"]["ObjA"]["cords"]["Z"].get<float>()
		);
		obj.scale = J["HexGrid"]["3DObj"]["ObjA"]["scale"].get<float>();
		m_objs.push_back(obj);*/
	//}
		infile.close();
}

void CGrid::initialize(int cols, int  rows, float size, bool flat)
{
	reset();
	//Setters
	int vDataIndx = 0, tIndIndex = 0;
	float v1[3], v2[3], v3[3], v1v2[3], v1v3[3], norm[3];
	m_row = rows;
	m_col = cols;
	setVDataSize();
	setVertexUVsSize();
	m_numFacesGrid = m_col * m_row * 4;
	setTIndicesSize();
	setNIndices();

	// Initialize app-specific stuff here
// ==================================
//
	std::wstring wresourceFilenameVS, wresourceFilenameFS, wresourceFilenameTexture;
	std::string resourceFilenameVS, resourceFilenameFS, resourceFilenameTexture;

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
		&m_gridShaderPrgmID,
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
		&m_gridTextureProgramID,
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


	//create a bidimiensional array of CGridCells
	m_grid = new CGridCell*[m_row];
	for (int i = 0; i < m_row; i++)
	{
		m_grid[i] = new CGridCell[m_col];
	}

	for (int i = 0; i < m_row; i++)
	{
		for (int j = 0; j < m_col; j++)
		{
			if (i == 0 && j == 0)m_grid[i][j] = CGridCell(nullptr, m_cellSize, flat);
			else {
				m_grid[i][j] = CGridCell(m_grid[0][0].getVecVerx(), nullptr, m_orientation, i, j);
				
			}

			addVData(vDataIndx, m_grid[i][j].getVecVerx());
			addTInices(i, j, tIndIndex);

			m_gridQT.push_back(m_grid[i][j]);

		}
	}

	addVertexUVs();
	setNDataSize();
	for (int i = 0; i < m_numFacesGrid; i++)
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

		/*nData[i * 3] = norm[0];
		nData[(i * 3) + 1] = norm[1];
		nData[(i * 3) + 2] = norm[2];*/
		nData[i * 3] = 0.0f;
		nData[(i * 3) + 1] = 1.0f;
		nData[(i * 3) + 2] = 0.0f;
	}


	loaded = getOpenGLRenderer()->allocateGraphicsMemoryForObject(
		&m_gridShaderPrgmID,
		&m_graphicsMemoriObjectId,
		vData,
		m_numVertices,
		nData,
		m_numNormals,
		vertexUVs,
		m_nummUVCoord,
		tIndices,
		m_numIndicesVert,
		nIndices,
		m_numIndicesNormal,
		tIndices,
		m_numIndicesUVCoords
	);

	if (!loaded) {
		m_numFacesGrid = 0;
		if (m_graphicsMemoriObjectId>0)
		{
			getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_graphicsMemoriObjectId);
			m_graphicsMemoriObjectId = 0;
		}
	}
}

void CGrid::initializeQT(int cols, int rows, float size, bool flat)
{
	reset();
	//Setters
	int vDataIndx = 0, tIndIndex = 0;
	float v1[3], v2[3], v3[3], v1v2[3], v1v3[3], norm[3];
	m_row = rows;
	m_col = cols;
	vData = new float[18];


	m_nummUVCoord = 6;
	m_numFacesGrid = 4;
	m_numVertices = 6;
	m_numIndicesNormal = m_numFacesGrid;
	vertexUVs = new float[12];
	tIndices = new unsigned short[12];
	m_numIndicesVert = m_numFacesGrid;

	m_numIndicesUVCoords = m_numFacesGrid;

	nData = new float[12];
	m_numNormals = m_numFacesGrid;

	nIndices = new unsigned short[12];
	m_numIndicesNormal = m_numFacesGrid;


	// Initialize app-specific stuff here
// ==================================
//
	std::wstring wresourceFilenameVS, wresourceFilenameFS, wresourceFilenameTexture;
	std::string resourceFilenameVS, resourceFilenameFS, resourceFilenameTexture;

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
		&m_gridShaderPrgmID,
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
		&m_gridTextureProgramID,
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


	//create a bidimiensional array of CGridCells
	m_grid = new CGridCell*[m_row];
	for (int i = 0; i < m_row; i++)
	{
		m_grid[i] = new CGridCell[m_col];
	}

	for (int i = 0; i < m_row; i++)
	{
		for (int j = 0; j < m_col; j++)
		{
			if (i == 0 && j == 0)m_grid[i][j] = CGridCell(nullptr, m_cellSize, flat);
			else {
				m_grid[i][j] = CGridCell(m_grid[0][0].getVecVerx(), nullptr, m_orientation, i, j);

			}

			//addVData(vDataIndx, m_grid[i][j].getVecVerx());
			//addTInices(i, j, tIndIndex);

			m_gridQT.push_back(m_grid[i][j]);

		}
	}

	vData[0] = m_gridQT[0].getVecVerx()[0].getX();
	vData[1] = m_gridQT[0].getVecVerx()[0].getY();
	vData[2] = m_gridQT[0].getVecVerx()[0].getZ();
	vData[3] = m_gridQT[0].getVecVerx()[1].getX();
	vData[4] = m_gridQT[0].getVecVerx()[1].getY();
	vData[5] = m_gridQT[0].getVecVerx()[1].getZ();
	vData[6] = m_gridQT[0].getVecVerx()[2].getX();
	vData[7] = m_gridQT[0].getVecVerx()[2].getY();
	vData[8] = m_gridQT[0].getVecVerx()[2].getZ();
	vData[9] = m_gridQT[0].getVecVerx()[3].getX();
	vData[10] = m_gridQT[0].getVecVerx()[3].getY();
	vData[11] = m_gridQT[0].getVecVerx()[3].getZ();
	vData[12] = m_gridQT[0].getVecVerx()[4].getX();
	vData[13] = m_gridQT[0].getVecVerx()[4].getY();
	vData[14] = m_gridQT[0].getVecVerx()[4].getZ();
	vData[15] = m_gridQT[0].getVecVerx()[5].getX();
	vData[16] = m_gridQT[0].getVecVerx()[5].getY();
	vData[17] = m_gridQT[0].getVecVerx()[5].getZ();

	//
	vertexUVs[0] = 0.5f;
	vertexUVs[1] = 0.9f;
	vertexUVs[2] = 0.5f;
	vertexUVs[3] = 0.9f;
	vertexUVs[4] = 0.5f;
	vertexUVs[5] = 0.9f;
	vertexUVs[6] = 0.5f;
	vertexUVs[7] = 0.9f;
	vertexUVs[8] = 0.5f;
	vertexUVs[9] = 0.9f;
	vertexUVs[10] = 0.5f;
	vertexUVs[11] = 0.9f;

	//
	tIndices[0] = 0;
	tIndices[1] = +2;
	tIndices[2] = +1;

	tIndices[3] = 0;
	tIndices[4] = +2;
	tIndices[5] = +5;

	tIndices[6] = +2;
	tIndices[7] = +3;
	tIndices[8] = +5;

	tIndices[9] = +3;
	tIndices[10] = +5;
	tIndices[11] = +4;

	//
	nData[0] = 0.0;
	nData[1] = 1.0;
	nData[2] = 0.0;

	nData[3] = 0.0;
	nData[4] = 1.0;
	nData[5] = 0.0;

	nData[6] = 0.0;
	nData[7] = 1.0;
	nData[8] = 0.0;

	nData[9] = 0.0;
	nData[10] = 1.0;
	nData[11] = 0.0;

	//
	nIndices[0] = 0;
	nIndices[1] = 0;
	nIndices[2] = 0;

	nIndices[3] = 1;
	nIndices[4] = 1;
	nIndices[5] = 1;

	nIndices[6] = 2;
	nIndices[7] = 2;
	nIndices[8] = 2;

	nIndices[9] = 3;
	nIndices[10] = 3;
	nIndices[11] = 3;

	//


	loaded = getOpenGLRenderer()->allocateGraphicsMemoryForObject(
		&m_gridShaderPrgmID,
		&m_graphicsMemoriObjectId,
		vData,
		m_numVertices,
		nData,
		m_numNormals,
		vertexUVs,
		m_nummUVCoord,
		tIndices,
		m_numIndicesVert,
		nIndices,
		m_numIndicesNormal,
		tIndices,
		m_numIndicesUVCoords
	);

	if (!loaded) {
		m_numFacesGrid = 0;
		if (m_graphicsMemoriObjectId > 0)
		{
			getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_graphicsMemoriObjectId);
			m_graphicsMemoriObjectId = 0;
		}
	}
}



void CGrid::update(double deltatime)
{
}

void CGrid::renderQT()
{
	CGameMenu *menu = getMenu();

	// If menu is active, render menu
	if (menu != NULL
		&& menu->isInitialized()
		&& menu->isActive())
	{
	}
	else 
	{

		float color[3] = { 1.0f, 1.0f, 1.0f };
		unsigned int noTexture = 0;
		double totalDegreesRotatedRadians = 3.1459 / 180.0;

		


		for (int i = 0; i < m_gridQT.size(); i++)
		{
			m_objectPosition = m_gridQT[i].getPos();
			MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrixRotationTranslation((float)totalDegreesRotatedRadians, m_objectPosition);

		

			getOpenGLRenderer()->renderObject(
				&m_gridShaderPrgmID,
				&m_graphicsMemoriObjectId,
				&noTexture,
				m_numFacesGrid,
				color,
				&modelMatrix,
				COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
				false
			);
		}
	}
}

void CGrid::render()
{
	renderQT();
	//CGameMenu *menu = getMenu();

	//// If menu is active, render menu
	//if (menu != NULL
	//	&& menu->isInitialized()
	//	&& menu->isActive())
	//{
	//	//...
	//}
	//else // Otherwise, render app-specific stuff here...
	//{
	//	// =================================
	//	//
	//	// White 
	//	// Colors are in the 0..1 range, if you want to use RGB, use (R/255, G/255, G/255)
	//	float color[3] = { 1.0f, 1.0f, 1.0f };
	//	unsigned int noTexture = 0;

	//	// convert total degrees rotated to radians;
	//	double totalDegreesRotatedRadians = 1 * 3.1459 / 180.0;

	//	// Get a matrix that has both the object rotation and translation
	//	MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrixRotationTranslation((float)totalDegreesRotatedRadians, m_objectPosition);

	//	if (m_graphicsMemoriObjectId > 0 && m_numFacesGrid > 0)
	//	{
	//		CVector3 pos2 = m_objectPosition;
	//		pos2 += CVector3(0.0f, 0.0f, 0.0f);
	//		MathHelper::Matrix4 modelMatrix2 = MathHelper::SimpleModelMatrixRotationTranslation((float)totalDegreesRotatedRadians, pos2);

	//		getOpenGLRenderer()->renderObject(
	//			&m_gridShaderPrgmID,
	//			&m_graphicsMemoriObjectId,
	//			&noTexture,
	//			m_numFacesGrid,
	//			color,
	//			&modelMatrix,
	//			COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
	//			false
	//		);

	//		if (!m_objs.empty()) {

	//			for (int i = 0; i < m_objs.size(); i++) {
	//				C3DModel *m_p3DModel;
	//				totalDegreesRotatedRadians = m_objs[i].rotation * 3.1459 / 180.0;
	//				m_p3DModel = m_objs[i].Obj;
	//				pos2 = m_objs[i].cords;
	//				pos2 = CVector3(m_grid[ (int)m_objs[i].cords.getX()][(int)m_objs[i].cords.getZ()].getPos().getX(), 0, m_grid[(int)m_objs[i].cords.getX()][(int)m_objs[i].cords.getZ()].getPos().getZ());
	//				MathHelper::Matrix4 pos3 = MathHelper::SimpleModelMatrixRotationTranslation((float)totalDegreesRotatedRadians, pos2);
	//				for (int i = 0; i < m_p3DModel->getNumMaterials(); i++)
	//				{
	//					for (int j = 0; j < m_p3DModel->getScopeMat(i).size(); j += 2)
	//					{

	//						//if (*m_p3DModel->getTextureObjId(i))continue;//
	//						getOpenGLRenderer()->renderObjectMultiMat(
	//							m_p3DModel->getScopeMat(i)[j],
	//							m_p3DModel->getScopeMat(i)[j + 1],
	//							m_p3DModel->getShaderProgramId(),
	//							m_p3DModel->getGraphicsMemoryObjectId(),
	//							&noTexture,
	//							m_p3DModel->getNumFaces(),
	//							color,
	//							&pos3,
	//							COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
	//							false
	//						);
	//					}
	//				}

	//			}
	//		}
	//	}

	//	// =================================
	//}
}

CVector3 CGrid::getPos(int x, int y)
{
	return m_grid[x][y].getPos();
}

void CGrid::onF2(int mods)
{
	std::wstring wideStringBuffer = L"";
	wideStringBuffer.resize(MAX_PATH);

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = L"All files\0*.*\0";
	ofn.lpstrFile = &wideStringBuffer[0];
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = L"Select a model file";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn))
	{
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wideStringBuffer[0], (int)wideStringBuffer.size(), NULL, 0, NULL, NULL);
		std::string multibyteString(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, &wideStringBuffer[0], (int)wideStringBuffer.size(), &multibyteString[0], size_needed, NULL, NULL);
		cout << "Filename to load: " << multibyteString.c_str() << endl;

		if (!readWorld(multibyteString.c_str()))
		{
			cout << "Unable to load hexgrid" << endl;
		}
	}
	initializeQT(m_col, m_row, m_cellSize, m_orientation);
}

void CGrid::onF4(int mods)
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

void CGrid::onArrowUp(int mods)
{
	getOpenGLRenderer()->moveCamera(0.05f);
}

void CGrid::onArrowDown(int mods)
{
	getOpenGLRenderer()->moveCamera(-0.05f);
}

void CGrid::run()
{
	if (canRun())
	{
		// Create the Window 
		if (getGameWindow()->create(CAPP_PROGRA3_HEXGRID_WINDOW_TITLE))
		{

			initialize();

			// Set initial clear screen color
			getOpenGLRenderer()->setClearScreenColor(0.25f, 0.0f, 0.75f);

			// Initialize window width/height in the renderer
			//getOpenGLRenderer()->setFramebufferWidth(getGameWindow()->getWidth());
		
			//getOpenGLRenderer()->setFramebufferHeight(getGameWindow()->getHeight());

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

bool CGrid::initializeMenu()
{
	//initialize(10, 10, 1, false);
	return true;
}

CGridCell CGrid::getCell(int x, int y)
{
	return m_gridQT[(y*m_row) + x];
}



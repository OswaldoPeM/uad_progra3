#pragma once
#include<vector>
#include<math.h>
#include "Include\COpenGLRenderer.h"
#include"CGridCell.h"
#include "Include/CApp.h"

class CGrid:public CApp
{
	CGridCell **m_grid=nullptr;
	CVector3 m_objectPosition{ 0.0f,0.0f,0.0f };
	int  m_col, m_row, m_numFacesGrid,m_renderPolygonMode;
	bool loaded = false;
	bool m_initialized;
	double m_objectRotatiion;
	double m_objecrotatiionSpeed;

	unsigned int m_gridShaderPrgmID=1;
	unsigned int m_gridTextureProgramID=1;
	unsigned int m_textureID=0;
	unsigned int m_graphicsMemoriObjectId=0;

	float *vData=nullptr,*vertexUVs=nullptr,*nData=nullptr;
	unsigned short *tIndices=nullptr,*nIndices=nullptr;

	int m_numVertices, m_numNormals, m_nummUVCoord, m_numIndicesVert, m_numIndicesNormal, m_numIndicesUVCoords;

	void reset();

	void setVDataSize();
	void setVertexUVsSize();
	void setTIndicesSize();
	void setNDataSize();
	void setNIndices();

	void addVData(int &index, CVector3 *vertex);
	void addVertexUVs();
	void addTInices(int i, int j, int &index);

	void normcrossprod(float vi[3], float v2[3], float out[3]);
	void normalize(float v[3]);

	
public:
	CGrid();
	CGrid(int window_width, int window_height);
	~CGrid();
	void initialize();
	void initialize(int cols, int  rows, float size, bool flat);
	void update(double deltatime);
	void render();
	void run();
	bool initializeMenu();
	CVector3 getPos(int x, int y);

	void onF4(int mods);
	void onArrowUp(int mods);
	void onArrowDown(int mods);
};


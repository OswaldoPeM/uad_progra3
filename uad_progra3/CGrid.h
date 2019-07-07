#pragma once
#include<vector>
#include<math.h>
#include "Include\COpenGLRenderer.h"
#include"CGridCell.h"
#include "Include/CApp.h"

class CGrid:public CApp
{
	CGridCell **m_grid;
	int  m_col, m_row, m_numFacesGrid;
	bool loaded = false;

	unsigned int m_gridShaderPrgmID=0;
	unsigned int m_gridTextureProgramID=0;
	unsigned int m_graphicsMemoriObjectId;

	float *vData,*vertexUVs,*nData;
	unsigned short *tIndices,*nIndices;

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
	~CGrid();

	void initialize(int cols, int  rows, float size, bool flat);
	CVector3 getPos(int x, int y);
};


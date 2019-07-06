#pragma once
#include<vector>
#include<math.h>
#include "Include\COpenGLRenderer.h"
#include"CGridCell.h"

class CGrid
{
	CGridCell **m_grid;
	int  m_col, m_row, m_numFacesGrid;

	unsigned int m_gridShaderPrgmID=0;
	unsigned int m_gridTextureProgramID=0;
	unsigned int m_graphicsMemoriObjectId;

	float *vData,*vertexUVs,*nData;
	int *tIndices,nIndices;
	

	


	void reset();

	void setVDataSize();
	void setVertexUVsSize();
	void setTIndicesSize();
	void setNDataSize();

	void addVData(int &index, CVector3 *vertex);
	void addVertexUVs();
	void addTInices(int i, int j, int &index);

public:
	CGrid();
	~CGrid();
	void initialize(int cols, int  rows, float size, bool flat);
	CVector3 getPos(int x, int y);
};


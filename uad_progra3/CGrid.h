#pragma once
#include<vector>
#include<math.h>
#include"CGridCell.h"

class CGrid
{
	int  m_col, m_row, m_numFacesGrid;
	CGridCell **m_grid;
	float *vData;
	float *vertexUVs;
	int *tIndices;

	void clearGrid();
	void setTIndicesSize();
	void setVDataSize();
	void setVertexUVsSize();
	void addVData(int &index, CVector3 *vertex);
	void addVertexUVs();
	void addTInices(int i, int j, int &index);

public:
	CGrid();
	~CGrid();
	void initialize(int cols, int  rows, float size, bool flat);
	CVector3 getPos(int x, int y);
};


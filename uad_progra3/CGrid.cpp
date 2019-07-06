#include "CGrid.h"



void CGrid::clearGrid()
{
	m_row = 0;
	m_col = 0;
	if (m_grid != nullptr) {
		for (int i = 0; i < m_col; i++)
		{
			delete[] m_grid[i];
		}
		m_grid = nullptr;
	}
	if (vData != nullptr) {
		delete[] vData;
		vData = nullptr;
	}
	if (vertexUVs != nullptr) {
		delete[] vertexUVs;
		vertexUVs = nullptr;
	}
}

void CGrid::setTIndicesSize()
{
	tIndices = new int[m_numFacesGrid * 3];
}

void CGrid::setVDataSize()
{
	vData = new float[m_col*m_row * 3 * 6];
}

void CGrid::setVertexUVsSize()
{
	vertexUVs = new float[m_col*m_row * 6];
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
	int length = m_col * m_row * 6;
	for (int i = 0; i < length; i++)
	{
		vertexUVs[i++] = 0.5f;
		vertexUVs[i] = .90f;
	}
}

void CGrid::addTInices(int i, int j, int &index)
{
	tIndices[index++] = (m_col * 6 * i) + (j * 6);
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 2;
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 1;

	tIndices[index++] = (m_col * 6 * i) + (j * 6);
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 5;
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 2;

	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 2;
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 5;
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 3;

	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 2;
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 5;
	tIndices[index++] = (m_col * 6 * i) + (j * 6) + 4;
}

CGrid::CGrid()
{
}


CGrid::~CGrid()
{
}

void CGrid::initialize(int cols, int  rows, float size, bool flat)
{
	clearGrid();
	//Setters
	int vDataIndx = 0, tIndIndex = 0;
	m_row = rows;
	m_col = cols;
	setVDataSize();
	setVertexUVsSize();
	m_numFacesGrid = m_col * m_row * 4;
	setTIndicesSize();

	//create a bidimiensional array of CGridCells
	float x, y, sizeSide, sizeHight;
	m_grid = new CGridCell*[m_row];
	for (int i = 0; i < m_row; i++)
	{
		m_grid[i] = new CGridCell[m_col];
	}

	//if flat, x cosf(30) y sinf(30).
	//space between hex center and 
	x = (!flat) ? sinf(60)*size * 2 : cosf(90)*size * 2;
	y = (!flat) ? cosf(0)*size * 2 : cosf(30)*size * 2;


	for (int i = 0; i < m_row; i++)
	{
		for (int j = 0; j < m_col; j++)
		{
			if (i == 0)if (j == 0)m_grid[i][j] = CGridCell(-1, size, flat);
			m_grid[i][j] = CGridCell(m_grid[0][0].getVecVerx(), -1, x, y, size, flat, i, j);

			addVData(vDataIndx, m_grid[i][j].getVecVerx());
			addTInices(i, j, tIndIndex);
		}
	}
	addVertexUVs();
}

CVector3 CGrid::getPos(int x, int y)
{
	return m_grid[x][y].getPos();
}

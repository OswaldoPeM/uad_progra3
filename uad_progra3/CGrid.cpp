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
}

void CGrid::setVData()
{
	int i1,i2;
	vData = new float[m_col*m_row * 3 * 6];
	for (int i = 0; i < m_col; i++)
	{
			i1 = i * 3;
		for (int j = 0; j < m_row; j++)
		{
			
		}
	}
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

	m_row = rows;
	m_col = cols;

	//create a bidimiensional array of CGridCells
	float x, y,sizeSide,sizeHight;
	m_grid = new CGridCell*[cols];
	for (int i = 0; i < cols; i++)
	{
		m_grid[i] = new CGridCell[rows];
	}

	//if flat, x cosf(30) y sinf(30).
	//space between hex center and 
	x = (!flat) ? sinf(60)*size*2 : cosf(90)*size*2;
	y = (!flat) ? cosf(0)*size*2 : cosf(30)*size*2;


	for (int i = 0; i < cols; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			if (i == 0)if (j == 0)m_grid[i][j] = CGridCell(-1, size, flat);
			m_grid[i][j] = CGridCell(m_grid[0][0].getVecVerx(), -1, x, y, size, flat, i, j);

		}
	}
}

CVector3 CGrid::getPos(int x, int y)
{
	return m_grid[x][y].getPos();
}

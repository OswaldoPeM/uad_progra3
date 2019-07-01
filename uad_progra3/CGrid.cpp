#include "CGrid.h"



CGrid::CGrid()
{
}


CGrid::~CGrid()
{
}

void CGrid::initialize(int cols, int  rows, float size, bool flat)
{
	// // // // // NOTE: CHECK IF THIS IS OK

	//create a bidimiensional array of CGridCells
	//m_grid = new CGridCell[cols][rows];

	float x, y,sizeSide,sizeHight;
	//if flat, x cosf(30) y sinf(30).
	x = (!flat) ? cosf(60)*size : cosf(30)*size;
	y = (!flat) ? sinf(60)*size : sinf(30)*size;
	//space between hex center.
	sizeSide = size * x * 2;
	sizeHight = size * y * 2;

	for (int i = 0; i < rows; i++)
	{
		m_grid[i] = new CGridCell[rows];
	}
	for (int i = 0; i < cols; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			if (i == 0)if (j == 0)m_grid[i][j] = CGridCell(-1, size, flat);

		}
	}
}

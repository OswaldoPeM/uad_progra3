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

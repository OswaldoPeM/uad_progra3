#pragma once
#include<vector>
#include<math.h>
#include"CGridCell.h"

class CGrid
{
	int  m_col, m_row;
	CGridCell **m_grid;
public:
	CGrid();
	~CGrid();
	void initialize(int cols, int  rows, float size,bool flat);
};


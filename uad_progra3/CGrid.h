#pragma once
#include<vector>
#include"CGridCell.h"

class CGrid
{
	CGridCell **m_grid;
public:
	CGrid();
	~CGrid();
	void initialize(int cols, int  rows, float size,bool flat);
};


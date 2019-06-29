#pragma once
#include"Include/CVector3.h"
class CGridCell
{
	//0 to 5 are the vertex of the hex, the 6th is te center of the hex.
	CVector3 m_vertex[7];

public:
	CGridCell();
	~CGridCell();
};


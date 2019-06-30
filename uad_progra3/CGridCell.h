#pragma once
#include"Include/CVector3.h"
class CGridCell
{
	int m_ObjInstanceIndex = -1;
	//0 to 5 are the vertex of the hex, the 6th is te center of the hex.
	CVector3 m_vertex[7];

public:
	CGridCell();
	CGridCell(int objIndex,float size);
	CGridCell(CVector3 firstCell, int ObjIndex, int X, int Y,float size, bool flat);
	~CGridCell();

};


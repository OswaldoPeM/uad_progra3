#include "CGridCell.h"



CGridCell::CGridCell()
{
}

CGridCell::CGridCell(int objIndex,float size,bool flat)
{
	m_ObjInstanceIndex = objIndex;
	m_vertex[6].X = 0;
	m_vertex[6].Y = 0;
	m_vertex[6].Z = 0;
}

CGridCell::CGridCell(CVector3 firstCell, int ObjIndex, int X, int Y, float size, bool flat)
{
}


CGridCell::~CGridCell()
{
}

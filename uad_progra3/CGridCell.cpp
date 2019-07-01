#include "CGridCell.h"



CGridCell::CGridCell()
{
}

CGridCell::CGridCell(int objIndex,float size,bool flat)
{
	m_ObjInstanceIndex = objIndex;
	for (int i = 0; i < 6; i++)
	{
		m_vertex[i].X = (!flat) ? cosf(60 * i)*size : cosf(30*i)*size;
		m_vertex[i].Y = 0;
		m_vertex[i].Z = (!flat) ? sinf(60 * i)*size : sinf(30 * i)*size;
	}
	m_vertex[6].X = 0;
	m_vertex[6].Y = 0;
	m_vertex[6].Z = 0;
}

CGridCell::CGridCell(CVector3 firstCell, int ObjIndex, float X, float Y, float size, bool flat)
{
}


CGridCell::~CGridCell()
{
}

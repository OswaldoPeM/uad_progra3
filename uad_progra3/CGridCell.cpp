#include "CGridCell.h"



void CGridCell::getVertex(float * buf)
{
	for (int i = 0; i < 18; i = i)
	{
		buf[i++] = m_vertex->getX();
		buf[i++] = m_vertex->getY();
		buf[i++] = m_vertex->getZ();
	}
}

CGridCell::CGridCell()
{
}

CGridCell::CGridCell(int objIndex,float size,bool flat)
{
	m_ObjInstanceIndex = objIndex;
	for (int i = 0; i < 6; i++)
	{
		m_vertex[i].X = (!flat) ? sinf(60 * i)*size : cosf(60 * i)*size;
		m_vertex[i].Y = 0;
		m_vertex[i].Z = (!flat) ? cosf(60 * i)*size : sinf(60 * i)*size;
	}
	m_vertex[6].X = 0;
	m_vertex[6].Y = 0;
	m_vertex[6].Z = 0;
}

CGridCell::CGridCell(CVector3 *firstCell, int ObjIndex, float X, float Y, float size, bool flat,int i,int j)
{
	float farX,awaY,par;
	m_ObjInstanceIndex = ObjIndex;
	if (!flat) {
		par = (!(i & 1)) ? firstCell[1].getX() : 0;
		farX = firstCell[1].getX();
		awaY = firstCell[0].getZ()*1.5f;
		for (int i = 0; i < 7; i++)
		{
			m_vertex[i].X = firstCell[i].getX() + (farX * 2 * j) + par;
			m_vertex[i].Y = 0;
			m_vertex[i].Z = firstCell[i].getZ() + (awaY  * j);
		}
	}
	else {

		par = (!(j & 1)) ? firstCell[4].getZ() : 0;
		farX = firstCell[3].getX();
		awaY = firstCell[4].getZ();

		for (int i = 0; i < 7; i++)
		{
			m_vertex[i].X = firstCell->getX() + X;
			m_vertex[i].Y = 0;
			m_vertex[i].Z = firstCell->getZ() + Y;
		}
	}
}


CGridCell::~CGridCell()
{
}



int CGridCell::getObjInstance()
{
	return m_ObjInstanceIndex;
}

void CGridCell::setObjIns(int objIns)
{
	m_ObjInstanceIndex = objIns;
}

CVector3 * CGridCell::getVecVerx()
{
	return m_vertex;
}

CVector3 CGridCell::getPos()
{
	return m_vertex[6];
}

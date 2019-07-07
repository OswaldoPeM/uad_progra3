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

CGridCell::CGridCell(int *objIndex,float size,bool flat)
{
	m_ObjInstanceIndex = objIndex;
	for (int i = 0; i < 6; i++)
	{
		m_vertex[i].X = (flat) ? -sin(((60 * i) - 30)*(3.14159265359/180.0))*size : cosf(((60 * i) - 30)*(3.14159265359 / 180.0))*size;
		m_vertex[i].Y = 0;
		m_vertex[i].Z = (flat) ? -cos(((60 * i)-30)*(3.14159265359 / 180.0))*size : sinf(((60 * i) - 30)*(3.14159265359 / 180.0))*size;
	}
	m_vertex[6].X = 0;
	m_vertex[6].Y = 0;
	m_vertex[6].Z = 0;
}

CGridCell::CGridCell(CVector3 *firstCell, int *ObjIndex, bool flat,int x,int y)
{
	float farX,awaY,par;
	m_ObjInstanceIndex = ObjIndex;
	if (!flat) {
		par = ((x & 1)) ? firstCell[0].getX() : 0;
		farX = firstCell[0].getX();
		awaY = firstCell[2].getZ()*1.5f;
		for (int i = 0; i < 7; i++)
		{
			m_vertex[i].X = firstCell[i].getX() + (farX * 2 * y) + par;
			m_vertex[i].Y = 0;
			m_vertex[i].Z = firstCell[i].getZ() + (awaY  * x);
		}
	}
	else {

		par = ((y & 1)) ? firstCell[4].getZ() : 0;
		farX = firstCell[3].getX()*1.5;
		awaY = firstCell[4].getZ();

		for (int i = 0; i < 7; i++)
		{
			m_vertex[i].X = firstCell->getX() + (farX * y);
			m_vertex[i].Y = 0;
			m_vertex[i].Z = firstCell->getZ() + (awaY * 2 * x) + par;
		}
	}
}


CGridCell::~CGridCell()
{
}



int *CGridCell::getObjInstance()
{
	return m_ObjInstanceIndex;
}

void CGridCell::setObjIns(int *objIns)
{
	m_ObjInstanceIndex = objIns;
}

void CGridCell::setTextureID(unsigned int texID)
{
	m_textureID = texID;
}

int * CGridCell::getObjins()
{
	return m_ObjInstanceIndex;
}

CVector3 * CGridCell::getVecVerx()
{
	return m_vertex;
}

CVector3 CGridCell::getPos()
{
	return m_vertex[6];
}

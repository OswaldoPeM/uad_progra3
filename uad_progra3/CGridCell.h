#include"Include/CVector3.h"
#include<math.h>
#pragma once

class CGridCell
{
	int m_ObjInstanceIndex = -1;
	//0 to 5 are the vertex of the hex, the 6th is te center of the hex.
	CVector3 m_vertex[7];
	void getVertex(float* buf);
public:
	CGridCell();
	CGridCell(int objIndex, float size, bool flat);
	CGridCell(CVector3 *firstCell, int ObjIndex, float X, float Y, float size, bool flat, int i, int j);
	~CGridCell();

	void setObjIns(int objIns);

	CVector3 *getVecVerx();
	CVector3 getPos();
	int getObjInstance();
};
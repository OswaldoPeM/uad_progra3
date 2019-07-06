#include"Include/CVector3.h"
#include<math.h>
#pragma once

class CGridCell
{

	//0 to 5 are the vertex of the hex, the 6th is te center of the hex.
	CVector3 m_vertex[7];
	void getVertex(float* buf);

	int *m_ObjInstanceIndex = nullptr;
	unsigned int m_textureID;


public:
	CGridCell();
	CGridCell(int *objIndex, float size, bool flat);
	CGridCell(CVector3 *firstCell, int *ObjIndex, bool flat, int x, int y);
	~CGridCell();

	void setObjIns(int objIns);
	void setTextureID(unsigned int texID);

	int* getObjins();
	CVector3 *getVecVerx();
	CVector3 getPos();
	int getObjInstance();
};
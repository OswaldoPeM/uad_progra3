#pragma once
#include<vector>
#include"Include/CVector3.h"
#include"Include/C3DModel.h"
class CObjInstance
{
	//Index of Vector<object>,


public:
	C3DModel *Obj;
	const char* fileName;
	float scale,rotation;

	CObjInstance();
	~CObjInstance();
};


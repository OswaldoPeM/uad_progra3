#pragma once
#include"Include/CVector3.h"
class AABB
{
public:
	CVector3 m_points[8];
	void setPoints(CVector3 *points);
	void setPointAB(CVector3 AA, CVector3 BB, float h);
	AABB();
	~AABB();
};


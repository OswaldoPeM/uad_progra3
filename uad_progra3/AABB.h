#pragma once
#include"Include/CVector3.h"
class AABB
{
public:
	CVector3 m_points[8];
	void setPoints(CVector3 *points);

	AABB();
	~AABB();
};


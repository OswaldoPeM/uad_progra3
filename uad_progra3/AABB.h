#pragma once
#include"Include/CVector3.h"
class AABB
{
public:
	CVector3 m_points[8];
	AABB();
	~AABB();
};


#include "AABB.h"



void AABB::setPoints(CVector3 * points)
{
	*m_points = *points;
}

void AABB::setPointAB(CVector3 AA, CVector3 BB, float h)
{
	m_points[0] = AA;
	m_points[1] = CVector3(BB.X, BB.Y, AA.Z);
	m_points[2] = CVector3(AA.X, AA.Y, BB.Z);
	m_points[3] = BB;

	m_points[4] = m_points[0] + CVector3(0, h, 0);
	m_points[5] = m_points[1] + CVector3(0, h, 0);
	m_points[6] = m_points[2] + CVector3(0, h, 0);
	m_points[7] = m_points[3] + CVector3(0, h, 0);
}

AABB::AABB()
{
}


AABB::~AABB()
{
}

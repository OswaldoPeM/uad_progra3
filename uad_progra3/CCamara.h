#pragma once
#include "Include/CVector3.h"
#include "Include/MathHelper.h"

class CCamara
{
	CVector3 m_pos, m_rightVec, m_upVec, m_lookAt;
	MathHelper::Matrix4 m_projMatrix, m_viewMatrix;
public:
	CCamara();
	~CCamara();
};


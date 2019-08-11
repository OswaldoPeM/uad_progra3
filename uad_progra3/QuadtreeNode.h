#pragma once

#include<vector>
#include"AABB.h"


template <class T>
class QuadtreeNode
{
public:
	QuadtreeNode<T> 
		*m_n1,
		*m_n2,
		*m_n3,
		*m_n4;
	std::vector<T*> m_data;
	AABB m_boundigBox;

	QuadtreeNode();
	~QuadtreeNode();
};


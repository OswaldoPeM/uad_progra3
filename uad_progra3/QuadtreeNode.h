#pragma once

#include<vector>
#include"Include/CFrustum.h"



template <class T>
class QuadtreeNode
{
public:
	QuadtreeNode<T>
		*m_n1 = nullptr,
		*m_n2 = nullptr,
		*m_n3 = nullptr,
		*m_n4 = nullptr;
	std::vector<T*> m_data;
	AABB m_boundingBox;

	void setAABB(AABB box);
	void subDivide(int limit, std::vector<T*> grid);

	bool isInAABB(CVector3 potition);
	void divideAABB();

	std::vector<T*> getData(std::vector<T*> &buffer);

	void render(unsigned int *graphicsMemoriObjectId);

	QuadtreeNode();
	~QuadtreeNode();
};


template<class T>
void QuadtreeNode<T>::setAABB(AABB box)
{
	AABB = box;
}

template<class T>
void QuadtreeNode<T>::subDivide(int limit, std::vector<T*> grid)
{
	int count = 0;
	for (int i = 0; i < grid.size(); i++)
	{
		if (isInAABB(grid[i]->getPos()))
		{
			count += 4;
			if (grid[i]->OBJ)
			{
				count += grid[i]->objQT.Obj->getNumFaces();
			}
		}
	}
	if (count > limit)
	{
		m_n1 = new QuadtreeNode<T>();
		m_n2 = new QuadtreeNode<T>();
		m_n3 = new QuadtreeNode<T>();
		m_n4 = new QuadtreeNode<T>();

		divideAABB();

		m_n1->subDivide(limit, grid);
		m_n2->subDivide(limit, grid);
		m_n3->subDivide(limit, grid);
		m_n4->subDivide(limit, grid);
	}
	else {
		for (int i = 0; i < grid.size(); i++)
		{
			if (isInAABB(grid[i]->getPos()))
			{
				m_data.push_back(grid[i]);
			}
		}
	}
}

template<class T>
bool QuadtreeNode<T>::isInAABB(CVector3 potition)
{
	float max_X, min_X, max_Z, min_Z;
	min_X = m_boundingBox.m_points[0].X;
	max_Z = m_boundingBox.m_points[0].Z;

	max_X = m_boundingBox.m_points[3].X;
	min_Z = m_boundingBox.m_points[3].Z;
	// verifica si esta dentro de las cordenandas.
	if (potition.X > min_X && potition.Z > min_Z) {
		if (potition.X < max_X&&potition.Z < max_Z) {
			return true;
		}
	}

	return false;
}

template<class T>
void QuadtreeNode<T>::divideAABB()
{
	CVector3 medio((m_boundingBox.m_points[0].X + m_boundingBox.m_points[3].X) / 2, m_boundingBox.m_points[0].Y, (m_boundingBox.m_points[0].Z + m_boundingBox.m_points[3].Z) / 2);
	m_n1->m_boundingBox.setPointAB(m_boundingBox.m_points[0], medio, m_boundingBox.m_points[4].Y);
	m_n2->m_boundingBox.setPointAB(CVector3(medio.X, m_boundingBox.m_points[0].Y, m_boundingBox.m_points[3].Z), CVector3(m_boundingBox.m_points[3].X, m_boundingBox.m_points[0].Y, medio.Z), m_boundingBox.m_points[4].Y);
	m_n3->m_boundingBox.setPointAB(CVector3(m_boundingBox.m_points[0].X, m_boundingBox.m_points[0].Y, medio.Z), CVector3(medio.X, m_boundingBox.m_points[0].Y, m_boundingBox.m_points[3].Z), m_boundingBox.m_points[4].Y);
	m_n4->m_boundingBox.setPointAB(medio, m_boundingBox.m_points[3], m_boundingBox.m_points[4].Y);

}

template<class T>
inline std::vector<T*> QuadtreeNode<T>::getData(std::vector<T*> &buffer)
{
	if (m_n1 != nullptr) {
		m_n1->getData(buffer);
		m_n2->getData(buffer);
		m_n3->getData(buffer);
		m_n4->getData(buffer);
	}
	else {
		for (int i = 0; i < m_data.size(); i++)
		{
			buffer.push_back(m_data[i]);
		}
	}
	
	return vector<T*>();
}

template<class T>
void QuadtreeNode<T>::render(unsigned int *graphicsMemoriObjectId)
{
	if (m_n1 != nullptr) {
		if (CFrustum::isAABBVisible(m_boundingBox) > 0) {

		}
	}

}



QuadtreeNode<class T>::QuadtreeNode()
{
}


QuadtreeNode<class T>::~QuadtreeNode()
{
	if (m_n1 != nullptr) {
		delete m_n1;
		delete m_n2;
		delete m_n3;
		delete m_n4;
	}
}

#pragma once
#include"QuadtreeNode.h"


class CGrid;

template <class T>
class Quadtree
{
public:
	int m_trilimit;
	QuadtreeNode<T> *m_root = nullptr;
	Quadtree();
	Quadtree(int limit, CGrid &grid);

	void render(unsigned int *graphicsMemoriObjectId);
	void getData(vector<T*> &buffer);
	~Quadtree();
};



Quadtree<class T>::Quadtree()
{
}

template<class T>
Quadtree<T>::Quadtree(int limit, CGrid &grid)
{
	AABB rootbox;
	m_root = new QuadtreeNode<T>();

	CVector3 AA, BB;
	if (grid.m_orientation) {
		AA.X = grid.m_gridQT[0]->getVecVerx()[5].X;
		AA.Y = grid.m_gridQT[0]->getVecVerx()[6].Y;
		AA.Z = grid.m_gridQT[0]->getVecVerx()[0].Z;

		if ((grid.m_row & 1) == 1) {
			BB.X = grid.m_gridQT[grid.m_gridQT.size() - 1]->getVecVerx()[1].X;
		}
		else {
			BB.X = grid.m_gridQT[grid.m_gridQT.size() - 1 - grid.m_col]->getVecVerx()[1].X;
		}
		BB.Y = grid.m_gridQT[grid.m_gridQT.size() - 1]->getVecVerx()[6].Y;
		BB.Z = grid.m_gridQT[grid.m_gridQT.size() - 1]->getVecVerx()[3].Z;
	}
	else {
		AA.X = grid.m_gridQT[0]->getVecVerx()[5].X;
		AA.Y = grid.m_gridQT[0]->getVecVerx()[6].Y;
		AA.Z = grid.m_gridQT[0]->getVecVerx()[0].Z;

		if ((grid.m_col & 1) == 1) {
			BB.Z = grid.m_gridQT[grid.m_gridQT.size() - 1]->getVecVerx()[1].Z;
		}
		else {
			BB.Z = grid.m_gridQT[grid.m_gridQT.size() - 1 - grid.m_col]->getVecVerx()[1].Z;
		}
		BB.Y = grid.m_gridQT[grid.m_gridQT.size() - 1]->getVecVerx()[6].Y;
		BB.X = grid.m_gridQT[grid.m_gridQT.size() - 1]->getVecVerx()[1].X;
	}

	rootbox.setPointAB(AA, BB, 1.0f);
	m_root->subDivide(m_trilimit, grid.m_gridQT);
}

template<class T>
void Quadtree<T>::render(unsigned int *graphicsMemoriObjectId)
{
	m_root->render();
}

template<class T>
inline void Quadtree<T>::getData(vector<T*>& buffer)
{
	m_root->getData(buffer);
}


Quadtree<class T>::~Quadtree()
{
}

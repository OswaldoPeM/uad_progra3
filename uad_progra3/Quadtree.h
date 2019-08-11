#pragma once
#include"QuadtreeNode.h"
//#include"CGrid.h"

class CGrid;

template <class T>
class Quadtree
{
public:
	int m_trilimit;
	QuadtreeNode<T> *m_root = nullptr;
	Quadtree();
	Quadtree(int limit, CGrid grid);
	~Quadtree();
};


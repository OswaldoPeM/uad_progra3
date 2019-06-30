#pragma once
#include<vector>
class CObjInstance
{
	//Index of Vector<object>,
	int ObjInd = 0;
	float references[3] = { 0.0f,1.0f,0.0f };
	std::vector<int> IndTex;

public:
	CObjInstance();
	~CObjInstance();
};


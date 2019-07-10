#pragma once
#include<vector>
#include"Include/Globals.h"
#include"Include/C3DModel_Obj.h"
#include"CGrid.h"

class CWorld :public CApp
{
	COpenGLRenderer *openGlRenderer;
	CGrid *GRID;
	std::vector<unsigned int> gridShaders;

public:
	void initialize();
	void readWorld();
	void update();
	void render();
	
	CWorld();
	CWorld(int window_width, int window_height);
	~CWorld();
};


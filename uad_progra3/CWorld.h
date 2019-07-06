#pragma once
#include<vector>
#include"Include/Globals.h"
#include"Include/C3DModel_Obj.h"
#include"CGrid.h"

class CWorld
{
	COpenGLRenderer *openGlRenderer;
public:
	void initialize();
	void readWorld();
	void update();
	void render();
	
	CWorld();
	~CWorld();
};


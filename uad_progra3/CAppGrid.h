#pragma once
#ifndef CAPPGRID_H
#define CAPPGRID_H
#include"Include/Globals.h"
#include "Include/CApp.h"
#include"Include/CWideStringHelper.h"
#include"CWorld.h"
class CAppGrid : public CApp
{
	CWorld *mundo;
protected:
	bool initializeMenu();
public:
	CAppGrid();
	CAppGrid(int window_width, int window_height);
	~CAppGrid();

	//Method to initialize any objets for this class
	void  initialize() {}
	//Method to update any grid based on time elapsed sincelast frame
	void update(double deltaTime);
	//Method to run the app
	void run();
	//Method to render the app
	void render();
	//Called when mouse moves within the window
	void onMouseMove(float deltaX, float deltaY);


	void onF2(int mods);
};

#endif  //!CAPPGRID_H
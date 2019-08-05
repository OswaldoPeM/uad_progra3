// uad_progra3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<fstream>
#include <iomanip>
#include <iostream>
using namespace std;

#include "Include/CGameWindow.h"
#include "Include/CApp.h"
#include "Include/CAppCubeTest.h"
#include "Include/CAppObjLoader.h"
#include "Include/CAppGeometricFigures.h"
#include "Include/CAppEmpty.h"
#include "CAppGrid.h"

//incluir esto en el lector de JSON
//#include "Dependencies/JSON/nlohmann/json.hpp"
//using  json = nlohmann::json;

int main()
{
	
	CApp *app = NULL;                  // Pointer to BASE class CApp
	app = new CAppCubeTest(800, 600);  // Using pointer to base class, create a new object of DERIVED class
	app->run();                        // Run the app
	delete app;                        // Delete pointer
	app = NULL;                        // Set pointer to NULL

	return 0;
}


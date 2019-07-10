#include "CWorld.h"



void CWorld::initialize()
{

}

void CWorld::readWorld()
{
}

void CWorld::update()
{
}

void CWorld::render()
{
}

CWorld::CWorld()
{
	
}

CWorld::CWorld(int window_width, int window_height):
	CApp(window_width, window_height),
	GRID(nullptr),
	openGlRenderer(nullptr)
{
}


CWorld::~CWorld()
{
}

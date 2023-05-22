#include "pch.h"
#include "TRMain.h"

TRMain::TRMain()
{
	active_world = new TRWorld();
}

TRMain::~TRMain()
{
	delete active_world;
}

void TRMain::Update()
{

}
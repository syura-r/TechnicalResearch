#include "Sky.h"

#include "DrawMode.h"
#include "OBJLoader.h"

Sky::Sky()
{
	Create(OBJLoader::GetModel("sky"));
	position = { 0,-5,0 };
	scale = 5;
}

void Sky::Draw()
{
	//‰e–³‚µ
	if(Object3D::GetDrawShadow())
		return;
	CustomDraw(false,false,ALPHA,true);
}

void Sky::DrawReady()
{
	pipelineName = "Sky";
}

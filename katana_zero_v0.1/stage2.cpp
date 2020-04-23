#include "stdafx.h"
#include "stage2.h"

HRESULT stage2::init()
{
	DATA->setBgColImg(IMAGEMANAGER->findImage("stage2_bg_collision"));
	//DATA->setBgRenImg(IMAGEMANAGER->findImage("stage2_bg_render"));
	DATA->stageInitialize(4288,1000);
	DATA->setStartLoc(50, 600);

	playerPt = new playerClass;
	playerPt->init();

	return S_OK;
}

void stage2::release()
{
	playerPt->release();
	SAFE_DELETE(playerPt);
}

void stage2::update()
{
	playerPt->update();
}

void stage2::render()
{
	DATA->getBgCol().img->render(getMemDC(), -1 * DATA->getBgCol().x, -1 * DATA->getBgCol().y);
	//DATA->getBgRen().img->render(getMemDC(), -1 * DATA->getBgCol().x, -1 * DATA->getBgCol().y);
	playerPt->render();
}
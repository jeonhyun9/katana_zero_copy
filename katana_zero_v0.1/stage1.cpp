#include "stdafx.h"
#include "stage1.h"

HRESULT stage1::init()
{
	DATA->setBgColImg(IMAGEMANAGER->findImage("stage1_bg_collision"));
	DATA->setBgRenImg(IMAGEMANAGER->findImage("stage1_bg_render"));
	DATA->stageInitialize(2176,3500);
	DATA->setStartLoc(350, 100);

	playerPt = new playerClass;
	playerPt->init();

	enemyPt = new enemyClass;
	enemyPt->init();

	enemyPt->generateEnemy(1179, 998, E_BOLD);
	enemyPt->generateEnemy(1130, 998, E_BOLD);

	return S_OK;
}

void stage1::release()
{
	playerPt->release();
	SAFE_DELETE(playerPt);

	enemyPt->release();
	SAFE_DELETE(enemyPt);
}

void stage1::update()
{
	playerPt->update();
	enemyPt->update();

	/*충돌처리는 이런식으로*/
	for (int i = 0; i < enemyPt->getVectorEnemy().size(); i++)
	{
		RECT tempRECT;
		if (IntersectRect(&tempRECT,&playerPt->getPlayerRect(), &enemyPt->getEnemyRect(i)))
		{
			enemyPt->setEnemyState(i, E_HURT);
		}
	}
}

void stage1::render()
{
	DATA->getBgCol().img->render(getMemDC(), -1 * DATA->getBgCol().x, -1 * DATA->getBgCol().y);
	DATA->getBgRen().img->render(getMemDC(), -1 * DATA->getBgCol().x, -1 * DATA->getBgCol().y);

	playerPt->render();
	enemyPt->render();
}

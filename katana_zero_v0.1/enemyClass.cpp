#include "stdafx.h"
#include "enemyClass.h"

HRESULT enemyClass::init()
{
	return S_OK;
}

void enemyClass::release()
{
}

void enemyClass::update()
{
	for (int i = 0; i < vEnemy.size(); i++)
	{
		vEnemy[i].rc = RectMakeCenter(vEnemy[i].x - DATA->getBgCol().x, vEnemy[i].y - DATA->getBgCol().y, vEnemy[i].width, vEnemy[i].height);
		enemyAnimation(i);
	}
}

void enemyClass::render()
{
	for (int i = 0; i < vEnemy.size(); i++)
	{
		Rectangle(getMemDC(), vEnemy[i].rc);
		switch (vEnemy[i].state)
		{
		case E_IDLE:
			vEnemy[i].idleImg->frameRender(getMemDC(), vEnemy[i].rc.left, vEnemy[i].rc.top);
			break;
		}
	}
}

void enemyClass::generateEnemy(float _x, float _y, enum_EnemyCategory _category)
{
	sEnemy.x = _x;
	sEnemy.y = _y;
	sEnemy.isRight = false;
	sEnemy.category = _category;
	sEnemy.state = E_IDLE;
	sEnemy.frameCnt = sEnemy.frameIdx = 0;

	switch (sEnemy.category)
	{
	case E_BOLD:
		sEnemy.width = 30;
		sEnemy.height = 70;
		sEnemy.idleImg = IMAGEMANAGER->findImage("enemy_bold_idle");
		sEnemy.walkImg = IMAGEMANAGER->findImage("enemy_bold_walk");
		sEnemy.runImg = IMAGEMANAGER->findImage("enemy_bold_run");
		sEnemy.hurtImg = IMAGEMANAGER->findImage("enemy_bold_hurt");

		break;
	case E_GANG:
		break;
	}
	vEnemy.push_back(sEnemy);
}

void enemyClass::enemyAnimation(int i)
{
	switch (vEnemy[i].state)
	{
	case E_IDLE:
		if (vEnemy[i].isRight)
		{
			vEnemy[i].frameCnt++;
			vEnemy[i].idleImg->setFrameY(0);
			if (vEnemy[i].frameCnt % 7 == 0)
			{
				vEnemy[i].frameIdx++;
				if (vEnemy[i].frameIdx > 7)
				{
					vEnemy[i].frameIdx = 0;
				}
				vEnemy[i].idleImg->setFrameX(vEnemy[i].frameIdx);
			}
		}
		else
		{
			vEnemy[i].frameCnt++;
			vEnemy[i].idleImg->setFrameY(1);
			if (vEnemy[i].frameCnt % 7 == 0)
			{
				vEnemy[i].frameIdx--;
				if (vEnemy[i].frameIdx < 0)
				{
					vEnemy[i].frameIdx = 8;
				}
				vEnemy[i].idleImg->setFrameX(vEnemy[i].frameIdx);
			}
		}
		break;
	case E_WALK:
		break;
	}
}



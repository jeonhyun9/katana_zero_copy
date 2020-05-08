#include "stdafx.h"
#include "bossClass.h"

HRESULT bossClass::init()
{
	boss.isRight = false;
	boss.lazerReady = false;
	boss.lazerHitBox = false;
	boss.isGenrateCoolTime = false;
	boss.aimAngle = PI;
	boss.frameCnt = 0;
	boss.currentPattern = 0;
	boss.frameIdx = 0;
	boss.speed = 5.0f;
	boss.frameY = 0;
	boss.lazerY = boss.aimCnt = 0;
	boss.width = 44;
	boss.height = 88;
	boss.idleImg = IMAGEMANAGER->findImage("boss_headhunter_idle");
	boss.aimImg = IMAGEMANAGER->findImage("boss_headhunter_aim");
	boss.hurtRecoverImg = IMAGEMANAGER->findImage("boss_headhunter_hurt_recover");
	boss.aimLazerImg = IMAGEMANAGER->findImage("boss_aimlazer");
	boss.lazerImg = IMAGEMANAGER->findImage("boss_lazer");
	boss.drawGunImg = IMAGEMANAGER->findImage("boss_headhunter_drawgun");

	return S_OK;
}

void bossClass::release()
{
}

void bossClass::update()
{
	SLOW_inBoss = DATA->getSlow();
	SLOW_ratio_inBoss = DATA->getSlowRatio();

	if (INPUT->GetKeyDown(VK_F1))
	{
		boss.state = B_AIM;
	}
	
	//if (boss.state == B_IDLE)
	//{
	//	boss.aimCnt++;
	//	if (boss.aimCnt > 10)
	//	{
	//		boss.aimCnt = 0;
	//		boss.state = B_AIM;
	//	}
	//}

	if (DATA->getStop() == false)
	{
		if (boss.state == B_AIM)
		{
			boss.aimCnt++;
			if (boss.aimCnt > 40)
			{
				boss.lazerReady = true;
			}
		}

		boss.frameCnt++;
		boss.rc = RectMakeCenter(boss.x - DATA->getBgCol().x, boss.y - DATA->getBgCol().y, boss.width, boss.height);

		boss.findL = (boss.x - boss.width / 2) - boss.speed * 2;
		boss.findR = (boss.x + boss.width / 2) + boss.speed * 2;
		boss.findY = boss.y + boss.height / 2;
		boss.findB = boss.y + boss.height / 2;
		boss.centerX = boss.rc.left + boss.width / 2;
		boss.centerY = boss.rc.top + boss.height / 2;
		boss.centerX = boss.rc.left + boss.width / 2;
		boss.centerY = boss.rc.top + boss.height / 2;

		this->bossAnimation();
		this->bossOnFloor();
	}
	
}

void bossClass::render()
{
	//Rectangle(getMemDC(), boss.rc);
	//Rectangle(getMemDC(), boss.rcLazer);
	if (boss.isGenrateCoolTime == false)
	{
		switch (boss.state)
		{
		case B_IDLE:
			boss.idleImg->frameRender(getMemDC(), boss.rc.left, boss.rc.top, boss.frameIdx, boss.frameY);
			break;
		case B_AIM:
			int tempLazerX;
			boss.isRight ? boss.frameY = 0 : boss.frameY = 1;
			boss.isRight ? tempLazerX = boss.centerX + 55 - DATA->getBgCol().x : tempLazerX = 0 - boss.centerX + 10 - DATA->getBgCol().x;
			boss.aimImg->frameRender(getMemDC(), boss.rc.left - 34, boss.rc.top - 10, 0, boss.frameY);
			if (boss.isRight)
			{
				boss.aimLazerImg->alphaRender(getMemDC(), tempLazerX - 34 + DATA->getBgCol().x, boss.centerY - 15, 120);
				if (boss.lazerReady)
				{
					boss.lazerImg->frameRender(getMemDC(), tempLazerX - 34 + DATA->getBgCol().x, boss.centerY - 36, 0, boss.lazerY);
					boss.rcLazer = RectMake(tempLazerX - 34 + DATA->getBgCol().x, boss.centerY - 36, 2000, 50);
				}
			}
			else
			{
				boss.aimLazerImg->alphaRender(getMemDC(), tempLazerX + 190 - DATA->getBgCol().x, boss.centerY - 15, 120);
				if (boss.lazerReady)
				{
					boss.lazerImg->frameRender(getMemDC(), tempLazerX + 190 - DATA->getBgCol().x, boss.centerY - 36, 0, boss.lazerY);
					boss.rcLazer = RectMake(0, boss.centerY - 36, boss.centerX - 55, 50);
				}
			}
			break;
		case B_DRAWGUN:
			boss.drawGunImg->frameRender(getMemDC(), boss.rc.left - 30, boss.rc.top + 3, boss.frameIdx, boss.frameY);
			break;
		case B_HURTRECOVER:
			if (boss.isRight)
			{
				boss.hurtRecoverImg->frameRender(getMemDC(), boss.rc.left - 120, boss.rc.top - 15, boss.frameIdx, boss.frameY);
			}
			else
			{
				boss.hurtRecoverImg->frameRender(getMemDC(), boss.rc.left + 25, boss.rc.top - 15, boss.frameIdx, boss.frameY);
			}
			break;
		}
	}
	
}

void bossClass::bossAnimation()
{
	switch (boss.state)
	{
	case B_IDLE:
		this->animationSetFrame(6, 11);
		break;
	case B_AIM:
		if(boss.lazerReady)this->animationSetLazer();
		break;
	case B_HURTRECOVER:
		this->animationSetFrame(7, 10);
		break;
	case B_DRAWGUN:
		this->animationSetFrame(5, 7);
		break;
	}
}

void bossClass::animationSetFrame(int _frameTum, int _End)
{
	boss.frameTum = _frameTum;
	boss.oldFrameTum = boss.frameTum;
	if (SLOW_inBoss)
	{
		boss.frameTum *= SLOW_ratio_inBoss;
	}
	else
	{
		boss.frameTum = boss.oldFrameTum;
	}

	if (boss.isRight)
	{
		boss.frameY = 0;
		if (boss.frameCnt % boss.frameTum == 0)
		{
			boss.frameIdx++;
			if (boss.frameIdx > _End - 1)
			{
				if (boss.state == B_HURTRECOVER)boss.isGenrateCoolTime = true;
				if (boss.state == B_DRAWGUN)boss.state = B_AIM;
				boss.frameIdx = 0;
			}
		}
	}
	else
	{
		boss.frameY = 1;
		if (boss.frameCnt % boss.frameTum == 0)
		{
			boss.frameIdx--;
			if (boss.frameIdx < 0)
			{
				if (boss.state == B_HURTRECOVER)boss.isGenrateCoolTime = true;
				if (boss.state == B_DRAWGUN)boss.state = B_AIM;
				boss.frameIdx = _End;
			}
		}
	}
}

void bossClass::animationSetLazer()
{
	boss.frameTum = 4;
	boss.oldFrameTum = boss.frameTum;
	if (SLOW_inBoss)
	{
		boss.frameTum *= SLOW_ratio_inBoss;
	}
	else
	{
		boss.frameTum = boss.oldFrameTum;
	}

	if (boss.frameCnt % boss.frameTum == 0)
	{
		boss.lazerY++;
		boss.lazerY < 2 ? boss.lazerHitBox = true : boss.lazerHitBox = false;
		if (boss.lazerY > 5)
		{
			boss.lazerY = 0;
			//boss.state = B_IDLE;
			boss.aimCnt = 0;
			boss.lazerReady = false;
		}
	}
}



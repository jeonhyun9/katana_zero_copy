#include "stdafx.h"
#include "bossClass.h"

HRESULT bossClass::init()
{
	boss.isRight = false;
	boss.lazerReady = false;
	boss.lazerHitBox = false;
	boss.isGenrateCoolTime = false;
	boss.isLazerGenerated = false;
	boss.distance = 0;
	animationOff = false;

	for (int i = 0; i < 6; i++)
	{
		pattern[i] = i;
	}

	preLocation = RANDOM->Range(1, 3);
	prePattern = B_IDLE;

	DATA->setBossHit(false);

	boss.isWallJump = false;
	boss.isFire = false;
	boss.aimAngle = PI;
	boss.frameCnt = 0;
	boss.currentPattern = 0;
	boss.frameIdx = 0;
	boss.speed = 5.0f;
	boss.jumpSpeed = 10.0f;
	boss.wallJumpSpeed = 18.0f;
	boss.fallSpeed = 10.0f;
	boss.dashSpeed = 100.0f;
	boss.frameY = 0;
	boss.stateCnt = boss.lazerY = boss.aimCnt = boss.headFrameIdx = boss.headFrameCnt = boss.headFrameY = 0;
	boss.width = 44;
	boss.height = 88;
	boss.idleImg = IMAGEMANAGER->findImage("boss_idle");
	boss.aimImg = IMAGEMANAGER->findImage("boss_aim");
	boss.hurtRecoverImg = IMAGEMANAGER->findImage("boss_recover");
	boss.aimLazerImg = IMAGEMANAGER->findImage("boss_aimlazer");
	boss.lazerImg = IMAGEMANAGER->findImage("boss_lazer");
	boss.drawRifleImg = IMAGEMANAGER->findImage("boss_drawgun");
	boss.landImg = IMAGEMANAGER->findImage("boss_land");
	boss.dashEndImg = IMAGEMANAGER->findImage("boss_dash_end");
	boss.preDashImg = IMAGEMANAGER->findImage("boss_predash");
	boss.sweepImg = IMAGEMANAGER->findImage("boss_sweep");
	boss.drawGunImg = IMAGEMANAGER->findImage("boss_takeoutgun");
	boss.shootImg = IMAGEMANAGER->findImage("boss_shoot");
	boss.teleportImg = IMAGEMANAGER->findImage("boss_teleport");
	boss.dashImg = IMAGEMANAGER->findImage("boss_dash");
	boss.teleportGroundImg = IMAGEMANAGER->findImage("boss_teleport_ground");
	boss.inImg = IMAGEMANAGER->findImage("boss_in");
	boss.outImg = IMAGEMANAGER->findImage("boss_out");

	boss.dieGroundImg = IMAGEMANAGER->findImage("boss_dieground");
	boss.headGroundImg = IMAGEMANAGER->findImage("boss_headground");
	boss.headFlyImg = IMAGEMANAGER->findImage("boss_headfly");
	boss.dieFlyImg = IMAGEMANAGER->findImage("boss_diefly");
	boss.noHeadImg = IMAGEMANAGER->findImage("boss_nohead");
	boss.deadImg = IMAGEMANAGER->findImage("boss_dead");

	boss.gravity = 0;
	boss.moveAngle = PI / 2;
	boss.shootAngle = 0;

	boss.preJumpImg = IMAGEMANAGER->findImage("boss_prejump");
	boss.jumpImg = IMAGEMANAGER->findImage("boss_jump");
	boss.wallGrabImg = IMAGEMANAGER->findImage("boss_wallgrab");
	boss.wallJumpImg = IMAGEMANAGER->findImage("boss_walljump");

	bulletPt = new bossBullet;
	bulletPt->init(20,1200);

	minePt = new bossMine;
	minePt->init(15, 300);

	return S_OK;
}

void bossClass::release()
{
	bulletPt->release();
	SAFE_DELETE(bulletPt);

	minePt->release();
	SAFE_DELETE(minePt);
}

void bossClass::update()
{
	bulletPt->update();

	minePt->update();

	SLOW_inBoss = DATA->getSlow();
	SLOW_ratio_inBoss = DATA->getSlowRatio();
	DATA->setBossRect(boss.rc);
	DATA->setBossData(boss);

	if (DATA->getBossHit() == true)
	{
		this->setBossHit(boss.isRight ? false : true);
	}

	if (INPUT->GetKeyDown(VK_F1))
	{
		boss.state = B_AIM;
	}

	if (DATA->getStop() == false)
	{
		//레이저 애니메이션
		if (vLazer.empty() == false)
		{
			for (int i = 0; i < vLazer.size(); i++)
			{
				vLazer[i].frameCnt++;
				if (vLazer[i].isRotate)
				{
					if (vLazer[i].frameCnt % 4 == 0)
					{
						if (boss.frameIdx > 1 && boss.frameIdx < 25 && boss.state == B_SWEEP)
						{
							vLazer[i].isRight ? vLazer[i].angle -= 0.19f : vLazer[i].angle += 0.19f;
						}
						
						vLazer[i].frameIdx++;
						if (vLazer[i].frameIdx > 9)
						{
							vLazer[i].frameIdx = 10;
						}
					}
				}
				else
				{
					if (vLazer[i].frameCnt % 4 == 0)
					{
						vLazer[i].frameIdx++;
						if (vLazer[i].frameIdx == 4)SOUNDMANAGER->play("lazer",0.5f);
						if (vLazer[i].frameIdx > 9)
						{
							vLazer[i].frameIdx = 10;
							vLazer.erase(vLazer.begin() + i);
						}
					}
				}
			}
		}

		//보스 죽음
		if (boss.state == B_DIEFLY)
		{
			boss.stateCnt++;
			boss.isRight ? boss.moveAngle = PI -1 : boss.moveAngle = 1;
			boss.gravity += 0.4f;
			if (boss.isCrashedLeft == false && boss.isCrashedRight == false)boss.x += cosf(boss.moveAngle) * (boss.jumpSpeed / SLOW_ratio_inBoss);
			boss.y += -sinf(boss.moveAngle) * (boss.fallSpeed / SLOW_ratio_inBoss) + (boss.gravity / SLOW_ratio_inBoss);
			if (boss.isCrashedBottom == true && boss.stateCnt > 10)
			{
				boss.stateCnt = 0;
				boss.isRight ? boss.isRight = false : boss.isRight = true;
				setBossState(B_DIEGROUND);
			}
		}
		
		//보스 일자 레이저
		if (boss.state == B_AIM)
		{
			boss.aimCnt++;
			if (boss.aimCnt > 40)
			{
				boss.lazerReady = true;
			}
		}

		//보스 머리 굴러다님
		if (boss.state == B_NOHEAD && boss.headState != H_GROUND)
		{
			boss.stateCnt++;
			boss.isRight ? boss.moveAngle = PI - 1 : boss.moveAngle = 1;
			boss.gravity += (0.4f / SLOW_ratio_inBoss);
			if (boss.isCrashedLeft == false && boss.isCrashedRight == false)boss.x += cosf(boss.moveAngle) * (boss.jumpSpeed / SLOW_ratio_inBoss);
			boss.y += -sinf(boss.moveAngle) * (boss.fallSpeed / SLOW_ratio_inBoss) + (boss.gravity / SLOW_ratio_inBoss);
			if (boss.isCrashedBottom == true && boss.stateCnt > 10)
			{
				boss.stateCnt = 0;
				if (boss.isCrashedLeft == true)
				{
					boss.isRight = true;
					boss.isRight ? boss.headFrameIdx = 0 : boss.headFrameIdx = 8;
				}
				else if (boss.isCrashedRight == true)
				{
					boss.isRight = false;
					boss.isRight ? boss.headFrameIdx = 0 : boss.headFrameIdx = 8;
				}
				else
				{
					boss.isRight ? boss.isRight = false : boss.isRight = true;
					boss.isRight ? boss.headFrameIdx = 0 : boss.headFrameIdx = 8;
				}
				boss.headState = H_GROUND;
			}
		}

		//기어다니기
		if (boss.state == B_DEAD)
		{
			if (boss.isRight && boss.isCrashedRight == false)boss.x += 0.3f;
			if (boss.isRight == false && boss.isCrashedLeft == false)boss.x -= 0.3f;
		}

		//벽으로 점프
		if (boss.state == B_JUMP && boss.isWallJump==false)
		{
			boss.isRight ? boss.moveAngle = 1 : boss.moveAngle = PI - 1;
			if (SLOW_inBoss == false)boss.gravity += 0.1f;
			boss.x += cosf(boss.moveAngle) * (boss.jumpSpeed / SLOW_ratio_inBoss);
			boss.y += -sinf(boss.moveAngle) * (boss.jumpSpeed / SLOW_ratio_inBoss) + (boss.gravity / SLOW_ratio_inBoss);
			if (boss.isCrashedRight == true)
			{
				setBossState(B_WALLGRAB);
				boss.stateCnt = 0;
				boss.shootAngle = 0;
				boss.gravity = 0;
			}
			if (boss.isCrashedLeft == true)
			{
				setBossState(B_WALLGRAB);
				boss.stateCnt = 0;
				boss.shootAngle = PI;
				boss.gravity = 0;
			}
		}

		//벽점프+총알발사
		if (boss.isWallJump)
		{
			boss.stateCnt++;
			if (boss.stateCnt > 25 && boss.state !=B_WALLJUMP)
			{
				setBossState(B_WALLJUMP);
				boss.isFire = true;
			}
			boss.isRight ? boss.moveAngle = 1 : boss.moveAngle = PI - 1;
			boss.gravity += (0.22f / SLOW_ratio_inBoss);
			boss.x += cosf(boss.moveAngle) * (boss.wallJumpSpeed/ SLOW_ratio_inBoss);
			boss.y += -sinf(boss.moveAngle) * (boss.jumpSpeed/ SLOW_ratio_inBoss) + (boss.gravity /SLOW_ratio_inBoss);
			if (boss.isCrashedBottom == true)
			{
				boss.gravity = 0;
				boss.isWallJump = false;
				boss.stateCnt = 0;
				this->bossOnFloor();
				setBossState(B_LAND);
			}
			if (boss.state == B_WALLJUMP)
			{
				if (boss.stateCnt > 2 * SLOW_ratio_inBoss)
				{
					boss.stateCnt = 0;
					if (boss.isRight && boss.frameIdx < 7)
					{
						bulletPt->fire(boss.x - DATA->getBgCol().x, boss.y + 70 - DATA->getBgCol().y, boss.shootAngle);
						boss.shootAngle += 0.32f;
					}
					if (boss.isRight==false && boss.frameIdx > 0)
					{
						bulletPt->fire(boss.x - DATA->getBgCol().x, boss.y + 70 - DATA->getBgCol().y, boss.shootAngle);
						boss.shootAngle -= 0.32f;
					}
				}
			}
		}

		//대쉬
		if (boss.state == B_DASH)
		{
			boss.stateCnt++;
			boss.height = 50;
			boss.isRight ? boss.moveAngle = 1 : boss.moveAngle = PI - 1;
			boss.x += cosf(boss.moveAngle) * (boss.dashSpeed / SLOW_ratio_inBoss);
			if (boss.stateCnt > 15 * SLOW_ratio_inBoss)
			{
				boss.stateCnt = 0;
				setBossState(B_DASH_END);
			}
		}
		else if (boss.state !=B_NOHEAD)
		{
			boss.height = 88;
		}

		//레이저 텔포
		if (boss.state == B_TELEPORT)
		{
			switch (boss.stateCnt)
			{
			case 0:
				boss.x = 221;
				boss.y = 222;
				if (boss.isLazerGenerated == false)
				{
					this->generateLazer(boss.x, boss.y, 0);
					boss.isLazerGenerated = true;
				}
				break;
			case 1:
				boss.x = 1148;
				boss.y = 222;
				if (boss.isLazerGenerated == false)
				{
					this->generateLazer(boss.x, boss.y, 0);
					boss.isLazerGenerated = true;
				}
				break;
			case 2:
				boss.x = 348;
				boss.y = 222;
				if (boss.isLazerGenerated == false)
				{
					this->generateLazer(boss.x, boss.y, 0);
					boss.isLazerGenerated = true;
				}
				break;
			case 3:
				boss.x = 974;
				boss.y = 222;
				if (boss.isLazerGenerated == false)
				{
					this->generateLazer(boss.x, boss.y, 0);
					boss.isLazerGenerated = true;
				}
				break;
			case 4:
				if (boss.isLazerGenerated == false)
				{
					this->generateLazer(boss.x, boss.y, 0);
					boss.isLazerGenerated = true;
				}
				break;
			case 5:
				if (boss.isLazerGenerated == false)
				{
					this->generateLazer(boss.x, boss.y, 0);
					boss.isLazerGenerated = true;
				}
			case 6:
				if (boss.isLazerGenerated == false)
				{
					this->generateLazer(boss.x, boss.y, 0);
					boss.isLazerGenerated = true;
				}
				break;
			}
		}
		
		//스윕 텔포
		if (boss.state == B_SWEEP)
		{
			switch (boss.stateCnt)
			{
			case 0:
				boss.x = 221;
				boss.y = 222;
				if (boss.isLazerGenerated == false && boss.frameIdx == 18)
				{
					this->generateRotateLazer(boss.x, boss.y, false, 0);
					boss.isLazerGenerated = true;
				}
				break;
			case 1:
				boss.x = 1198;
				if (boss.isLazerGenerated == false && boss.frameIdx == 8)
				{
					this->generateRotateLazer(boss.x, boss.y, true, 0);
					boss.isLazerGenerated = true;
				}
				break;
			}
		}

		//지상 텔포
		if (boss.state == B_TELEPORT_GROUND)
		{
			boss.x = 210;
			boss.y = 530;
			if (boss.isLazerGenerated == false && boss.frameIdx == 4)
			{
				this->generateLazer(boss.x, boss.y,true, 0);
				boss.isLazerGenerated = true;
			}
		}

		boss.frameCnt++;
		boss.headFrameCnt++;
		boss.rc = RectMakeCenter(boss.x - DATA->getBgCol().x, boss.y - DATA->getBgCol().y, boss.width, boss.height);

		boss.findL = (boss.x - boss.width / 2) - boss.speed * 2;
		boss.findR = (boss.x + boss.width / 2) + boss.speed * 2;
		boss.findY = boss.y + boss.height / 2;

		if (boss.state == B_HURTRECOVER)
		{
			boss.findB = (boss.y + boss.height / 2) + 25;
		}
		else if (boss.state == B_DIEFLY)
		{
			boss.findB = (boss.y + boss.height / 2) - 10;
		}
		else
		{
			boss.findB = (boss.y + boss.height / 2);
		}

		if (boss.distance < 100 && boss.state == B_IN)
		{
			setBossState(B_OUT);
		}
		
		boss.centerX = boss.rc.left + boss.width / 2;
		boss.centerY = boss.rc.top + boss.height / 2;
		boss.centerX = boss.rc.left + boss.width / 2;
		boss.centerY = boss.rc.top + boss.height / 2;
		if (animationOff == false)
		{
			this->bossAnimation();
		}
	
		if (boss.state == B_IDLE || boss.state == B_AIM || boss.state == B_LAND || boss.state == B_DIEGROUND || boss.state == B_DEAD || boss.state ==B_HURTRECOVER)
		{
			this->bossOnFloor();
		}
		this->bossPixelCollision();
		this->headAnimation();
	}
	
}

void bossClass::render()
{
	bulletPt->render();
	minePt->render();
	//Rectangle(getMemDC(), boss.rc);
	//Rectangle(getMemDC(), boss.rcLazer);
	if (vLazer.empty() == false)
	{
		for (int i = 0; i < vLazer.size(); i++)
		{
			if (vLazer[i].isGround && vLazer[i].isRotate == true)
			{
				if (boss.isRight)
				{
					if (boss.frameIdx > 3 && boss.frameIdx < 22)
					{
						//vLazer[i].img->rotateRender(getMemDC(), vLazer[i].x - DATA->getBgCol().x - 40 , vLazer[i].y - DATA->getBgCol().y, vLazer[i].angle);
					}
					if (boss.frameIdx == 23)
					{
						vLazer.erase(vLazer.begin() + i);
					}
				}
				else
				{ 
					if (boss.frameIdx < 18 && boss.frameIdx > 3)
					{
						//vLazer[i].img->rotateRender(getMemDC(), vLazer[i].x - DATA->getBgCol().x - 30, vLazer[i].y - DATA->getBgCol().y, vLazer[i].angle);
					}
					if (boss.frameIdx == 2)
					{
						vLazer.erase(vLazer.begin() + i);
					}
				}
			}
			else if (vLazer[i].isGround && vLazer[i].isRotate == false)
			{
				//가로 레이저
				vLazer[i].rc = RectMake(vLazer[i].x - DATA->getBgCol().x + 26, vLazer[i].y - DATA->getBgCol().y - 13, 1500, 36);
				vLazer[i].img->frameRender(getMemDC(), vLazer[i].x - DATA->getBgCol().x + 26, vLazer[i].y - DATA->getBgCol().y - 13, 0, vLazer[i].frameIdx);
				//Rectangle(getMemDC(), vLazer[i].rc);
			}
			else
			{
				//세로 레이저
				vLazer[i].rc = RectMake(vLazer[i].x - DATA->getBgCol().x - 32, vLazer[i].y - DATA->getBgCol().y + 45, 36, 1500);
				vLazer[i].img->frameRender(getMemDC(), vLazer[i].x - DATA->getBgCol().x - 32, vLazer[i].y - DATA->getBgCol().y + 45, vLazer[i].frameIdx, 0);
				//Rectangle(getMemDC(), vLazer[i].rc);
			}
		}
	}

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
		case B_DRAWRIFLE:
			boss.drawRifleImg->frameRender(getMemDC(), boss.rc.left - 36, boss.rc.top + 3, boss.frameIdx, boss.frameY);
			break;
		case B_HURTRECOVER:
			if (boss.isRight)
			{
				boss.hurtRecoverImg->frameRender(getMemDC(), boss.rc.left - 120, boss.rc.top - 17, boss.frameIdx, boss.frameY);
			}
			else
			{
				boss.hurtRecoverImg->frameRender(getMemDC(), boss.rc.left + 25, boss.rc.top - 17, boss.frameIdx, boss.frameY);
			}
			break;
		case B_PREJUMP:
			boss.preJumpImg->frameRender(getMemDC(), boss.rc.left -22, boss.rc.top + 3, boss.frameIdx, boss.frameY);
			break;
		case B_JUMP:
			boss.jumpImg->frameRender(getMemDC(), boss.rc.left - 10, boss.rc.top, 0, boss.frameY);
			break;
		case B_WALLGRAB:
			if (boss.isRight)
			{
				boss.wallGrabImg->frameRender(getMemDC(), boss.rc.left - 16, boss.rc.top + 2, boss.frameIdx, boss.frameY);
			}
			else
			{
				boss.wallGrabImg->frameRender(getMemDC(), boss.rc.left , boss.rc.top + 2, boss.frameIdx, boss.frameY);
			}
			break;
		case B_WALLJUMP:
			if (boss.isRight)
			{
				if (boss.frameIdx != 7)
				{
					IMAGEMANAGER->findImage("boss_walljump_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 46, boss.rc.top - 46, boss.frameIdx - 3, boss.frameY, 40);
					IMAGEMANAGER->findImage("boss_walljump_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 36, boss.rc.top - 36, boss.frameIdx - 2, boss.frameY, 80);
					IMAGEMANAGER->findImage("boss_walljump_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 26, boss.rc.top - 26, boss.frameIdx - 1, boss.frameY, 120);
				}
				boss.wallJumpImg->frameRender(getMemDC(), boss.rc.left - 16, boss.rc.top - 16, boss.frameIdx, boss.frameY);
			}
			else
			{
				if (boss.frameIdx != 0)
				{
					IMAGEMANAGER->findImage("boss_walljump_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 11, boss.rc.top - 46, boss.frameIdx - 3, boss.frameY, 40);
					IMAGEMANAGER->findImage("boss_walljump_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 21, boss.rc.top - 36, boss.frameIdx - 2, boss.frameY, 80);
					IMAGEMANAGER->findImage("boss_walljump_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 31, boss.rc.top - 26, boss.frameIdx - 1, boss.frameY, 120);
				}
				boss.wallJumpImg->frameRender(getMemDC(), boss.rc.left - 41, boss.rc.top - 16, boss.frameIdx, boss.frameY);
			}
			break;
		case B_LAND:
			boss.landImg->frameRender(getMemDC(), boss.rc.left -22, boss.rc.top + 2, boss.frameIdx, boss.frameY);
			break;
		case B_DASH_PRE:
			IMAGEMANAGER->findImage("boss_warning")->alphaRender(getMemDC(), 270 - DATA->getBgCol().x, boss.rc.top + 44,120);
			boss.preDashImg->frameRender(getMemDC(), boss.rc.left - 70, boss.rc.top + 2, boss.frameIdx, boss.frameY);
			break;
		case B_DASH:
			boss.isRight ? boss.frameY = 0 : boss.frameY = 1;
			if (boss.isRight)
			{
				IMAGEMANAGER->findImage("boss_dash_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 228, boss.rc.top + 8, 0, boss.frameY, 60);
				IMAGEMANAGER->findImage("boss_dash_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 188, boss.rc.top + 8, 0, boss.frameY, 80);
				IMAGEMANAGER->findImage("boss_dash_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 148, boss.rc.top + 8, 0, boss.frameY, 100);
				IMAGEMANAGER->findImage("boss_dash_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 108, boss.rc.top + 8, 0, boss.frameY, 120);
				IMAGEMANAGER->findImage("boss_dash_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 68, boss.rc.top + 8, 0, boss.frameY, 140);
			}
			else
			{
				IMAGEMANAGER->findImage("boss_dash_shadow")->alphaFrameRender(getMemDC(), boss.rc.left + 118, boss.rc.top + 8, 0, boss.frameY, 60);
				IMAGEMANAGER->findImage("boss_dash_shadow")->alphaFrameRender(getMemDC(), boss.rc.left + 78, boss.rc.top + 8, 0, boss.frameY, 80);
				IMAGEMANAGER->findImage("boss_dash_shadow")->alphaFrameRender(getMemDC(), boss.rc.left + 38, boss.rc.top + 8, 0, boss.frameY, 100);
				IMAGEMANAGER->findImage("boss_dash_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 8, boss.rc.top + 8, 0, boss.frameY, 120);
				IMAGEMANAGER->findImage("boss_dash_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 48, boss.rc.top + 8, 0, boss.frameY, 140);
			}
			boss.dashImg->frameRender(getMemDC(), boss.rc.left - 58, boss.rc.top + 8, 0, boss.frameY);
			break;
		case B_DASH_END:
			boss.dashEndImg->frameRender(getMemDC(), boss.rc.left - 10, boss.rc.top - 4, boss.frameIdx, boss.frameY);
			break;
		case B_TELEPORT:
			boss.teleportImg->frameRender(getMemDC(), boss.rc.left - 12, boss.rc.top - 2, boss.frameIdx, boss.frameY);
			break;
		case B_SWEEP:
			boss.sweepImg->frameRender(getMemDC(), boss.rc.left - 74, boss.rc.top - 4, boss.frameIdx, boss.frameY);
			break;
		case B_TELEPORT_GROUND:
			boss.teleportGroundImg->frameRender(getMemDC(), boss.rc.left - 38, boss.rc.top + 24, boss.frameIdx, boss.frameY);
			break;
		case B_DRAWGUN:
			boss.drawGunImg->frameRender(getMemDC(), boss.rc.left - 24, boss.rc.top + 2, boss.frameIdx, boss.frameY);
			break;
		case B_SHOOT:
			if (boss.isRight)
			{
				boss.shootImg->frameRender(getMemDC(), boss.rc.left - 22, boss.rc.top - 1, boss.frameIdx, boss.frameY);
			}
			else
			{
				boss.shootImg->frameRender(getMemDC(), boss.rc.left - 70, boss.rc.top - 1, boss.frameIdx, boss.frameY);
			}
			break;
		case B_IN:
			boss.inImg->frameRender(getMemDC(), boss.rc.left - 14, boss.rc.top + 28, boss.frameIdx, boss.frameY);
			break;
		case B_OUT:
			boss.outImg->frameRender(getMemDC(), boss.rc.left - 14, boss.rc.top + 28, boss.frameIdx, boss.frameY);
			break;
		case B_DIEGROUND:
			boss.dieGroundImg->frameRender(getMemDC(), boss.rc.left - 14, boss.rc.top + 6, boss.frameIdx, boss.frameY);
			break;
		case B_HEADGROUND:
			break;
		case B_HEADFLY:
			break;
		case B_DIEFLY:
			boss.dieFlyImg->frameRender(getMemDC(), boss.rc.left - 19, boss.rc.top + 18, boss.frameIdx, boss.frameY);
			break;
		case B_NOHEAD:
			switch (boss.headState)
			{
			case H_FLY:
				boss.headFlyImg->frameRender(getMemDC(), boss.rc.left, boss.rc.top +3, boss.headFrameIdx, boss.headFrameY);
				break;
			case H_GROUND:
				if (boss.isRight)
				{
					boss.headGroundImg->frameRender(getMemDC(), boss.rc.left, boss.rc.top + 3, boss.headFrameIdx, boss.headFrameY);
				}
				else
				{
					boss.headGroundImg->frameRender(getMemDC(), boss.rc.left - 50, boss.rc.top + 3, boss.headFrameIdx, boss.headFrameY);
				}
				break;
			}
			boss.noHeadImg->frameRender(getMemDC(), boss.noHead_x - DATA->getBgCol().x, boss.noHead_y - DATA->getBgCol().y, boss.frameIdx, boss.frameY);
			break;
		case B_DEAD:
			boss.deadImg->frameRender(getMemDC(), boss.rc.left - 14, boss.rc.top + 46, boss.frameIdx, boss.frameY);
			break;
		}
	}

	char str[256];
	//글자배경모드
	SetBkMode(getMemDC(), TRANSPARENT);
	//글자색상
	SetTextColor(getMemDC(), RGB(255, 0, 255));
	sprintf(str, "%d", boss.currentPattern);
	TextOut(getMemDC(), 10, 10, str, strlen(str));
}

void bossClass::headAnimation()
{
	
	switch (boss.headState)
	{
	case H_FLY:
		if (boss.isRight)
		{
			boss.headFrameY = 0;
			if (boss.headFrameCnt % 5 == 0)
			{
				boss.headFrameIdx++;
				if (boss.headFrameIdx > 7)
				{
					boss.headFrameIdx = 0;
				}
			}
		}
		else
		{
			boss.headFrameY = 1;
			if (boss.headFrameCnt % 5 == 0)
			{
				boss.headFrameIdx--;
				if (boss.headFrameIdx < 2)
				{
					boss.headFrameIdx = 8;
				}
			}
		}		
		break;
	case H_GROUND:
		if (boss.isRight)
		{
			boss.headFrameY = 0;
			if (boss.headFrameCnt % 5 == 0)
			{
				boss.headFrameIdx++;
				if (boss.headFrameIdx > 7)
				{
					boss.headFrameIdx = 8;
				}
			}
		}
		else
		{
			boss.headFrameY = 1;
			if (boss.headFrameCnt % 5 == 0)
			{
				boss.headFrameIdx--;
				if (boss.headFrameIdx < 1)
				{
					boss.headFrameIdx = 0;
				}
			}
		}
		break;
	}
}

void bossClass::generateLazer(float _x, float _y, int _frameidx)
{
	SOUNDMANAGER->play("aim_v",0.5f);
	sLazer.isGround = false;
	sLazer.isRotate = false;
	sLazer.img = IMAGEMANAGER->findImage("boss_lazer_top");
	sLazer.frameIdx = _frameidx;
	sLazer.frameCnt = 0;
	sLazer.x = _x;
	sLazer.y = _y;

	vLazer.push_back(sLazer);
}

void bossClass::generateLazer(float _x, float _y, bool _set, int _frameidx)
{
	SOUNDMANAGER->play("aim_h", 0.5f);
	sLazer.isGround = true;
	sLazer.isRotate = false;
	sLazer.img = IMAGEMANAGER->findImage("boss_lazer_ground");
	sLazer.frameIdx = _frameidx;
	sLazer.frameCnt = 0;
	sLazer.x = _x;
	sLazer.y = _y;

	vLazer.push_back(sLazer);
}

void bossClass::generateRotateLazer(float _x, float _y, bool _set, int _frameidx)
{
	sLazer.isGround = true;
	sLazer.isRotate = true;
	sLazer.isRight = _set;
	_set ? sLazer.angle = -0.72 : sLazer.angle = PI + 0.5;
	sLazer.img = IMAGEMANAGER->findImage("boss_lazer_ground_small");
	sLazer.frameIdx = _frameidx;
	sLazer.frameCnt = 0;
	sLazer.x = _x;
	sLazer.y = _y;

	vLazer.push_back(sLazer);
}

void bossClass::setBossHit(bool _set)
{
	DATA->setBossHit(false);
	if (DATA->getCurrentClear() == STAGE3_CLEARED)
	{
		boss.state = B_HURTRECOVER;
		boss.currentPattern++;
		boss.gravity = 0;
		boss.isWallJump = false;
		boss.stateCnt = 0;
		boss.lazerY = 0;
		boss.isRight = _set;
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 10;
	}
	else
	{
		boss.gravity = 0;
		boss.isWallJump = false;
		boss.stateCnt = 0;
		boss.isRight = _set;
		if (boss.currentPattern == 8)
		{
			boss.noHead_x = boss.x;
			boss.noHead_y = boss.y;
			boss.headState = H_FLY;
			setBossState(B_NOHEAD);
			boss.height = 42;
			boss.width = 40;
		}
		if (boss.currentPattern <= 6)
		{
			setBossState(B_HURTRECOVER);
			boss.currentPattern++;
		}
		if (boss.currentPattern == 7)
		{
			setBossState(B_DIEFLY);
			boss.currentPattern++;
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
		if (boss.isCrashedBottom)
		{
			this->animationSetFrame(4, 13);
		}
		else
		{
			this->animationSetFrame(4, 13);
		}
		break;
	case B_DRAWRIFLE:
		this->animationSetFrame(5, 7);
		break;
	case B_PREJUMP:
		this->animationSetFrame(7, 3);
		break;
	case B_WALLGRAB:
		this->animationSetFrame(7, 3);
		break;
	case B_WALLJUMP:
		this->animationSetFrame(4, 7);
		break;
	case B_LAND:
		this->animationSetFrame(5, 4);
		break;
	case B_DASH_END:
		this->animationSetFrame(5, 10);
		break;
	case B_DASH_PRE:
		this->animationSetFrame(5, 8);
		break;
	case B_SWEEP:
		this->animationSetFrame(4, 26);
		break;
	case B_DRAWGUN:
		this->animationSetFrame(4, 7);
		break;
	case B_SHOOT:
		this->animationSetFrame(4, 8);
		break;
	case B_TELEPORT:
		this->animationSetFrame(3, 8);
		break;
	case B_TELEPORT_GROUND:
		this->animationSetFrame(3, 8);
		break;
	case B_IN:
		this->animationSetFrame(5, 4);
		break;
	case B_OUT:
		this->animationSetFrame(5, 4);
		break;
	case B_DIEGROUND:
		this->animationSetFrame(3, 8);
		break;
	case B_HEADGROUND:
		this->animationSetFrame(5, 8);
		break;
	case B_HEADFLY:
		this->animationSetFrame(5, 8);
		break;
	case B_DIEFLY:
		this->animationSetFrame(5, 4);
		break;
	case B_NOHEAD:
		this->animationSetFrame(5, 6);
		break;
	case B_DEAD:
		this->animationSetFrame(5, 18);
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
		if (boss.state == B_HURTRECOVER && boss.isCrashedBottom == false && DATA->getCurrentClear()!= STAGE3_CLEARED)
		{
			boss.frameIdx = 1;
			boss.isRight ? boss.moveAngle = PI - 1 : boss.moveAngle = 1;
			if (SLOW_inBoss == false)boss.gravity += 0.4f;
			if (boss.isCrashedLeft == false && boss.isCrashedRight == false)boss.x += cosf(boss.moveAngle) * (boss.jumpSpeed / SLOW_ratio_inBoss);
			boss.y += -sinf(boss.moveAngle) * (boss.fallSpeed / SLOW_ratio_inBoss) + (boss.gravity / SLOW_ratio_inBoss);
		}
		else if (boss.state == B_HURTRECOVER && boss.isCrashedBottom == true && boss.frameIdx > 12 && DATA->getCurrentClear() != STAGE3_CLEARED)
		{
			boss.gravity = 0;
			boss.stateCnt = 0;
			setBossState(B_OUT);
		}
		else if (boss.frameCnt % boss.frameTum == 0)
		{
			if (boss.state == B_HURTRECOVER && boss.isCrashedBottom == false && boss.frameIdx == 1)
			{
				boss.frameIdx = 1;
			}
			else
			{
				boss.frameIdx++;
			}
			if (boss.frameIdx == 2)
			{
				if (boss.state == B_SHOOT)
				{
					int random = RANDOM->Range(0, 5);
					float add;
					switch (random)
					{
					case 0:
						add = 0.0f;
						break;
					case 1:
						add = 0.05f;
						break;
					case 2:
						add = 0.1f;
						break;
					case 3:
						add = 0.15f;
						break;
					case 4:
						add = 0.2f;
						break;
					case 5:
						add = 0.25f;
						break;
					}
					boss.isRight ? boss.shootAngle = 0 : boss.shootAngle = PI;
					minePt->fire(boss.x + 50, boss.y, boss.shootAngle + add);
					boss.stateCnt++;
				}
			}
			if (boss.frameIdx > _End)
			{
				switch (boss.state)
				{
				case B_HURTRECOVER:boss.isGenrateCoolTime = true;
					boss.gravity = 0;
					break;
				case B_DRAWRIFLE:
					SOUNDMANAGER->play("aim_h", 0.5f);
					boss.state = B_AIM;
					break;
				case B_WALLGRAB:
					boss.isRight ? boss.isRight = false : boss.isRight = true;
					boss.isWallJump = true;
					boss.state = B_JUMP;
					break;
				case B_PREJUMP:
					boss.isRight ? boss.isRight = false : boss.isRight = true;
					boss.state = B_JUMP;
					break;
				case B_DASH_PRE:
					boss.state = B_DASH;
					break;
				case B_WALLJUMP:boss.frameIdx = _End;
					boss.isFire = false;
					break;
				case B_LAND:
					SOUNDMANAGER->play("p_land");
					boss.stateCnt = 0;
					setBossState(B_OUT);
					break;
				case B_DASH_END:
					boss.isRight ? boss.isRight = false : boss.isRight = true;
					boss.stateCnt = 0;
					setBossState(B_OUT);
					break;
				case B_TELEPORT:
					if (boss.stateCnt < 3)
					{
						boss.isLazerGenerated = false;
						boss.stateCnt++;
						boss.frameIdx = 0;
					}
					else if (boss.stateCnt < 6)
					{
						boss.isLazerGenerated = false;
						boss.stateCnt++;
						boss.x = DATA->getPlayerData().x;
						boss.frameIdx = 0;
					}
					else
					{
						boss.isLazerGenerated = false;
						boss.stateCnt = 0;
						int rand = RANDOM->Range(1, 2);
						if (rand == 1)
						{
							setBossState(B_TELEPORT_GROUND);
						}
						else
						{
							boss.x = 1080;
							boss.y = 530;
							boss.isRight = false;
							prePattern = B_DASH_PRE;
							setBossState(B_DASH_PRE);
						}
					}
					break;
				case B_SWEEP:
					if (boss.stateCnt == 0)
					{
						boss.isLazerGenerated = false;
						boss.stateCnt++;
						boss.frameIdx = _End;
						boss.isRight = false;
					}
					else
					{
						boss.isLazerGenerated = false;
						boss.stateCnt = 0;
						setBossState(B_TELEPORT_GROUND);
					}
					break;
				case B_TELEPORT_GROUND:
					boss.stateCnt = 0;
					boss.isLazerGenerated = false;
					setBossState(B_OUT);
					break;
				case B_DRAWGUN:
					setBossState(B_SHOOT);
					break;
				case B_SHOOT:
					if (boss.stateCnt == 3)
					{
						boss.stateCnt = 0;
						setBossState(B_OUT);
					}
					else
					{
						SOUNDMANAGER->play("shoot");
						boss.frameIdx = 0;
						if (boss.distance < 300)
						{
							setBossState(B_OUT);
						}
					}
					break;
				case B_OUT:
					this->randomLocation();
					break;
				case B_IN:
					this->randomPatten(boss.x);
					break;
				case B_DIEGROUND:
					boss.x > playerX ? boss.isRight = false : boss.isRight = true;
					setBossState(B_DEAD);
					break;
				case B_NOHEAD:
					boss.frameIdx = _End;
					animationOff = true;
					break;
				default:boss.frameIdx = 0;
					break;
				}

			}
		}
	}
	else
	{
		boss.frameY = 1;
		if (boss.state == B_HURTRECOVER && boss.isCrashedBottom == false && DATA->getCurrentClear() != STAGE3_CLEARED)
		{
			boss.frameIdx = 12;
			boss.isRight ? boss.moveAngle = PI - 1 : boss.moveAngle = 1;
			if (SLOW_inBoss == false)boss.gravity += 0.4f;
			if (boss.isCrashedLeft == false && boss.isCrashedRight == false)boss.x += cosf(boss.moveAngle) * (boss.jumpSpeed / SLOW_ratio_inBoss);
			boss.y += -sinf(boss.moveAngle) * (boss.fallSpeed / SLOW_ratio_inBoss) + (boss.gravity / SLOW_ratio_inBoss);
		}
		else if (boss.state == B_HURTRECOVER && boss.isCrashedBottom == true && boss.frameIdx < 2 && DATA->getCurrentClear() != STAGE3_CLEARED)
		{
			boss.gravity = 0;
			boss.stateCnt = 0;
			setBossState(B_OUT);
		}
		else if (boss.frameCnt % boss.frameTum == 0)
		{
			boss.frameIdx--;
			if (boss.frameIdx == 5)
			{
				if (boss.state == B_SHOOT)
				{
					int random = RANDOM->Range(0, 5);
					float add;
					switch (random)
					{
					case 0:
						add = 0.0f;
						break;
					case 1:
						add = -0.05f;
						break;
					case 2:
						add = -0.1f;
						break;
					case 3:
						add = -0.15f;
						break;
					case 4:
						add = -0.2f;
						break;
					case 5:
						add = -0.25f;
						break;
					}
					boss.isRight ? boss.shootAngle = 0 : boss.shootAngle = PI;
					minePt->fire(boss.x + 50, boss.y, boss.shootAngle + add);
					boss.stateCnt++;
				}
			}
			if (boss.frameIdx < 0)
			{
				switch (boss.state)
				{
				case B_HURTRECOVER:boss.isGenrateCoolTime = true;
					boss.gravity = 0;
					break;
				case B_DRAWRIFLE:
					SOUNDMANAGER->play("aim_h", 0.5f);
					boss.state = B_AIM;
					break;
				case B_WALLGRAB:
					boss.isRight ? boss.isRight = false : boss.isRight = true;
					boss.isWallJump = true;
					boss.state = B_JUMP;
					break;
				case B_PREJUMP:
					boss.isRight ? boss.isRight = false : boss.isRight = true;
					boss.state = B_JUMP;
					break;
				case B_DASH_PRE:
					boss.state = B_DASH;
					break;
				case B_WALLJUMP:boss.frameIdx = 0;
					boss.isFire = false;
					break;
				case B_LAND:
					SOUNDMANAGER->play("p_land");
					boss.stateCnt = 0;
					setBossState(B_OUT);
					break;
				case B_DASH_END:
					boss.isRight ? boss.isRight = false : boss.isRight = true;
					boss.stateCnt = 0;
					setBossState(B_OUT);
					break;
				case B_TELEPORT:
					if (boss.stateCnt < 3)
					{
						boss.isLazerGenerated = false;
						boss.stateCnt++;
						boss.frameIdx = 0;
					}
					else if (boss.stateCnt < 6)
					{
						boss.isLazerGenerated = false;
						boss.stateCnt++;
						boss.x = DATA->getPlayerData().x;
						boss.frameIdx = 0;
					}
					else
					{
						boss.isLazerGenerated = false;
						boss.stateCnt = 0;
						int rand = RANDOM->Range(1, 2);
						if (rand == 1)
						{
							setBossState(B_TELEPORT_GROUND);
						}
						else
						{
							boss.x = 1080;
							boss.y = 530;
							boss.isRight = false;
							prePattern = B_DASH_PRE;
							setBossState(B_DASH_PRE);
						}
					}
					break;
				case B_SWEEP:
					if (boss.stateCnt == 0)
					{
						boss.isLazerGenerated = false;
						boss.stateCnt++;
						boss.frameIdx = 0;
						boss.isRight = true;
					}
					else
					{
						boss.isLazerGenerated = false;
						boss.stateCnt = 0;
						setBossState(B_TELEPORT_GROUND);
					}
					break;
				case B_TELEPORT_GROUND:
					boss.stateCnt = 0;
					boss.isLazerGenerated = false;
					setBossState(B_OUT);
					break;
				case B_DRAWGUN:
					setBossState(B_SHOOT);
					break;
				case B_SHOOT:
					if (boss.stateCnt == 3)
					{
						boss.stateCnt = 0;
						setBossState(B_OUT);
					}
					else
					{
						SOUNDMANAGER->play("shoot");
						boss.frameIdx = _End;
						if (boss.distance < 300)
						{
							setBossState(B_OUT);
						}
					}
					break;
				case B_OUT:
					this->randomLocation();
					break;
				case B_IN:
					this->randomPatten(boss.x);
					break;
				case B_DIEGROUND:
					boss.x > playerX ? boss.isRight = false : boss.isRight = true;
					setBossState(B_DEAD);
					break;
				case B_NOHEAD:
					boss.frameIdx = 0;
					animationOff = true;
					break;
				default:boss.frameIdx = _End;
					break;
				}
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
		if (boss.lazerY == 2)SOUNDMANAGER->play("lazer", 0.5f);
		boss.lazerY < 2 ? boss.lazerHitBox = true : boss.lazerHitBox = false;
		if (boss.lazerY > 4)
		{
			boss.lazerY = 0;
			//boss.state = B_IDLE;
			boss.aimCnt = 0;
			boss.lazerReady = false;
		}
	}
}

void bossClass::bossPixelCollision()
{
	//픽셀충돌 왼쪽
	for (int i = boss.y - boss.height / 2; i < boss.y; i++)
	{
		COLORREF color = GetPixel(DATA->getBgCol().img->getMemDC(), boss.findL, i);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (r == 255 && g == 0 && b == 0)
		{
			boss.isCrashedLeft = true;
		}
		else
		{
			boss.isCrashedLeft = false;
		}
	}

	//픽셀충돌 오른쪽
	for (int i = boss.y - boss.height / 2; i < boss.y; i++)
	{
		COLORREF color = GetPixel(DATA->getBgCol().img->getMemDC(), boss.findR, i);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (r == 255 && g == 0 && b == 0)
		{
			boss.isCrashedRight = true;
		}
		else
		{
			boss.isCrashedRight = false;
		}
	}

	//픽셀충돌 아래
	for (int i = boss.x - boss.width / 2; i < boss.x + boss.width / 2; i++)
	{
		COLORREF color = GetPixel(DATA->getBgCol().img->getMemDC(), i, boss.findB);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			if (r == 255 && g == 0 && b == 0)continue;
			boss.isCrashedBottom = true;
			break;
		}
		else
		{
			boss.isCrashedBottom = false;
		}
	}
}

void bossClass::setBossState(enum_BossState _state)
{
	boss.state = _state;
	switch (boss.state)
	{
	case B_DRAWRIFLE:
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 8;
		break;
	case B_PREJUMP:
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 3;
		break;
	case B_JUMP:
		SOUNDMANAGER->play("p_jump");
		boss.isRight ? boss.frameY = 0 : boss.frameY = 1;
		break;
	case B_WALLGRAB:
		SOUNDMANAGER->play("p_wallgrab");
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 3;
		break;
	case B_WALLJUMP:
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 7;
		break;
	case B_LAND:
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 4;
		break;
	case B_IDLE:
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 12;
		break;
	case B_DASH_PRE:
		SOUNDMANAGER->play("predash");
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 8;
		break;
	case B_DASH_END:
		SOUNDMANAGER->play("dash");
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 10;
		break;
	case B_TELEPORT:
		boss.isLazerGenerated = false;
		boss.isRight = true;
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 8;
		break;
	case B_SWEEP:
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 26;
		break;
	case B_TELEPORT_GROUND:
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 8;
		break;
	case B_DRAWGUN:
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 7;
		break;
	case B_SHOOT:
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 8;
		break;
	case B_IN:
		SOUNDMANAGER->play("teleport");
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 4;
		break;
	case B_OUT:
		boss.stateCnt = 0;
		SOUNDMANAGER->play("teleport");
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 4;
		break;
	case B_DEAD:
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 19;
		break;
	case B_DIEFLY:
		SOUNDMANAGER->stop("stage5");
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 4;
		break;
	case B_DIEGROUND:
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 8;
		break;
	case B_NOHEAD:
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 6;
		break;
	}
}

void bossClass::randomLocation()
{
	int _num = RANDOM->Range(1, 3);

	//이전 패턴과 다른 패턴이 나올때까지 while문을 돌린다.
	while (1)
	{
		if (_num == preLocation)
		{
			_num = RANDOM->Range(1, 3);
		}
		else break;
	}
	
	switch (_num)
	{
	case 1:
		boss.isRight = true;
		boss.x = 270;
		boss.y = 530;
		setBossState(B_IN);
		break;
	case 2:
		boss.isRight = false;
		boss.x = 1080;
		boss.y = 530;
		setBossState(B_IN);
		break;
	case 3:
		boss.isRight = false;
		boss.x = 970;
		boss.y = 530;
		setBossState(B_IN);
		break;
	}
	
	preLocation = _num;
	
}

void bossClass::randomPatten(int _bossX)
{
	int randNum;
	switch (_bossX)
	{
	case 270:
		randNum = RANDOM->Range(1, 3);
		if (randNum == 1)
		{
			prePattern != B_DASH_PRE ? setBossState(B_DASH_PRE) : this->randomLocation();
			prePattern = B_DASH_PRE;
		}
		else if (randNum == 2)
		{
			prePattern != B_DRAWGUN ? setBossState(B_DRAWGUN) : this->randomLocation();
			prePattern = B_DRAWGUN;
		}
		else
		{
			prePattern != B_PREJUMP ? setBossState(B_PREJUMP) : this->randomLocation();
			prePattern = B_PREJUMP;
		}
		break;
	case 1080:
		randNum = RANDOM->Range(1, 3);
		if (randNum == 1)
		{
			prePattern != B_DASH_PRE ? setBossState(B_DASH_PRE) : this->randomLocation();
			prePattern = B_DASH_PRE;
		}
		else if (randNum == 2)
		{
			prePattern != B_DRAWGUN ? setBossState(B_DRAWGUN) : this->randomLocation();
			prePattern = B_DRAWGUN;
		}
		else
		{
			prePattern != B_TELEPORT ? setBossState(B_TELEPORT) : this->randomLocation();
			prePattern = B_TELEPORT;
		}
		break;
	case 970:
		randNum = RANDOM->Range(1, 3);
		if (randNum == 1)
		{
			prePattern != B_TELEPORT ? setBossState(B_TELEPORT) : this->randomLocation();
			prePattern = B_TELEPORT;
		}
		else if (randNum == 2)
		{
			prePattern != B_PREJUMP ? setBossState(B_PREJUMP) : this->randomLocation();
			prePattern = B_PREJUMP;
		}
		else this->randomLocation();
		break;
	}
	
}




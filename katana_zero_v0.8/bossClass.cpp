#include "stdafx.h"
#include "bossClass.h"

HRESULT bossClass::init()
{
	boss.isRight = false;
	boss.lazerReady = false;
	boss.lazerHitBox = false;
	boss.isGenrateCoolTime = false;
	boss.isLazerGenerated = false;

	boss.isWallJump = false;
	boss.isFire = false;
	boss.aimAngle = PI;
	boss.frameCnt = 0;
	boss.currentPattern = 0;
	boss.frameIdx = 0;
	boss.speed = 5.0f;
	boss.jumpSpeed = 10.0f;
	boss.wallJumpSpeed = 18.0f;
	boss.dashSpeed = 100.0f;
	boss.frameY = 0;
	boss.stateCnt = boss.lazerY = boss.aimCnt = 0;
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

	boss.gravity = 0;
	boss.moveAngle = PI / 2;
	boss.shootAngle = 0;

	boss.preJumpImg = IMAGEMANAGER->findImage("boss_prejump");
	boss.jumpImg = IMAGEMANAGER->findImage("boss_jump");
	boss.wallGrabImg = IMAGEMANAGER->findImage("boss_wallgrab");
	boss.wallJumpImg = IMAGEMANAGER->findImage("boss_walljump");

	bulletPt = new missile;
	bulletPt->init(15,1200);

	return S_OK;
}

void bossClass::release()
{
	bulletPt->release();
	SAFE_DELETE(bulletPt);
}

void bossClass::update()
{
	bulletPt->update();

	SLOW_inBoss = DATA->getSlow();
	SLOW_ratio_inBoss = DATA->getSlowRatio();

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
							vLazer[i].isRight ? vLazer[i].angle -= 0.1f : vLazer[i].angle += 0.19f;
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
						if (vLazer[i].frameIdx > 9)
						{
							vLazer[i].frameIdx = 10;
						}
					}
				}
			}
		}

		if (boss.state == B_AIM)
		{
			boss.aimCnt++;
			if (boss.aimCnt > 40)
			{
				boss.lazerReady = true;
			}
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
			if (boss.stateCnt == 25)
			{
				setBossState(B_WALLJUMP);
				boss.isFire = true;
			}
			boss.isRight ? boss.moveAngle = 1 : boss.moveAngle = PI - 1;
			if(SLOW_inBoss==false)boss.gravity += 0.22f;
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
				if (boss.isRight)
				{
					bulletPt->fire(boss.x - DATA->getBgCol().x, boss.y + 100 - DATA->getBgCol().y, boss.shootAngle);
					boss.shootAngle += 0.2f;
				}
				else
				{
					bulletPt->fire(boss.x - DATA->getBgCol().x, boss.y + 100 - DATA->getBgCol().y, boss.shootAngle);
					boss.shootAngle -= 0.2f;
				}
			}
		}


		//대쉬
		if (boss.state == B_DASH)
		{
			boss.stateCnt++;
			boss.isRight ? boss.moveAngle = 1 : boss.moveAngle = PI - 1;
			boss.x += cosf(boss.moveAngle) * (boss.dashSpeed / SLOW_ratio_inBoss);
			if (boss.stateCnt > 15 * SLOW_ratio_inBoss)
			{
				boss.stateCnt = 0;
				setBossState(B_DASH_END);
			}
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
				if (boss.isLazerGenerated == false)
				{
					this->generateLazer(boss.x, boss.y, 0);
					boss.isLazerGenerated = true;
				}
				break;
			case 2:
				boss.x = 348;
				if (boss.isLazerGenerated == false)
				{
					this->generateLazer(boss.x, boss.y, 0);
					boss.isLazerGenerated = true;
				}
				break;
			case 3:
				boss.x = 974;
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
				
				break;
			}
		}

		//지상 텔포
		if (boss.state == B_TELEPORT_GROUND)
		{
			boss.x = 210;
			boss.y = 530;
			if (boss.isLazerGenerated == false)
			{
				this->generateLazer(boss.x, boss.y,true, 0);
				boss.isLazerGenerated = true;
			}
		}

		boss.frameCnt++;
		boss.rc = RectMakeCenter(boss.x - DATA->getBgCol().x, boss.y - DATA->getBgCol().y, boss.width, boss.height);

		boss.findL = (boss.x - boss.width / 2) - boss.speed * 2;
		boss.findR = (boss.x + boss.width / 2) + boss.speed * 2;
		boss.findY = boss.y + boss.height / 2;
		boss.findB = (boss.y + boss.height / 2) + 50;
		boss.centerX = boss.rc.left + boss.width / 2;
		boss.centerY = boss.rc.top + boss.height / 2;
		boss.centerX = boss.rc.left + boss.width / 2;
		boss.centerY = boss.rc.top + boss.height / 2;

		this->bossAnimation();
		if (boss.state == B_IDLE || boss.state == B_AIM || boss.state == B_LAND)
		{
			this->bossOnFloor();
		}
		this->bossPixelCollision();
	}
	
}

void bossClass::render()
{
	bulletPt->render();
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
	
				}
				else
				{
					//if (boss.frameIdx < 18 && boss.frameIdx > 3)
					//{
					//	vLazer[i].img->rotateFrameRender(getMemDC(), vLazer[i].x - DATA->getBgCol().x - 10, vLazer[i].y - DATA->getBgCol().y, 0, 4, vLazer[i].angle);
					//}
				}
			}
			else if (vLazer[i].isGround && vLazer[i].isRotate == false)
			{
				vLazer[i].img->frameRender(getMemDC(), vLazer[i].x - DATA->getBgCol().x + 26, vLazer[i].y - DATA->getBgCol().y - 13, 0, vLazer[i].frameIdx);
			}
			else
			{
				vLazer[i].img->frameRender(getMemDC(), vLazer[i].x - DATA->getBgCol().x - 32, vLazer[i].y - DATA->getBgCol().y + 45, vLazer[i].frameIdx, 0);
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
			boss.drawRifleImg->frameRender(getMemDC(), boss.rc.left - 30, boss.rc.top + 3, boss.frameIdx, boss.frameY);
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
		case B_PREJUMP:
			boss.preJumpImg->frameRender(getMemDC(), boss.rc.left -22, boss.rc.top, boss.frameIdx, boss.frameY);
			break;
		case B_JUMP:
			boss.jumpImg->frameRender(getMemDC(), boss.rc.left - 10, boss.rc.top, 0, boss.frameY);
			break;
		case B_WALLGRAB:
			boss.wallGrabImg->frameRender(getMemDC(), boss.rc.left - 16, boss.rc.top +2, boss.frameIdx, boss.frameY);
			break;
		case B_WALLJUMP:
			if (boss.isRight)
			{
				if (boss.frameIdx != 7)
				{
					IMAGEMANAGER->findImage("boss_walljump_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 94, boss.rc.top + 6, boss.frameIdx - 3, boss.frameY, 40);
					IMAGEMANAGER->findImage("boss_walljump_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 84, boss.rc.top + 16, boss.frameIdx - 2, boss.frameY, 80);
					IMAGEMANAGER->findImage("boss_walljump_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 74, boss.rc.top + 26, boss.frameIdx - 1, boss.frameY, 120);
				}
			}
			else
			{
				if (boss.frameIdx != 0)
				{
					IMAGEMANAGER->findImage("boss_walljump_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 34, boss.rc.top + 6, boss.frameIdx - 3, boss.frameY, 40);
					IMAGEMANAGER->findImage("boss_walljump_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 44, boss.rc.top + 16, boss.frameIdx - 2, boss.frameY, 80);
					IMAGEMANAGER->findImage("boss_walljump_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 54, boss.rc.top + 26, boss.frameIdx - 1, boss.frameY, 120);
				}
			}
			boss.wallJumpImg->frameRender(getMemDC(), boss.rc.left - 64, boss.rc.top +36, boss.frameIdx, boss.frameY);
			break;
		case B_LAND:
			boss.landImg->frameRender(getMemDC(), boss.rc.left -22, boss.rc.top + 2, boss.frameIdx, boss.frameY);
			break;
		case B_DASH_PRE:
			boss.preDashImg->frameRender(getMemDC(), boss.rc.left - 70, boss.rc.top + 2, boss.frameIdx, boss.frameY);
			break;
		case B_DASH:
			boss.isRight ? boss.frameY = 0 : boss.frameY = 1;
			if (boss.isRight)
			{
				IMAGEMANAGER->findImage("boss_dash_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 228, boss.rc.top + 38, 0, boss.frameY, 60);
				IMAGEMANAGER->findImage("boss_dash_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 188, boss.rc.top + 38, 0, boss.frameY, 80);
				IMAGEMANAGER->findImage("boss_dash_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 148, boss.rc.top + 38, 0, boss.frameY, 100);
				IMAGEMANAGER->findImage("boss_dash_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 108, boss.rc.top + 38, 0, boss.frameY, 120);
				IMAGEMANAGER->findImage("boss_dash_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 68, boss.rc.top + 38, 0, boss.frameY, 140);
			}
			else
			{
				IMAGEMANAGER->findImage("boss_dash_shadow")->alphaFrameRender(getMemDC(), boss.rc.left + 118, boss.rc.top + 38, 0, boss.frameY, 60);
				IMAGEMANAGER->findImage("boss_dash_shadow")->alphaFrameRender(getMemDC(), boss.rc.left + 78, boss.rc.top + 38, 0, boss.frameY, 80);
				IMAGEMANAGER->findImage("boss_dash_shadow")->alphaFrameRender(getMemDC(), boss.rc.left + 38, boss.rc.top + 38, 0, boss.frameY, 100);
				IMAGEMANAGER->findImage("boss_dash_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 8, boss.rc.top + 38, 0, boss.frameY, 120);
				IMAGEMANAGER->findImage("boss_dash_shadow")->alphaFrameRender(getMemDC(), boss.rc.left - 48, boss.rc.top + 38, 0, boss.frameY, 140);
			}
			boss.dashImg->frameRender(getMemDC(), boss.rc.left - 58, boss.rc.top + 38, 0, boss.frameY);
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
		}
	}

	
	
}

void bossClass::generateLazer(float _x, float _y, int _frameidx)
{
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
	_set ? sLazer.angle = 1 : sLazer.angle = PI + 0.5;
	sLazer.img = IMAGEMANAGER->findImage("boss_lazer_ground_small");
	sLazer.frameIdx = _frameidx;
	sLazer.frameCnt = 0;
	sLazer.x = _x;
	sLazer.y = _y;

	vLazer.push_back(sLazer);
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
		this->animationSetFrame(7, 13);
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
		this->animationSetFrame(5, 7);
		break;
	case B_SHOOT:
		this->animationSetFrame(5, 8);
		break;
	case B_TELEPORT:
		this->animationSetFrame(3, 8);
		break;
	case B_TELEPORT_GROUND:
		this->animationSetFrame(3, 8);
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
				switch (boss.state)
				{
				case B_HURTRECOVER:boss.isGenrateCoolTime = true;
					break;
				case B_DRAWRIFLE:boss.state = B_AIM;
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
				case B_LAND:setBossState(B_IDLE);
					break;
				case B_DASH_END:
					boss.isRight ? boss.isRight = false : boss.isRight = true;
					setBossState(B_IDLE);
					break;
				case B_TELEPORT:
					if (boss.stateCnt < 3)
					{
						boss.isLazerGenerated = false;
						boss.stateCnt++;
						boss.frameIdx = 0;
					}
					else
					{
						boss.isLazerGenerated = false;
						boss.stateCnt = 0;
						boss.isRight ? boss.isRight = false : boss.isRight = true;
						setBossState(B_SWEEP);
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
		if (boss.frameCnt % boss.frameTum == 0)
		{
			boss.frameIdx--;
			if (boss.frameIdx < 0)
			{
				switch (boss.state)
				{
				case B_HURTRECOVER:boss.isGenrateCoolTime = true;
					break;
				case B_DRAWRIFLE:boss.state = B_AIM;
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
				case B_LAND:setBossState(B_IDLE);
					break;
				case B_DASH_END:
					boss.isRight ? boss.isRight = false : boss.isRight = true;
					setBossState(B_IDLE);
					break;
				case B_TELEPORT:
					boss.stateCnt++;
					break;
					if (boss.stateCnt < 3)
					{
						boss.isLazerGenerated = false;
						boss.stateCnt++;
						boss.frameIdx = _End;
					}
					else
					{
						boss.isLazerGenerated = false;
						boss.stateCnt = 0;
						boss.isRight ? boss.isRight = false : boss.isRight = true;
						setBossState(B_SWEEP);
					}
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
	case B_PREJUMP:
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 3;
		break;
	case B_JUMP:
		boss.isRight ? boss.frameY = 0 : boss.frameY = 1;
		break;
	case B_WALLGRAB:
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
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 8;
		break;
	case B_DASH_END:
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 10;
		break;
	case B_TELEPORT:
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 8;
		break;
	case B_SWEEP:
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 26;
		break;
	case B_TELEPORT_GROUND:
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 8;
		break;
	}
}




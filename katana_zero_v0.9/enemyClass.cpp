#include "stdafx.h"
#include "enemyClass.h"

HRESULT enemyClass::init()
{
	firstSetup = false;
	BULLETSIZE = 1;


	return S_OK;
}

void enemyClass::release()
{
}

void enemyClass::update()
{
	SLOW_inEnemy = DATA->getSlow();
	SLOW_Ratio_inEnemy = DATA->getSlowRatio();

	if (DATA->getStop() == true)
	{

	}
	else if (vEnemy.empty()==false)
	{
		for (int i = 0; i < vEnemy.size(); i++)
		{
			//파인드 렉트 크기
		
			//건스파크 좌표 수정
			if (vEnemy[i].isRight)
			{
				if ((int)vEnemy[i].distanceToPlayer_Y < -20 && (int)vEnemy[i].distanceToPlayer_Y > -50)
				{
					vEnemy[i].gunsparkX = vEnemy[i].rc.left + 65;
					vEnemy[i].gunsparkY = vEnemy[i].rc.top + 10;
				}
				else if ((int)vEnemy[i].distanceToPlayer_Y < -50 && (int)vEnemy[i].distanceToPlayer_Y > -100)
				{
					vEnemy[i].gunsparkX = vEnemy[i].rc.left + 65;
					vEnemy[i].gunsparkY = vEnemy[i].rc.top - 5;
				}
				else if ((int)vEnemy[i].distanceToPlayer_Y < -100 && (int)vEnemy[i].distanceToPlayer_Y > -200)
				{
					vEnemy[i].gunsparkX = vEnemy[i].rc.left + 55;
					vEnemy[i].gunsparkY = vEnemy[i].rc.top - 10;
				}
				else
				{
					vEnemy[i].gunsparkX = vEnemy[i].rc.left + 65;
					vEnemy[i].gunsparkY = vEnemy[i].rc.top + 18;
				}
			}
			else
			{
				if ((int)vEnemy[i].distanceToPlayer_Y < -20 && (int)vEnemy[i].distanceToPlayer_Y > -50)
				{
					vEnemy[i].gunsparkX = vEnemy[i].rc.left - 80;
					vEnemy[i].gunsparkY = vEnemy[i].rc.top + 10;
				}
				else if ((int)vEnemy[i].distanceToPlayer_Y < -50 && (int)vEnemy[i].distanceToPlayer_Y > -100)
				{
					vEnemy[i].gunsparkX = vEnemy[i].rc.left - 80;
					vEnemy[i].gunsparkY = vEnemy[i].rc.top - 5;
				}
				else if ((int)vEnemy[i].distanceToPlayer_Y < -100 && (int)vEnemy[i].distanceToPlayer_Y > -200)
				{
					vEnemy[i].gunsparkX = vEnemy[i].rc.left - 70;
					vEnemy[i].gunsparkY = vEnemy[i].rc.top - 10;
				}
				else if ((int)vEnemy[i].distanceToPlayer_Y > 20 && (int)vEnemy[i].distanceToPlayer_Y < 100)
				{
					vEnemy[i].gunsparkX = vEnemy[i].rc.left - 80;
					vEnemy[i].gunsparkY = vEnemy[i].rc.top + 18;
				}
				else
				{
					vEnemy[i].gunsparkX = vEnemy[i].rc.left - 80;
					vEnemy[i].gunsparkY = vEnemy[i].rc.top + 18;
				}
			}
			


			vEnemy[i].frameCnt++;
			//슬로우
			if (SLOW_inEnemy == true)
			{
				vEnemy[i].speed = vEnemy[i].speed_old / SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].speed = vEnemy[i].speed_old;
			}

			//공격 렉트
			if (vEnemy[i].isAttack == true && vEnemy[i].state == E_ATTACK && vEnemy[i].isAttackStart ==true)
			{
				if (vEnemy[i].isRight)
				{
					vEnemy[i].att.rc = RectMakeCenter((vEnemy[i].x + vEnemy[i].att.width / 2) - DATA->getBgCol().x, vEnemy[i].y - DATA->getBgCol().y, vEnemy[i].att.width, vEnemy[i].att.height);
				}
				else
				{
					vEnemy[i].att.rc = RectMakeCenter((vEnemy[i].x - vEnemy[i].att.width / 2) - DATA->getBgCol().x, vEnemy[i].y - DATA->getBgCol().y, vEnemy[i].att.width, vEnemy[i].att.height);
				}
			}
			else
			{
				vEnemy[i].att.rc = RectMakeCenter(-100, -100, 0, 0);
			}

			//반사된 총알과의 충돌
			for (int k = 0; k < vEnemy.size(); k++)
			{
				for (int j = 0; j < BULLETSIZE; j++)
				{
					RECT tempRect6;
					if (IntersectRect(&tempRect6, &vEnemy[i].rc, &vEnemy[k].bullet[j].rc) && vEnemy[k].bullet[j].isReflected == true && vEnemy[i].state != E_HURT)
					{
						vEnemy[i].isReflectDead = true;
						this->setEnemyHurt(i);
						vEnemy[k].bullet[j].fire = false;
						
					}
				}
			}

			//에너미 초기 위치 잡기
			if (vEnemy[i].isCrashedBottom == false)
			{
				vEnemy[i].y += vEnemy[i].speed;
			}

			//에너미 플레이어 인식
			if (vEnemy[i].founded == true && vEnemy[i].state != E_HURT && DATA->getDead() == false && vEnemy[i].state != E_GEN)
			{
				this->enemyMoveToPlayer(i);
				this->enemyOnFloor(i);
			}
			else
			{
				this->enemyMove(i);
			}

			//에너미 총알발사
			if (vEnemy[i].state == E_AIM)
			{
				vEnemy[i].fireCnt++;
				this->enemyShoot(i);
			}
			else
			{
				vEnemy[i].fireCnt = 50;
			}

			vEnemy[i].rc = RectMakeCenter(vEnemy[i].x - DATA->getBgCol().x, vEnemy[i].y - DATA->getBgCol().y, vEnemy[i].width, vEnemy[i].height);
			
			if (vEnemy[i].isRight)
			{
				vEnemy[i].detect.rc = RectMakeCenter((vEnemy[i].x + vEnemy[i].detect.width / 2) - DATA->getBgCol().x, vEnemy[i].y - DATA->getBgCol().y - 50, vEnemy[i].detect.width, vEnemy[i].detect.height);
			}
			else
			{
				vEnemy[i].detect.rc = RectMakeCenter((vEnemy[i].x - vEnemy[i].detect.width / 2) - DATA->getBgCol().x, vEnemy[i].y - DATA->getBgCol().y - 50, vEnemy[i].detect.width, vEnemy[i].detect.height);
			}
		
			//픽셀충돌 탐색점
			vEnemy[i].findL = (vEnemy[i].x - vEnemy[i].width / 2) - vEnemy[i].speed * 2;
			vEnemy[i].findR = (vEnemy[i].x + vEnemy[i].width / 2) + vEnemy[i].speed * 2;
			vEnemy[i].findY = vEnemy[i].y + vEnemy[i].height / 2;
			vEnemy[i].findB = vEnemy[i].y + vEnemy[i].height / 2;
			vEnemy[i].centerX = vEnemy[i].rc.left + vEnemy[i].width / 2;
			vEnemy[i].centerY = vEnemy[i].rc.top + vEnemy[i].height / 2;

			enemyPixelCollisionleft(i);
			enemyPixelCollisionRight(i);
			enemyPixelCollisionBottom(i);
			enemySetBullet(i);
			enemyMoveBullet(i);
			bloodAnimation(i);
			enemyAnimation(i);
			enemyDoorDead(i);
		}
	}

	
}

void enemyClass::render()
{
	for (int i = 0; i < vEnemy.size(); i++)
	{
		//Rectangle(getMemDC(), vEnemy[i].rc);
		//Rectangle(getMemDC(), vEnemy[i].detect.rc);
		//Rectangle(getMemDC(), vEnemy[i].att.rc);
		
		//총알렌더
		for (int j = 0; j < BULLETSIZE; j++)
		{
			if (vEnemy[i].bullet[j].fire == true)
			{
				//Rectangle(getMemDC(), vEnemy[i].bullet[j].rc);
				vEnemy[i].bullet[j].img->rotateRender(getMemDC(), vEnemy[i].bullet[j].rc.left, vEnemy[i].bullet[j].rc.top, vEnemy[i].bullet[j].angle);
			}
		}

		if (vEnemy[i].isRight)
		{
			vEnemy[i].frameY = 0;
		}
		else
		{
			vEnemy[i].frameY = 1;
		}
		switch (vEnemy[i].state)
		{
		case E_IDLE:
			switch (vEnemy[i].category)
			{
			case E_BOLD:
				vEnemy[i].idleImg->frameRender(getMemDC(), vEnemy[i].rc.left - 30, vEnemy[i].rc.top - 3, vEnemy[i].frameIdx, vEnemy[i].frameY);
				break;
			case E_POMP:
				vEnemy[i].idleImg->frameRender(getMemDC(), vEnemy[i].rc.left - 10, vEnemy[i].rc.top - 15, vEnemy[i].frameIdx, vEnemy[i].frameY);
				break;
			case E_COP:
				vEnemy[i].idleImg->frameRender(getMemDC(), vEnemy[i].rc.left - 20, vEnemy[i].rc.top - 8, vEnemy[i].frameIdx, vEnemy[i].frameY);
				break;
			case E_GRUNT:
				vEnemy[i].idleImg->frameRender(getMemDC(), vEnemy[i].rc.left - 20, vEnemy[i].rc.top - 5, vEnemy[i].frameIdx, vEnemy[i].frameY);
				break;
			case E_TURRET:
				vEnemy[i].idleImg->frameRender(getMemDC(), vEnemy[i].rc.left, vEnemy[i].rc.top, vEnemy[i].frameIdx, vEnemy[i].frameY);
				break;
			}
			break;
		case E_GEN:
			switch (vEnemy[i].category)
			{
			case E_BOLD:
				vEnemy[i].idleImg->alphaFrameRender(getMemDC(), vEnemy[i].rc.left - 30, vEnemy[i].rc.top - 3, vEnemy[i].frameIdx, vEnemy[i].frameY,vEnemy[i].alpha);
				break;
			case E_POMP:
				vEnemy[i].idleImg->alphaFrameRender(getMemDC(), vEnemy[i].rc.left - 10, vEnemy[i].rc.top - 15, vEnemy[i].frameIdx, vEnemy[i].frameY, vEnemy[i].alpha);
				break;
			case E_COP:
				vEnemy[i].idleImg->alphaFrameRender(getMemDC(), vEnemy[i].rc.left - 20, vEnemy[i].rc.top - 8, vEnemy[i].frameIdx, vEnemy[i].frameY, vEnemy[i].alpha);
				break;
			case E_GRUNT:
				vEnemy[i].idleImg->alphaFrameRender(getMemDC(), vEnemy[i].rc.left - 20, vEnemy[i].rc.top - 5, vEnemy[i].frameIdx, vEnemy[i].frameY, vEnemy[i].alpha);
				break;
			case E_TURRET:
				vEnemy[i].idleImg->frameRender(getMemDC(), vEnemy[i].rc.left, vEnemy[i].rc.top, vEnemy[i].frameIdx, vEnemy[i].frameY);
				break;
			}
			break;
		case E_WALK:
			switch (vEnemy[i].category)
			{
			case E_BOLD:
				vEnemy[i].walkImg->frameRender(getMemDC(), vEnemy[i].rc.left - 20, vEnemy[i].rc.top - 6, vEnemy[i].frameIdx, vEnemy[i].frameY);
				break;
			case E_POMP:
				vEnemy[i].walkImg->frameRender(getMemDC(), vEnemy[i].rc.left - 10, vEnemy[i].rc.top - 16, vEnemy[i].frameIdx, vEnemy[i].frameY);
				break;
			case E_COP:
				vEnemy[i].walkImg->frameRender(getMemDC(), vEnemy[i].rc.left - 20, vEnemy[i].rc.top - 6, vEnemy[i].frameIdx, vEnemy[i].frameY);
				break;
			case E_GRUNT:
				vEnemy[i].walkImg->frameRender(getMemDC(), vEnemy[i].rc.left - 20, vEnemy[i].rc.top - 15, vEnemy[i].frameIdx, vEnemy[i].frameY);
				break;
			}
			break;
		case E_RUN:
			switch (vEnemy[i].category)
			{
			case E_BOLD:
				vEnemy[i].runImg->frameRender(getMemDC(), vEnemy[i].rc.left - 20, vEnemy[i].rc.top - 8, vEnemy[i].frameIdx, vEnemy[i].frameY);
				break;
			case E_POMP:
				vEnemy[i].runImg->frameRender(getMemDC(), vEnemy[i].rc.left - 10, vEnemy[i].rc.top - 8, vEnemy[i].frameIdx, vEnemy[i].frameY);
				break;
			case E_COP:
				vEnemy[i].runImg->frameRender(getMemDC(), vEnemy[i].rc.left - 20, vEnemy[i].rc.top - 15, vEnemy[i].frameIdx, vEnemy[i].frameY);
				break;
			case E_GRUNT:
				vEnemy[i].runImg->frameRender(getMemDC(), vEnemy[i].rc.left - 20, vEnemy[i].rc.top - 8, vEnemy[i].frameIdx, vEnemy[i].frameY);
				break;
			}
			break;
		case E_HURT:
			switch (vEnemy[i].category)
			{
			case E_TURRET:
				if (vEnemy[i].isRight)
				{
					vEnemy[i].hurtImg->frameRender(getMemDC(), vEnemy[i].rc.left, vEnemy[i].rc.top - 4, vEnemy[i].frameIdx, 0);
				}
				else
				{
					vEnemy[i].hurtImg->frameRender(getMemDC(), vEnemy[i].rc.left, vEnemy[i].rc.top - 4, vEnemy[i].frameIdx, 1);
				}
				//플레이어의 방향에 따라 이펙트 이미지 렌더
				if (vEnemy[i].isPlayerRight)
				{
					if (vEnemy[i].isReflectDead == false && vEnemy[i].isDoorDead == false)vEnemy[i].slashHitImg->frameRender(getMemDC(), vEnemy[i].rc.left + 4, vEnemy[i].rc.top - 26, vEnemy[i].frameIdxHitSlash, 0);
				}
				else
				{
					if (vEnemy[i].isReflectDead == false && vEnemy[i].isDoorDead == false)vEnemy[i].slashHitImg->frameRender(getMemDC(), vEnemy[i].rc.left + 4, vEnemy[i].rc.top - 26, vEnemy[i].frameIdxHitSlash, 1);
				}
				break;
			case E_BOLD:
				//플레이어의 방향에 따라 피 이미지 렌더
				if (vEnemy[i].isPlayerRight)
				{
					if (vEnemy[i].isSplatted)
					{
						vEnemy[i].bloodRemainImgR->alphaRender(getMemDC(), vEnemy[i].rc.left + 40, vEnemy[i].rc.top - 50, vEnemy[i].bloodAlpha);
					}
				}
				else
				{
					if (vEnemy[i].isSplatted)
					{
						vEnemy[i].bloodRemainImgL->alphaRender(getMemDC(), vEnemy[i].rc.left - 40, vEnemy[i].rc.top - 50, vEnemy[i].bloodAlpha);
					}
				}
				//피격 이미지 렌더
				if (vEnemy[i].isRight)
				{
					vEnemy[i].hurtImg->frameRender(getMemDC(), vEnemy[i].rc.left - 10, vEnemy[i].rc.top + 7, vEnemy[i].frameIdx, 0);
				}
				else
				{
					vEnemy[i].hurtImg->frameRender(getMemDC(), vEnemy[i].rc.left - 10, vEnemy[i].rc.top + 7, vEnemy[i].frameIdx, 1);
				}
				//플레이어의 방향에 따라 이펙트 이미지 렌더
				if (vEnemy[i].isPlayerRight)
				{
					vEnemy[i].bloodImg->frameRender(getMemDC(), vEnemy[i].rc.left + 40, vEnemy[i].rc.top - 40, vEnemy[i].frameIdxBlood, 0);
					if (vEnemy[i].isReflectDead == false && vEnemy[i].isDoorDead == false)vEnemy[i].slashHitImg->frameRender(getMemDC(), vEnemy[i].rc.left + 4, vEnemy[i].rc.top - 26, vEnemy[i].frameIdxHitSlash, 0);
				}
				else
				{
					vEnemy[i].bloodImg->frameRender(getMemDC(), vEnemy[i].rc.left - 40, vEnemy[i].rc.top - 40, vEnemy[i].frameIdxBlood, 1);
					if (vEnemy[i].isReflectDead == false && vEnemy[i].isDoorDead == false)vEnemy[i].slashHitImg->frameRender(getMemDC(), vEnemy[i].rc.left + 4, vEnemy[i].rc.top - 26, vEnemy[i].frameIdxHitSlash, 1);
				}
				//피격 이미지 렌더
				break;
			case E_GRUNT:
				//플레이어의 방향에 따라 피 이미지 렌더
				if (vEnemy[i].isPlayerRight)
				{
					if (vEnemy[i].isSplatted)
					{
						vEnemy[i].bloodRemainImgR->alphaRender(getMemDC(), vEnemy[i].rc.left + 40, vEnemy[i].rc.top - 50, vEnemy[i].bloodAlpha);
					}
				}
				else
				{
					if (vEnemy[i].isSplatted)
					{
						vEnemy[i].bloodRemainImgL->alphaRender(getMemDC(), vEnemy[i].rc.left - 40, vEnemy[i].rc.top - 50, vEnemy[i].bloodAlpha);
					}
				}
				//피격 이미지 렌더
				if (vEnemy[i].isRight)
				{
					vEnemy[i].hurtImg->frameRender(getMemDC(), vEnemy[i].rc.left + 5, vEnemy[i].rc.top - 10, vEnemy[i].frameIdx, 0);
				}
				else
				{
					vEnemy[i].hurtImg->frameRender(getMemDC(), vEnemy[i].rc.left - 10, vEnemy[i].rc.top - 10, vEnemy[i].frameIdx, 1);
				}

				//플레이어의 방향에 따라 이펙트 이미지 렌더
				if (vEnemy[i].isPlayerRight)
				{
					vEnemy[i].bloodImg->frameRender(getMemDC(), vEnemy[i].rc.left + 40, vEnemy[i].rc.top - 40, vEnemy[i].frameIdxBlood, 0);
					if (vEnemy[i].isReflectDead == false && vEnemy[i].isDoorDead == false)vEnemy[i].slashHitImg->frameRender(getMemDC(), vEnemy[i].rc.left + 4, vEnemy[i].rc.top - 26, vEnemy[i].frameIdxHitSlash, 0);
				}
				else
				{
					vEnemy[i].bloodImg->frameRender(getMemDC(), vEnemy[i].rc.left - 40, vEnemy[i].rc.top - 40, vEnemy[i].frameIdxBlood, 1);
					if (vEnemy[i].isReflectDead == false && vEnemy[i].isDoorDead == false)vEnemy[i].slashHitImg->frameRender(getMemDC(), vEnemy[i].rc.left + 4, vEnemy[i].rc.top - 26, vEnemy[i].frameIdxHitSlash, 1);
				}
				//피격 이미지 렌더
				break;
			case E_COP:
				//플레이어의 방향에 따라 피 이미지 렌더
				if (vEnemy[i].isPlayerRight)
				{
					if (vEnemy[i].isSplatted)
					{
						vEnemy[i].bloodRemainImgR->alphaRender(getMemDC(), vEnemy[i].rc.left + 40, vEnemy[i].rc.top - 50, vEnemy[i].bloodAlpha);
					}
				}
				else
				{
					if (vEnemy[i].isSplatted)
					{
						vEnemy[i].bloodRemainImgL->alphaRender(getMemDC(), vEnemy[i].rc.left - 40, vEnemy[i].rc.top - 50, vEnemy[i].bloodAlpha);
					}
				}
				//피격 이미지 렌더
				if (vEnemy[i].isRight)
				{
					vEnemy[i].hurtImg->frameRender(getMemDC(), vEnemy[i].rc.left + 5, vEnemy[i].rc.top + 3, vEnemy[i].frameIdx, 0);
				}
				else
				{
					vEnemy[i].hurtImg->frameRender(getMemDC(), vEnemy[i].rc.left - 5, vEnemy[i].rc.top + 3, vEnemy[i].frameIdx, 1);
				}
				//플레이어의 방향에 따라 이펙트 이미지 렌더
				if (vEnemy[i].isPlayerRight)
				{
					vEnemy[i].bloodImg->frameRender(getMemDC(), vEnemy[i].rc.left + 40, vEnemy[i].rc.top - 40, vEnemy[i].frameIdxBlood, 0);
					if (vEnemy[i].isReflectDead == false && vEnemy[i].isDoorDead == false)vEnemy[i].slashHitImg->frameRender(getMemDC(), vEnemy[i].rc.left + 4, vEnemy[i].rc.top - 26, vEnemy[i].frameIdxHitSlash, 0);
				}
				else
				{
					vEnemy[i].bloodImg->frameRender(getMemDC(), vEnemy[i].rc.left - 40, vEnemy[i].rc.top - 40, vEnemy[i].frameIdxBlood, 1);
					if (vEnemy[i].isReflectDead == false && vEnemy[i].isDoorDead == false)vEnemy[i].slashHitImg->frameRender(getMemDC(), vEnemy[i].rc.left + 4, vEnemy[i].rc.top - 26, vEnemy[i].frameIdxHitSlash, 1);
				}
				//피격 이미지 렌더
				break;
			case E_POMP:
				//플레이어의 방향에 따라 피 이미지 렌더
				if (vEnemy[i].isPlayerRight)
				{
					if (vEnemy[i].isSplatted)
					{
						vEnemy[i].bloodRemainImgR->alphaRender(getMemDC(), vEnemy[i].rc.left + 40, vEnemy[i].rc.top - 50, vEnemy[i].bloodAlpha);
					}
				}
				else
				{
					if (vEnemy[i].isSplatted)
					{
						vEnemy[i].bloodRemainImgL->alphaRender(getMemDC(), vEnemy[i].rc.left - 40, vEnemy[i].rc.top - 50, vEnemy[i].bloodAlpha);
					}
				}
				//피격 이미지 렌더
				if (vEnemy[i].isRight)
				{
					vEnemy[i].hurtImg->frameRender(getMemDC(), vEnemy[i].rc.left - 10, vEnemy[i].rc.top - 12, vEnemy[i].frameIdx, 0);
				}
				else
				{
					vEnemy[i].hurtImg->frameRender(getMemDC(), vEnemy[i].rc.left - 10, vEnemy[i].rc.top - 12, vEnemy[i].frameIdx, 1);
				}
				//플레이어의 방향에 따라 이펙트 이미지 렌더
				if (vEnemy[i].isPlayerRight)
				{
					vEnemy[i].bloodImg->frameRender(getMemDC(), vEnemy[i].rc.left + 40, vEnemy[i].rc.top - 40, vEnemy[i].frameIdxBlood, 0);
					if (vEnemy[i].isReflectDead == false && vEnemy[i].isDoorDead == false)vEnemy[i].slashHitImg->frameRender(getMemDC(), vEnemy[i].rc.left + 4, vEnemy[i].rc.top - 26, vEnemy[i].frameIdxHitSlash, 0);
				}
				else
				{
					vEnemy[i].bloodImg->frameRender(getMemDC(), vEnemy[i].rc.left - 40, vEnemy[i].rc.top - 40, vEnemy[i].frameIdxBlood, 1);
					if (vEnemy[i].isReflectDead == false && vEnemy[i].isDoorDead == false)vEnemy[i].slashHitImg->frameRender(getMemDC(), vEnemy[i].rc.left + 4, vEnemy[i].rc.top - 26, vEnemy[i].frameIdxHitSlash, 1);
				}
				//피격 이미지 렌더
				break;

			}
			break;
		case E_ATTACK:
			switch (vEnemy[i].category)
			{
			case E_POMP:
				vEnemy[i].attackImg->frameRender(getMemDC(), vEnemy[i].rc.left - 30, vEnemy[i].rc.top - 15, vEnemy[i].frameIdx, vEnemy[i].frameY);
				break;
			case E_GRUNT:
				vEnemy[i].attackImg->frameRender(getMemDC(), vEnemy[i].rc.left - 20, vEnemy[i].rc.top - 15, vEnemy[i].frameIdx, vEnemy[i].frameY);
				//vEnemy[i].slashImg->frameRender(getMemDC(), vEnemy[i].rc.left - 60, vEnemy[i].rc.top - 15, vEnemy[i].frameIdx, vEnemy[i].frameY);
				break;
			}
		case E_AIM:
			switch (vEnemy[i].category)
			{
			case E_BOLD:
				if (vEnemy[i].frameY == 0)
				{
					vEnemy[i].leftarm_R->rotateRender(getMemDC(), vEnemy[i].rc.left + 5, vEnemy[i].rc.top + 23, vEnemy[i].angleToPlayer - PI);
					vEnemy[i].aimImg_R->render(getMemDC(), vEnemy[i].rc.left - 30, vEnemy[i].rc.top);
					vEnemy[i].gunImg_R->rotateRender(getMemDC(), vEnemy[i].rc.right - 25, vEnemy[i].rc.top + 20, vEnemy[i].angleToPlayer - PI);
					vEnemy[i].rightarm_R->rotateRender(getMemDC(), vEnemy[i].rc.left - 10, vEnemy[i].rc.top + 20, vEnemy[i].angleToPlayer - PI);
					if (vEnemy[i].isFire)vEnemy[i].gunsparkImg->rotateFrameRender(getMemDC(), vEnemy[i].gunsparkX, vEnemy[i].gunsparkY, vEnemy[i].frameIdx, vEnemy[i].frameY, vEnemy[i].angleToPlayer - PI);
				}
				else
				{
					vEnemy[i].leftarm_L->rotateRender(getMemDC(), vEnemy[i].rc.left - 20, vEnemy[i].rc.top + 23, vEnemy[i].angleToPlayer);
					vEnemy[i].aimImg_L->render(getMemDC(), vEnemy[i].rc.left - 30, vEnemy[i].rc.top);
					vEnemy[i].gunImg_L->rotateRender(getMemDC(), vEnemy[i].rc.left - 20, vEnemy[i].rc.top + 20, vEnemy[i].angleToPlayer);
					vEnemy[i].rightarm_L->rotateRender(getMemDC(), vEnemy[i].rc.left - 5, vEnemy[i].rc.top + 20, vEnemy[i].angleToPlayer);
					if (vEnemy[i].isFire)vEnemy[i].gunsparkImg->rotateFrameRender(getMemDC(), vEnemy[i].gunsparkX, vEnemy[i].gunsparkY, vEnemy[i].frameIdx, vEnemy[i].frameY, vEnemy[i].angleToPlayer);
				}
				break;
			case E_COP:
				if (vEnemy[i].frameY == 0)
				{
					vEnemy[i].leftarm_R->rotateRender(getMemDC(), vEnemy[i].rc.left + 5, vEnemy[i].rc.top + 23, vEnemy[i].angleToPlayer - PI);
					vEnemy[i].aimImg_R->render(getMemDC(), vEnemy[i].rc.left - 30, vEnemy[i].rc.top - 3);
					vEnemy[i].gunImg_R->rotateRender(getMemDC(), vEnemy[i].rc.right - 25, vEnemy[i].rc.top + 20, vEnemy[i].angleToPlayer - PI);
					vEnemy[i].rightarm_R->rotateRender(getMemDC(), vEnemy[i].rc.left - 10, vEnemy[i].rc.top + 20, vEnemy[i].angleToPlayer - PI);
					if (vEnemy[i].isFire)vEnemy[i].gunsparkImg->rotateFrameRender(getMemDC(), vEnemy[i].gunsparkX, vEnemy[i].gunsparkY, vEnemy[i].frameIdx, vEnemy[i].frameY, vEnemy[i].angleToPlayer - PI);
				}
				else
				{
					vEnemy[i].leftarm_L->rotateRender(getMemDC(), vEnemy[i].rc.left - 20, vEnemy[i].rc.top + 23, vEnemy[i].angleToPlayer);
					vEnemy[i].aimImg_L->render(getMemDC(), vEnemy[i].rc.left - 30, vEnemy[i].rc.top - 3);
					vEnemy[i].gunImg_L->rotateRender(getMemDC(), vEnemy[i].rc.left - 20, vEnemy[i].rc.top + 20, vEnemy[i].angleToPlayer);
					vEnemy[i].rightarm_L->rotateRender(getMemDC(), vEnemy[i].rc.left - 5, vEnemy[i].rc.top + 20, vEnemy[i].angleToPlayer);
					if (vEnemy[i].isFire)vEnemy[i].gunsparkImg->rotateFrameRender(getMemDC(), vEnemy[i].gunsparkX, vEnemy[i].gunsparkY, vEnemy[i].frameIdx, vEnemy[i].frameY, vEnemy[i].angleToPlayer);
				}
				break;
			case E_TURRET:
				vEnemy[i].idleImg->frameRender(getMemDC(), vEnemy[i].rc.left, vEnemy[i].rc.top, vEnemy[i].frameIdx, vEnemy[i].frameY);
				if (vEnemy[i].isFire)vEnemy[i].gunsparkImg->rotateFrameRender(getMemDC(), vEnemy[i].rc.left - 30, vEnemy[i].rc.top + 13, vEnemy[i].frameIdx, vEnemy[i].frameY, vEnemy[i].angleToPlayer);
				break;
			}
		}
	}

	//char str3[256];
	////글자배경모드
	//SetBkMode(getMemDC(), TRANSPARENT);
	////글자색상
	//SetTextColor(getMemDC(), RGB(255, 0, 255));
	//
	//sprintf(str3, "distanceY : %f", vEnemy[0].distanceToPlayer_Y);
	//TextOut(getMemDC(), 10, 100, str3, strlen(str3));

}

void enemyClass::enemyDoorDead(int i)
{
	if (vEnemy[i].category == E_TURRET)return;
	if (vEnemy[i].isDoorDeadMove)
	{
		vEnemy[i].x += (vEnemy[i].speed * 11);
		vEnemy[i].isDoorDeadMove = false;
	}
}

void enemyClass::generateEnemy(float _x, float _y, enum_EnemyCategory _category, enum_EnemyState _state , bool _set)
{
	sEnemy.x = _x;
	sEnemy.y = _y;
	sEnemy.isRight = _set;
	sEnemy.alpha = 0;
	sEnemy.isCrashedLeft = false;
	sEnemy.isCrashedRight = false;
	sEnemy.isCrashedTop = false;
	sEnemy.isCrashedBottom = false;
	sEnemy.isAttack = false;
	sEnemy.isDoorReached = false;
	sEnemy.isAttackReady = false;
	sEnemy.isAttackStart = false;
	sEnemy.isReflectDead = false;
	sEnemy.isFire = false;
	sEnemy.category = _category;
	sEnemy.state = _state;
	sEnemy.frameIdx = RANDOM->Range(0, 10);
	sEnemy.frameCnt = 0;
	sEnemy.frameCntBlood = 0;
	sEnemy.frameIdxBlood = 0;
	sEnemy.bloodAlpha = 180;
	sEnemy.fireCnt = 0;
	sEnemy.frameIdxHitSlash = 0;
	sEnemy.frameCntHitSlash = 0;
	sEnemy.distanceToPlayer = 0;
	sEnemy.founded = false;
	sEnemy.isDoorDead = false;
	sEnemy.isDoorDeadMove = false;
	sEnemy.speed = 1.0f;
	sEnemy.speed_old = 1.0f;
	sEnemy.isSplatted = false;
	sEnemy.detect.width = 600;
	sEnemy.detect.width_old = 600;
	sEnemy.detect.height = 130;
	sEnemy.reflect.isRender = false;
	sEnemy.reflect.img = IMAGEMANAGER->findImage("effect_reflect");
	sEnemy.reflect.frameIdx = 0;
	sEnemy.att.width = 45;
	sEnemy.att.height = 60;
	sEnemy.attackCnt = 0;

	for (int j = 0; j < BULLETSIZE; j++)
	{
		sEnemy.bullet[j].fire = false;
		sEnemy.bullet[j].isReflected = false;
		sEnemy.bullet[j].fireCnt = 0;
		sEnemy.bullet[j].speed = 14.0f;
		sEnemy.bullet[j].img = IMAGEMANAGER->findImage("enemy_bullet");
	}

	int rand = RANDOM->Range(1, 4);

	switch (rand)
	{
	case 1:
		sEnemy.bloodRemainImgL = IMAGEMANAGER->findImage("effect_blood_remain1_L");
		sEnemy.bloodRemainImgR = IMAGEMANAGER->findImage("effect_blood_remain1_R");
		break;
	case 2:
		sEnemy.bloodRemainImgL = IMAGEMANAGER->findImage("effect_blood_remain2_L");
		sEnemy.bloodRemainImgR = IMAGEMANAGER->findImage("effect_blood_remain2_R");
		break;
	case 3:
		sEnemy.bloodRemainImgL = IMAGEMANAGER->findImage("effect_blood_remain3_L");
		sEnemy.bloodRemainImgR = IMAGEMANAGER->findImage("effect_blood_remain3_R");
		break;
	case 4:
		sEnemy.bloodRemainImgL = IMAGEMANAGER->findImage("effect_blood_remain4_L");
		sEnemy.bloodRemainImgR = IMAGEMANAGER->findImage("effect_blood_remain4_R");
		break;
	}

	sEnemy.bloodImg = IMAGEMANAGER->findImage("effect_blood");
	sEnemy.slashHitImg = IMAGEMANAGER->findImage("effect_slashhit");
	

	switch (sEnemy.category)
	{
	case E_BOLD:
		sEnemy.width = 30;
		sEnemy.height = 70;
		sEnemy.attackTum = 70;
		sEnemy.findRange = 300;
		sEnemy.attackRange = 500;
		sEnemy.idleImg = IMAGEMANAGER->findImage("enemy_bold_idle");
		sEnemy.walkImg = IMAGEMANAGER->findImage("enemy_bold_walk");
		sEnemy.runImg = IMAGEMANAGER->findImage("enemy_bold_run");
		sEnemy.hurtImg = IMAGEMANAGER->findImage("enemy_bold_hurt");
		sEnemy.aimImg_L = IMAGEMANAGER->findImage("enemy_bold_aim_L");
		sEnemy.aimImg_R = IMAGEMANAGER->findImage("enemy_bold_aim_R");
		sEnemy.gunImg_L = IMAGEMANAGER->findImage("enemy_gun_L");
		sEnemy.gunImg_R = IMAGEMANAGER->findImage("enemy_gun_R");
		sEnemy.leftarm_L = IMAGEMANAGER->findImage("enemy_bold_leftarm_L");
		sEnemy.leftarm_R = IMAGEMANAGER->findImage("enemy_bold_leftarm_R");
		sEnemy.rightarm_L = IMAGEMANAGER->findImage("enemy_bold_rightarm_L");
		sEnemy.rightarm_R = IMAGEMANAGER->findImage("enemy_bold_rightarm_R");
		sEnemy.gunsparkImg = IMAGEMANAGER->findImage("effect_gunspark");

		break;
	case E_POMP:
		sEnemy.width = 30;
		sEnemy.height = 70;
		sEnemy.findRange = 30;
		sEnemy.attackRange = 35;
		sEnemy.idleImg = IMAGEMANAGER->findImage("enemy_pomp_idle");
		sEnemy.walkImg = IMAGEMANAGER->findImage("enemy_pomp_walk");
		sEnemy.runImg = IMAGEMANAGER->findImage("enemy_pomp_run");
		sEnemy.hurtImg = IMAGEMANAGER->findImage("enemy_pomp_hurt");
		sEnemy.attackImg = IMAGEMANAGER->findImage("enemy_pomp_attack");
		break;

	case E_GRUNT:
		sEnemy.width = 30;
		sEnemy.height = 70;
		sEnemy.findRange = 30;
		sEnemy.attackRange = 35;
		sEnemy.idleImg = IMAGEMANAGER->findImage("enemy_grunt_idle");
		sEnemy.walkImg = IMAGEMANAGER->findImage("enemy_grunt_walk");
		sEnemy.runImg = IMAGEMANAGER->findImage("enemy_grunt_run");
		sEnemy.hurtImg = IMAGEMANAGER->findImage("enemy_grunt_hurt");
		sEnemy.attackImg = IMAGEMANAGER->findImage("enemy_grunt_attack");
		sEnemy.slashImg = IMAGEMANAGER->findImage("enemy_grunt_slash");

		break;

	case E_COP:
		sEnemy.width = 30;
		sEnemy.height = 70;
		sEnemy.attackTum = 70;
		sEnemy.findRange = 300;
		sEnemy.attackRange = 500;
		sEnemy.idleImg = IMAGEMANAGER->findImage("enemy_cop_idle");
		sEnemy.walkImg = IMAGEMANAGER->findImage("enemy_cop_walk");
		sEnemy.runImg = IMAGEMANAGER->findImage("enemy_cop_run");
		sEnemy.hurtImg = IMAGEMANAGER->findImage("enemy_cop_hurt");
		sEnemy.aimImg_L = IMAGEMANAGER->findImage("enemy_cop_aim_L");
		sEnemy.aimImg_R = IMAGEMANAGER->findImage("enemy_cop_aim_R");
		sEnemy.gunImg_R = IMAGEMANAGER->findImage("enemy_gun_R");
		sEnemy.gunImg_L = IMAGEMANAGER->findImage("enemy_gun_L");
		sEnemy.leftarm_L = IMAGEMANAGER->findImage("enemy_cop_leftarm_L");
		sEnemy.leftarm_R = IMAGEMANAGER->findImage("enemy_cop_leftarm_R");
		sEnemy.rightarm_L = IMAGEMANAGER->findImage("enemy_cop_rightarm_L");
		sEnemy.rightarm_R = IMAGEMANAGER->findImage("enemy_cop_rightarm_R");
		sEnemy.gunsparkImg = IMAGEMANAGER->findImage("effect_gunspark");
		break;

	case E_TURRET:
		sEnemy.width = 100;
		sEnemy.height = 64;
		sEnemy.attackTum = 15;
		sEnemy.findRange = 550;
		sEnemy.attackRange = 500;
		sEnemy.idleImg = IMAGEMANAGER->findImage("enemy_turret_idle");
		sEnemy.attackImg = IMAGEMANAGER->findImage("enemy_turret_idle");
		sEnemy.hurtImg = IMAGEMANAGER->findImage("enemy_turret_die");
		sEnemy.gunsparkImg = IMAGEMANAGER->findImage("effect_gunspark");
		break;
	}
	vEnemy.push_back(sEnemy);
}

void enemyClass::generateEnemy(float _x, float _y, int _findRange, enum_EnemyCategory _category, enum_EnemyState _state, bool _set)
{
	sEnemy.detect.width = _findRange;
	sEnemy.detect.width_old = _findRange;
	sEnemy.x = _x;
	sEnemy.y = _y;
	sEnemy.alpha = 0;
	sEnemy.isRight = _set;
	sEnemy.isCrashedLeft = false;
	sEnemy.isCrashedRight = false;
	sEnemy.isCrashedTop = false;
	sEnemy.isCrashedBottom = false;
	sEnemy.isAttack = false;
	sEnemy.isDoorReached = false;
	sEnemy.isAttackReady = false;
	sEnemy.isAttackStart = false;
	sEnemy.isReflectDead = false;
	sEnemy.isFire = false;
	sEnemy.category = _category;
	sEnemy.state = _state;
	sEnemy.frameIdx = RANDOM->Range(0, 10);
	sEnemy.frameCnt = 0;
	sEnemy.frameCntBlood = 0;
	sEnemy.frameIdxBlood = 0;
	sEnemy.bloodAlpha = 180;
	sEnemy.fireCnt = 0;
	sEnemy.frameIdxHitSlash = 0;
	sEnemy.frameCntHitSlash = 0;
	sEnemy.distanceToPlayer = 0;
	sEnemy.founded = false;
	sEnemy.isDoorDead = false;
	sEnemy.isDoorDeadMove = false;
	sEnemy.speed = 1.0f;
	sEnemy.speed_old = 1.0f;
	sEnemy.isSplatted = false;
	sEnemy.detect.height = 130;
	sEnemy.reflect.isRender = false;
	sEnemy.reflect.img = IMAGEMANAGER->findImage("effect_reflect");
	sEnemy.reflect.frameIdx = 0;
	sEnemy.att.width = 45;
	sEnemy.att.height = 60;
	sEnemy.attackCnt = 0;

	for (int j = 0; j < BULLETSIZE; j++)
	{
		sEnemy.bullet[j].fire = false;
		sEnemy.bullet[j].isReflected = false;
		sEnemy.bullet[j].fireCnt = 0;
		sEnemy.bullet[j].speed = 14.0f;
		sEnemy.bullet[j].img = IMAGEMANAGER->findImage("enemy_bullet");
	}

	int rand = RANDOM->Range(1, 4);

	switch (rand)
	{
	case 1:
		sEnemy.bloodRemainImgL = IMAGEMANAGER->findImage("effect_blood_remain1_L");
		sEnemy.bloodRemainImgR = IMAGEMANAGER->findImage("effect_blood_remain1_R");
		break;
	case 2:
		sEnemy.bloodRemainImgL = IMAGEMANAGER->findImage("effect_blood_remain2_L");
		sEnemy.bloodRemainImgR = IMAGEMANAGER->findImage("effect_blood_remain2_R");
		break;
	case 3:
		sEnemy.bloodRemainImgL = IMAGEMANAGER->findImage("effect_blood_remain3_L");
		sEnemy.bloodRemainImgR = IMAGEMANAGER->findImage("effect_blood_remain3_R");
		break;
	case 4:
		sEnemy.bloodRemainImgL = IMAGEMANAGER->findImage("effect_blood_remain4_L");
		sEnemy.bloodRemainImgR = IMAGEMANAGER->findImage("effect_blood_remain4_R");
		break;
	}

	sEnemy.bloodImg = IMAGEMANAGER->findImage("effect_blood");
	sEnemy.slashHitImg = IMAGEMANAGER->findImage("effect_slashhit");


	switch (sEnemy.category)
	{
	case E_BOLD:
		sEnemy.width = 30;
		sEnemy.height = 70;
		sEnemy.attackTum = 70;
		sEnemy.findRange = _findRange;
		sEnemy.attackRange = 900;
		sEnemy.idleImg = IMAGEMANAGER->findImage("enemy_bold_idle");
		sEnemy.walkImg = IMAGEMANAGER->findImage("enemy_bold_walk");
		sEnemy.runImg = IMAGEMANAGER->findImage("enemy_bold_run");
		sEnemy.hurtImg = IMAGEMANAGER->findImage("enemy_bold_hurt");
		sEnemy.aimImg_L = IMAGEMANAGER->findImage("enemy_bold_aim_L");
		sEnemy.aimImg_R = IMAGEMANAGER->findImage("enemy_bold_aim_R");
		sEnemy.gunImg_L = IMAGEMANAGER->findImage("enemy_gun_L");
		sEnemy.gunImg_R = IMAGEMANAGER->findImage("enemy_gun_R");
		sEnemy.leftarm_L = IMAGEMANAGER->findImage("enemy_bold_leftarm_L");
		sEnemy.leftarm_R = IMAGEMANAGER->findImage("enemy_bold_leftarm_R");
		sEnemy.rightarm_L = IMAGEMANAGER->findImage("enemy_bold_rightarm_L");
		sEnemy.rightarm_R = IMAGEMANAGER->findImage("enemy_bold_rightarm_R");
		sEnemy.gunsparkImg = IMAGEMANAGER->findImage("effect_gunspark");

		break;
	case E_POMP:
		sEnemy.width = 30;
		sEnemy.height = 70;
		sEnemy.findRange = 30;
		sEnemy.attackRange = 35;
		sEnemy.idleImg = IMAGEMANAGER->findImage("enemy_pomp_idle");
		sEnemy.walkImg = IMAGEMANAGER->findImage("enemy_pomp_walk");
		sEnemy.runImg = IMAGEMANAGER->findImage("enemy_pomp_run");
		sEnemy.hurtImg = IMAGEMANAGER->findImage("enemy_pomp_hurt");
		sEnemy.attackImg = IMAGEMANAGER->findImage("enemy_pomp_attack");
		break;

	case E_GRUNT:
		sEnemy.width = 30;
		sEnemy.height = 70;
		sEnemy.findRange = 30;
		sEnemy.attackRange = 35;
		sEnemy.idleImg = IMAGEMANAGER->findImage("enemy_grunt_idle");
		sEnemy.walkImg = IMAGEMANAGER->findImage("enemy_grunt_walk");
		sEnemy.runImg = IMAGEMANAGER->findImage("enemy_grunt_run");
		sEnemy.hurtImg = IMAGEMANAGER->findImage("enemy_grunt_hurt");
		sEnemy.attackImg = IMAGEMANAGER->findImage("enemy_grunt_attack");
		sEnemy.slashImg = IMAGEMANAGER->findImage("enemy_grunt_slash");

		break;

	case E_COP:
		sEnemy.width = 30;
		sEnemy.height = 70;
		sEnemy.attackTum = 70;
		sEnemy.findRange = _findRange;
		sEnemy.attackRange = 1300;
		sEnemy.idleImg = IMAGEMANAGER->findImage("enemy_cop_idle");
		sEnemy.walkImg = IMAGEMANAGER->findImage("enemy_cop_walk");
		sEnemy.runImg = IMAGEMANAGER->findImage("enemy_cop_run");
		sEnemy.hurtImg = IMAGEMANAGER->findImage("enemy_cop_hurt");
		sEnemy.aimImg_L = IMAGEMANAGER->findImage("enemy_cop_aim_L");
		sEnemy.aimImg_R = IMAGEMANAGER->findImage("enemy_cop_aim_R");
		sEnemy.gunImg_R = IMAGEMANAGER->findImage("enemy_gun_R");
		sEnemy.gunImg_L = IMAGEMANAGER->findImage("enemy_gun_L");
		sEnemy.leftarm_L = IMAGEMANAGER->findImage("enemy_cop_leftarm_L");
		sEnemy.leftarm_R = IMAGEMANAGER->findImage("enemy_cop_leftarm_R");
		sEnemy.rightarm_L = IMAGEMANAGER->findImage("enemy_cop_rightarm_L");
		sEnemy.rightarm_R = IMAGEMANAGER->findImage("enemy_cop_rightarm_R");
		sEnemy.gunsparkImg = IMAGEMANAGER->findImage("effect_gunspark");
		break;

	case E_TURRET:
		sEnemy.width = 100;
		sEnemy.height = 64;
		sEnemy.attackTum = 15;
		sEnemy.findRange = _findRange;
		sEnemy.attackRange = 500;
		sEnemy.idleImg = IMAGEMANAGER->findImage("enemy_turret_idle");
		sEnemy.attackImg = IMAGEMANAGER->findImage("enemy_turret_idle");
		sEnemy.hurtImg = IMAGEMANAGER->findImage("enemy_turret_die");
		sEnemy.gunsparkImg = IMAGEMANAGER->findImage("effect_gunspark");
		break;
	}
	vEnemy.push_back(sEnemy);
}

void enemyClass::generateEnemy(float _x, float _y, int _findRange, bool _found, enum_EnemyCategory _category, enum_EnemyState _state, bool _set)
{
	sEnemy.founded = true;
	sEnemy.detect.width = _findRange;
	sEnemy.detect.width_old = _findRange;
	sEnemy.x = _x;
	sEnemy.y = _y;
	sEnemy.alpha = 0;
	sEnemy.isRight = _set;
	sEnemy.isCrashedLeft = false;
	sEnemy.isCrashedRight = false;
	sEnemy.isCrashedTop = false;
	sEnemy.isCrashedBottom = false;
	sEnemy.isAttack = false;
	sEnemy.isDoorReached = false;
	sEnemy.isAttackReady = false;
	sEnemy.isAttackStart = false;
	sEnemy.isReflectDead = false;
	sEnemy.isFire = false;
	sEnemy.category = _category;
	sEnemy.state = _state;
	sEnemy.frameIdx = RANDOM->Range(0, 10);
	sEnemy.frameCnt = 0;
	sEnemy.frameCntBlood = 0;
	sEnemy.frameIdxBlood = 0;
	sEnemy.bloodAlpha = 180;
	sEnemy.fireCnt = 0;
	sEnemy.frameIdxHitSlash = 0;
	sEnemy.frameCntHitSlash = 0;
	sEnemy.distanceToPlayer = 0;
	sEnemy.founded = false;
	sEnemy.isDoorDead = false;
	sEnemy.isDoorDeadMove = false;
	sEnemy.speed = 1.0f;
	sEnemy.speed_old = 1.0f;
	sEnemy.isSplatted = false;
	sEnemy.detect.height = 130;
	sEnemy.reflect.isRender = false;
	sEnemy.reflect.img = IMAGEMANAGER->findImage("effect_reflect");
	sEnemy.reflect.frameIdx = 0;
	sEnemy.att.width = 45;
	sEnemy.att.height = 60;
	sEnemy.attackCnt = 0;

	for (int j = 0; j < BULLETSIZE; j++)
	{
		sEnemy.bullet[j].fire = false;
		sEnemy.bullet[j].isReflected = false;
		sEnemy.bullet[j].fireCnt = 0;
		sEnemy.bullet[j].speed = 14.0f;
		sEnemy.bullet[j].img = IMAGEMANAGER->findImage("enemy_bullet");
	}

	int rand = RANDOM->Range(1, 4);

	switch (rand)
	{
	case 1:
		sEnemy.bloodRemainImgL = IMAGEMANAGER->findImage("effect_blood_remain1_L");
		sEnemy.bloodRemainImgR = IMAGEMANAGER->findImage("effect_blood_remain1_R");
		break;
	case 2:
		sEnemy.bloodRemainImgL = IMAGEMANAGER->findImage("effect_blood_remain2_L");
		sEnemy.bloodRemainImgR = IMAGEMANAGER->findImage("effect_blood_remain2_R");
		break;
	case 3:
		sEnemy.bloodRemainImgL = IMAGEMANAGER->findImage("effect_blood_remain3_L");
		sEnemy.bloodRemainImgR = IMAGEMANAGER->findImage("effect_blood_remain3_R");
		break;
	case 4:
		sEnemy.bloodRemainImgL = IMAGEMANAGER->findImage("effect_blood_remain4_L");
		sEnemy.bloodRemainImgR = IMAGEMANAGER->findImage("effect_blood_remain4_R");
		break;
	}

	sEnemy.bloodImg = IMAGEMANAGER->findImage("effect_blood");
	sEnemy.slashHitImg = IMAGEMANAGER->findImage("effect_slashhit");


	switch (sEnemy.category)
	{
	case E_BOLD:
		sEnemy.width = 30;
		sEnemy.height = 70;
		sEnemy.attackTum = 70;
		sEnemy.findRange = _findRange;
		sEnemy.attackRange = 900;
		sEnemy.idleImg = IMAGEMANAGER->findImage("enemy_bold_idle");
		sEnemy.walkImg = IMAGEMANAGER->findImage("enemy_bold_walk");
		sEnemy.runImg = IMAGEMANAGER->findImage("enemy_bold_run");
		sEnemy.hurtImg = IMAGEMANAGER->findImage("enemy_bold_hurt");
		sEnemy.aimImg_L = IMAGEMANAGER->findImage("enemy_bold_aim_L");
		sEnemy.aimImg_R = IMAGEMANAGER->findImage("enemy_bold_aim_R");
		sEnemy.gunImg_L = IMAGEMANAGER->findImage("enemy_gun_L");
		sEnemy.gunImg_R = IMAGEMANAGER->findImage("enemy_gun_R");
		sEnemy.leftarm_L = IMAGEMANAGER->findImage("enemy_bold_leftarm_L");
		sEnemy.leftarm_R = IMAGEMANAGER->findImage("enemy_bold_leftarm_R");
		sEnemy.rightarm_L = IMAGEMANAGER->findImage("enemy_bold_rightarm_L");
		sEnemy.rightarm_R = IMAGEMANAGER->findImage("enemy_bold_rightarm_R");
		sEnemy.gunsparkImg = IMAGEMANAGER->findImage("effect_gunspark");

		break;
	case E_POMP:
		sEnemy.width = 30;
		sEnemy.height = 70;
		sEnemy.findRange = 30;
		sEnemy.attackRange = 35;
		sEnemy.idleImg = IMAGEMANAGER->findImage("enemy_pomp_idle");
		sEnemy.walkImg = IMAGEMANAGER->findImage("enemy_pomp_walk");
		sEnemy.runImg = IMAGEMANAGER->findImage("enemy_pomp_run");
		sEnemy.hurtImg = IMAGEMANAGER->findImage("enemy_pomp_hurt");
		sEnemy.attackImg = IMAGEMANAGER->findImage("enemy_pomp_attack");
		break;

	case E_GRUNT:
		sEnemy.width = 30;
		sEnemy.height = 70;
		sEnemy.findRange = 30;
		sEnemy.attackRange = 35;
		sEnemy.idleImg = IMAGEMANAGER->findImage("enemy_grunt_idle");
		sEnemy.walkImg = IMAGEMANAGER->findImage("enemy_grunt_walk");
		sEnemy.runImg = IMAGEMANAGER->findImage("enemy_grunt_run");
		sEnemy.hurtImg = IMAGEMANAGER->findImage("enemy_grunt_hurt");
		sEnemy.attackImg = IMAGEMANAGER->findImage("enemy_grunt_attack");
		sEnemy.slashImg = IMAGEMANAGER->findImage("enemy_grunt_slash");

		break;

	case E_COP:
		sEnemy.width = 30;
		sEnemy.height = 70;
		sEnemy.attackTum = 70;
		sEnemy.findRange = _findRange;
		sEnemy.attackRange = 900;
		sEnemy.idleImg = IMAGEMANAGER->findImage("enemy_cop_idle");
		sEnemy.walkImg = IMAGEMANAGER->findImage("enemy_cop_walk");
		sEnemy.runImg = IMAGEMANAGER->findImage("enemy_cop_run");
		sEnemy.hurtImg = IMAGEMANAGER->findImage("enemy_cop_hurt");
		sEnemy.aimImg_L = IMAGEMANAGER->findImage("enemy_cop_aim_L");
		sEnemy.aimImg_R = IMAGEMANAGER->findImage("enemy_cop_aim_R");
		sEnemy.gunImg_R = IMAGEMANAGER->findImage("enemy_gun_R");
		sEnemy.gunImg_L = IMAGEMANAGER->findImage("enemy_gun_L");
		sEnemy.leftarm_L = IMAGEMANAGER->findImage("enemy_cop_leftarm_L");
		sEnemy.leftarm_R = IMAGEMANAGER->findImage("enemy_cop_leftarm_R");
		sEnemy.rightarm_L = IMAGEMANAGER->findImage("enemy_cop_rightarm_L");
		sEnemy.rightarm_R = IMAGEMANAGER->findImage("enemy_cop_rightarm_R");
		sEnemy.gunsparkImg = IMAGEMANAGER->findImage("effect_gunspark");
		break;

	case E_TURRET:
		sEnemy.width = 100;
		sEnemy.height = 64;
		sEnemy.attackTum = 15;
		sEnemy.findRange = _findRange;
		sEnemy.attackRange = 500;
		sEnemy.idleImg = IMAGEMANAGER->findImage("enemy_turret_idle");
		sEnemy.attackImg = IMAGEMANAGER->findImage("enemy_turret_idle");
		sEnemy.hurtImg = IMAGEMANAGER->findImage("enemy_turret_die");
		sEnemy.gunsparkImg = IMAGEMANAGER->findImage("effect_gunspark");
		break;
	}
	vEnemy.push_back(sEnemy);
}

void enemyClass::enemyAnimation(int i)
{
	switch (vEnemy[i].category)
	{
	case E_BOLD:
		switch (vEnemy[i].state)
		{
		case E_IDLE:
			vEnemy[i].frameTum = 7;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}

			if (vEnemy[i].isRight)
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx++;
					if (vEnemy[i].frameIdx > 7)
					{
						vEnemy[i].frameIdx = 0;
					}
				}
			}
			else
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					if (vEnemy[i].frameIdx < 0)
					{
						vEnemy[i].frameIdx = 8;
					}
				}
			}
			break;
		case E_GEN:
			vEnemy[i].frameTum = 7;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}

			if (vEnemy[i].isRight)
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx++;
					vEnemy[i].alpha += 55;
					if (vEnemy[i].alpha >= 255)
					{
						vEnemy[i].state = E_IDLE;
					}
					if (vEnemy[i].frameIdx > 7)
					{
						vEnemy[i].frameIdx = 0;
					}
				}
			}
			else
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					vEnemy[i].alpha += 55;
					if (vEnemy[i].alpha >= 255)
					{
						vEnemy[i].state = E_IDLE;
					}
					if (vEnemy[i].frameIdx < 0)
					{
						vEnemy[i].frameIdx = 8;
					}
				}
			}
			break;
		case E_WALK:
			vEnemy[i].frameTum = 7;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}
			if (vEnemy[i].isRight)
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx++;
					if (vEnemy[i].frameIdx > 7)
					{
						vEnemy[i].frameIdx = 0;
					}
				}
			}
			else
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					if (vEnemy[i].frameIdx < 1)
					{
						vEnemy[i].frameIdx = 7;
					}
				}
			}
			break;
		case E_RUN:
			vEnemy[i].frameTum = 3;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}
			if (vEnemy[i].isRight)
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx++;
					if (vEnemy[i].frameIdx > 9)
					{
						vEnemy[i].frameIdx = 0;
					}
				}
			}
			else
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					if (vEnemy[i].frameIdx < 0)
					{
						vEnemy[i].frameIdx = 10;
					}
				}
			}
			break;
		case E_HURT:
			vEnemy[i].frameTum = 7;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}

			if (vEnemy[i].isRight)
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx++;
					if (vEnemy[i].frameIdx > 14)
					{
						vEnemy[i].frameIdx = 15;
					}
				}
			}
			else
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					if (vEnemy[i].frameIdx < 1)
					{
						vEnemy[i].frameIdx = 0;

					}
				}
			}

			break;
		case E_AIM:
			if (vEnemy[i].isFire)
			{
				vEnemy[i].frameTum = 3;
				vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
				if (SLOW_inEnemy)
				{
					vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
				}
				else
				{
					vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
				}
				if (vEnemy[i].isRight)
				{
					if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
					{
						vEnemy[i].frameIdx++;
						if (vEnemy[i].frameIdx > 3)
						{
							vEnemy[i].frameIdx = 0;
							vEnemy[i].isFire = false;
						}
					}
				}
				else
				{
					if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
					{
						vEnemy[i].frameIdx--;
						if (vEnemy[i].frameIdx < 1)
						{
							vEnemy[i].frameIdx = 4;
							vEnemy[i].isFire = false;
						}
					}
				}
				break;
			}
		}
		break;
	case E_POMP:
		switch (vEnemy[i].state)
		{
		case E_IDLE:
			vEnemy[i].frameTum = 7;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}

			if (vEnemy[i].isRight)
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx++;
					if (vEnemy[i].frameIdx > 8)
					{
						vEnemy[i].frameIdx = 0;
					}
				}
			}
			else
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					if (vEnemy[i].frameIdx < 0)
					{
						vEnemy[i].frameIdx = 8;
					}
				}
			}
			break;
		case E_GEN:
			vEnemy[i].frameTum = 7;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}

			if (vEnemy[i].isRight)
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx++;
					vEnemy[i].alpha += 55;
					if (vEnemy[i].alpha >= 255)
					{
						vEnemy[i].state = E_IDLE;
					}
					if (vEnemy[i].frameIdx > 8)
					{
						vEnemy[i].frameIdx = 0;
					}
				}
			}
			else
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					vEnemy[i].alpha += 55;
					if (vEnemy[i].alpha >= 255)
					{
						vEnemy[i].state = E_IDLE;
					}
					if (vEnemy[i].frameIdx < 0)
					{
						vEnemy[i].frameIdx = 8;
					}
				}
			}
			break;
		case E_WALK:
			vEnemy[i].frameTum = 7;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}
			if (vEnemy[i].isRight)
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx++;
					if (vEnemy[i].frameIdx > 8)
					{
						vEnemy[i].frameIdx = 0;
					}
				}
			}
			else
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					if (vEnemy[i].frameIdx < 0)
					{
						vEnemy[i].frameIdx = 8;
					}
				}
			}
			break;
		case E_RUN:
			vEnemy[i].frameTum = 3;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}
			if (vEnemy[i].isRight)
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx++;
					if (vEnemy[i].frameIdx > 10)
					{
						vEnemy[i].frameIdx = 0;
					}
				}
			}
			else
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					if (vEnemy[i].frameIdx < 0)
					{
						vEnemy[i].frameIdx = 10;
					}
				}
			}
			break;
		case E_HURT:
			vEnemy[i].frameTum = 7;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}
			if (vEnemy[i].isRight)
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx++;
					if (vEnemy[i].frameIdx > 14)
					{
						vEnemy[i].frameIdx = 15;

					}
				}
			}
			else
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					if (vEnemy[i].frameIdx < 1)
					{
						vEnemy[i].frameIdx = 0;

					}
				}
			}
			break;
		case E_ATTACK:
			vEnemy[i].frameTum = 5;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}
			if (vEnemy[i].isRight)
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx++;
					if (vEnemy[i].frameIdx > 3)vEnemy[i].isAttack = true;
					else if (vEnemy[i].frameIdx <= 3 )vEnemy[i].isAttack = false;
					if (vEnemy[i].frameIdx > 6)
					{
						vEnemy[i].frameIdx = 0;
					}
				}
			}
			else
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					if (vEnemy[i].frameIdx < 2)vEnemy[i].isAttack = true;
					else if(vEnemy[i].frameIdx >= 2)vEnemy[i].isAttack = false;
					if (vEnemy[i].frameIdx < 1)
					{
						vEnemy[i].frameIdx = 6;
					}
				}
			}
			break;
		}
		break;
	case E_COP:
		switch (vEnemy[i].state)
		{
		case E_IDLE:
			vEnemy[i].frameTum = 7;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}
			if (vEnemy[i].isRight)
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx++;
					if (vEnemy[i].frameIdx > 7)
					{
						vEnemy[i].frameIdx = 0;
					}
				}
			}
			else
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					if (vEnemy[i].frameIdx < 1)
					{
						vEnemy[i].frameIdx = 7;
					}
				}
			}
			break;
		case E_GEN:
			vEnemy[i].frameTum = 7;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}
			if (vEnemy[i].isRight)
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx++;
					vEnemy[i].alpha += 55;
					if (vEnemy[i].alpha >= 255)
					{
						vEnemy[i].state = E_IDLE;
					}
					if (vEnemy[i].frameIdx > 7)
					{
						vEnemy[i].frameIdx = 0;
					}
				}
			}
			else
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					vEnemy[i].alpha += 55;
					if (vEnemy[i].alpha >= 255)
					{
						vEnemy[i].state = E_IDLE;
					}
					if (vEnemy[i].frameIdx < 1)
					{
						vEnemy[i].frameIdx = 7;
					}
				}
			}
			break;
		case E_WALK:
			vEnemy[i].frameTum = 7;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}
			if (vEnemy[i].isRight)
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx++;
					if (vEnemy[i].frameIdx > 7)
					{
						vEnemy[i].frameIdx = 0;
					}
				}
			}
			else
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					if (vEnemy[i].frameIdx < 1)
					{
						vEnemy[i].frameIdx = 7;
					}
				}
			}
			break;
		case E_RUN:
			vEnemy[i].frameTum = 3;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}
			if (vEnemy[i].isRight)
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx++;
					if (vEnemy[i].frameIdx > 9)
					{
						vEnemy[i].frameIdx = 0;
					}
				}
			}
			else
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					if (vEnemy[i].frameIdx < 1)
					{
						vEnemy[i].frameIdx = 9;
					}
				}
			}
			break;
		case E_HURT:
			vEnemy[i].frameTum = 7;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}
			if (vEnemy[i].isRight)
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx++;
					if (vEnemy[i].frameIdx > 13)
					{
						vEnemy[i].frameIdx = 14;

					}
				}
			}
			else
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					if (vEnemy[i].frameIdx < 0)
					{
						vEnemy[i].frameIdx = 0;

					}
				}
			}
			break;
		case E_AIM:
			if (vEnemy[i].isFire)
			{
				vEnemy[i].frameTum = 3;
				vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
				if (SLOW_inEnemy)
				{
					vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
				}
				else
				{
					vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
				}
				if (vEnemy[i].isRight)
				{
					if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
					{
						vEnemy[i].frameIdx++;
						if (vEnemy[i].frameIdx > 3)
						{
							vEnemy[i].frameIdx = 0;
							vEnemy[i].isFire = false;
						}
					}
				}
				else
				{
					if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
					{
						vEnemy[i].frameIdx--;
						if (vEnemy[i].frameIdx < 1)
						{
							vEnemy[i].frameIdx = 4;
							vEnemy[i].isFire = false;
						}
					}
				}
				break;
			}
		}
		break;
	case E_GRUNT:
		switch (vEnemy[i].state)
		{
		case E_IDLE:
			vEnemy[i].frameTum = 7;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}
			if (vEnemy[i].isRight)
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx++;
					if (vEnemy[i].frameIdx > 7)
					{
						vEnemy[i].frameIdx = 0;
					}
				}
			}
			else
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					if (vEnemy[i].frameIdx < 0)
					{
						vEnemy[i].frameIdx = 7;
					}
				}
			}
			break;
		case E_GEN:
			vEnemy[i].frameTum = 7;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}
			if (vEnemy[i].isRight)
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx++;
					vEnemy[i].alpha += 30;
					if (vEnemy[i].alpha >= 255)
					{
						vEnemy[i].state = E_IDLE;
					}
					if (vEnemy[i].frameIdx > 7)
					{
						vEnemy[i].frameIdx = 0;
					}
				}
			}
			else
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					vEnemy[i].alpha += 30;
					if (vEnemy[i].alpha >= 255)
					{
						vEnemy[i].state = E_IDLE;
					}
					if (vEnemy[i].frameIdx < 0)
					{
						vEnemy[i].frameIdx = 7;
					}
				}
			}
			break;
		case E_WALK:
			vEnemy[i].frameTum = 7;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}
			if (vEnemy[i].isRight)
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx++;
					if (vEnemy[i].frameIdx > 9)
					{
						vEnemy[i].frameIdx = 0;
					}
				}
			}
			else
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					if (vEnemy[i].frameIdx < 0)
					{
						vEnemy[i].frameIdx = 9;
					}
				}
			}
			break;
		case E_RUN:
			vEnemy[i].frameTum = 3;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}
			if (vEnemy[i].isRight)
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx++;
					if (vEnemy[i].frameIdx > 9)
					{
						vEnemy[i].frameIdx = 0;
					}
				}
			}
			else
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					if (vEnemy[i].frameIdx < 0)
					{
						vEnemy[i].frameIdx = 9;
					}
				}
			}
			break;
		case E_HURT:
			vEnemy[i].frameTum = 7;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}
			if (vEnemy[i].isRight)
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx++;
					if (vEnemy[i].frameIdx > 14)
					{
						vEnemy[i].frameIdx = 15;

					}
				}
			}
			else
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					if (vEnemy[i].frameIdx < 0)
					{
						vEnemy[i].frameIdx = 0;

					}
				}
			}
			break;

		case E_ATTACK:
			vEnemy[i].frameTum = 5;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}
			if (vEnemy[i].isRight)
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx++;
					if (vEnemy[i].frameIdx > 4)vEnemy[i].isAttack = true;
					else if (vEnemy[i].frameIdx <= 4)vEnemy[i].isAttack = false;
					if (vEnemy[i].frameIdx > 7)
					{
						vEnemy[i].frameIdx = 0;
					}
				}
			}
			else
			{
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					if (vEnemy[i].frameIdx < 2)vEnemy[i].isAttack = true;
					else if (vEnemy[i].frameIdx >= 2)vEnemy[i].isAttack = false;
					else vEnemy[i].isAttack = false;
					if (vEnemy[i].frameIdx < 0)
					{
						vEnemy[i].frameIdx = 7;
					}
				}
			}
			break;

		}
		break;
	case E_TURRET:
		switch (vEnemy[i].state)
		{
		case E_IDLE:
			vEnemy[i].frameIdx = 0;
			break;
		case E_HURT:
			vEnemy[i].frameTum = 7;
			vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
			if (SLOW_inEnemy)
			{
				vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
			}
			else
			{
				vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
			}
		
			
				if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
				{
					vEnemy[i].frameIdx--;
					if (vEnemy[i].frameIdx < 1)
					{
						vEnemy[i].frameIdx = 0;

					}
				}
			
			break;
		case E_AIM:
			if (vEnemy[i].isFire)
			{
				vEnemy[i].frameTum = 3;
				vEnemy[i].oldFrameTum = vEnemy[i].frameTum;
				if (SLOW_inEnemy)
				{
					vEnemy[i].frameTum *= SLOW_Ratio_inEnemy;
				}
				else
				{
					vEnemy[i].frameTum = vEnemy[i].oldFrameTum;
				}
				if (vEnemy[i].isRight)
				{
					if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
					{
						vEnemy[i].frameIdx++;
						if (vEnemy[i].frameIdx > 3)
						{
							vEnemy[i].frameIdx = 0;
							vEnemy[i].isFire = false;
						}
					}
				}
				else
				{
					if (vEnemy[i].frameCnt % vEnemy[i].frameTum == 0)
					{
						vEnemy[i].frameIdx--;
						if (vEnemy[i].frameIdx < 1)
						{
							vEnemy[i].frameIdx = 4;
							vEnemy[i].isFire = false;
						}
					}
				}
				break;
			}
		}

	}
	
}

void enemyClass::bloodAnimation(int i)
{
	if (vEnemy[i].state == E_HURT)
	{
		vEnemy[i].bloodFrameTum = 7;
		vEnemy[i].oldBloodFrameTum = vEnemy[i].bloodFrameTum;
		if (SLOW_inEnemy)
		{
			vEnemy[i].bloodFrameTum *= SLOW_Ratio_inEnemy;
		}
		else
		{
			vEnemy[i].bloodFrameTum = vEnemy[i].oldBloodFrameTum;
		}

		vEnemy[i].slashFrameTum = 3;
		vEnemy[i].oldSlashFrameTum = vEnemy[i].slashFrameTum;
		if (SLOW_inEnemy)
		{
			vEnemy[i].slashFrameTum *= SLOW_Ratio_inEnemy;
		}
		else
		{
			vEnemy[i].slashFrameTum = vEnemy[i].oldSlashFrameTum;
		}

		//혈흔 + 슬래쉬이펙트
		if (vEnemy[i].isPlayerRight)
		{
			if (vEnemy[i].frameCnt % vEnemy[i].bloodFrameTum == 0)
			{
				vEnemy[i].frameIdxBlood++;
				if (vEnemy[i].frameIdxBlood > 5)vEnemy[i].isSplatted = true;

				if (vEnemy[i].frameIdxBlood > 10)
				{
					vEnemy[i].frameIdxBlood = 11;
				}
			}

			if (vEnemy[i].frameCnt % vEnemy[i].slashFrameTum == 0)
			{
				vEnemy[i].frameIdxHitSlash++;

				if (vEnemy[i].frameIdxHitSlash > 5)
				{
					vEnemy[i].frameIdxHitSlash = 6;
				}
			}
		}
		else
		{
			if (vEnemy[i].frameCnt % vEnemy[i].bloodFrameTum == 0)
			{
				vEnemy[i].frameIdxBlood--;
				if (vEnemy[i].frameIdxBlood < 5)vEnemy[i].isSplatted = true;

				if (vEnemy[i].frameIdxBlood < 1)
				{
					vEnemy[i].frameIdxBlood = 0;
				}
			}

			if (vEnemy[i].frameCnt % vEnemy[i].slashFrameTum == 0)
			{
				vEnemy[i].frameIdxHitSlash--;

				if (vEnemy[i].frameIdxHitSlash < 1)
				{
					vEnemy[i].frameIdxHitSlash = 0;
				}
			}
		}
	}
	
}

void enemyClass::enemySetBullet(int i)
{
	for (int j = 0; j < BULLETSIZE; j++)
	{
		if (vEnemy[i].state == E_HURT)
		{
			vEnemy[i].bullet[j].rc = RectMakeCenter(-100,-100,0,0);
		}
		else
		{
			vEnemy[i].bullet[j].rc = RectMakeCenter(vEnemy[i].bullet[j].x - DATA->getBgCol().x, vEnemy[i].bullet[j].y - DATA->getBgCol().y, 10, 10);
		}
	}
		
	
}

void enemyClass::enemyShoot(int i)
{
	if (vEnemy[i].fireCnt % vEnemy[i].attackTum * SLOW_Ratio_inEnemy == 0)
	{
		for (int j = 0; j < BULLETSIZE; j++)
		{
			if (vEnemy[i].bullet[j].fire == true)continue;
			{
				vEnemy[i].isFire = true;
				vEnemy[i].bullet->isReflected = false;
				vEnemy[i].bullet[j].fireCnt++;
				//if (vEnemy[i].bullet[j].fireCnt % 50 == 0)
				//{
				vEnemy[i].bullet[j].fire = true;
				if (vEnemy[i].category == E_TURRET)
				{
					vEnemy[i].bullet[j].angle = PI;
				}
				else
				{
					vEnemy[i].bullet[j].angle = vEnemy[i].angleToPlayer - PI;
				}
				
				if (vEnemy[i].isRight)
				{
					vEnemy[i].frameIdx = 0;
					vEnemy[i].bullet[j].x = vEnemy[i].x;
					vEnemy[i].bullet[j].y = vEnemy[i].y - 8;
				}
				else
				{
					vEnemy[i].frameIdx = 4;
					vEnemy[i].bullet[j].x = vEnemy[i].x - 30;
					vEnemy[i].bullet[j].y = vEnemy[i].y - 8;
				}
				//	}
				vEnemy[i].fireCnt = 0;
				break;
			}
		}
	}
}

void enemyClass::enemyMoveBullet(int i)
{
	for (int j = 0; j < BULLETSIZE; j++)
	{
		if (vEnemy[i].bullet[j].fire == false)continue;
		{
			vEnemy[i].bullet[j].x += cosf(vEnemy[i].bullet[j].angle) * (vEnemy[i].bullet[j].speed / (SLOW_Ratio_inEnemy/2));
			vEnemy[i].bullet[j].y += -sinf(vEnemy[i].bullet[j].angle) * (vEnemy[i].bullet[j].speed / (SLOW_Ratio_inEnemy/2));
			
			//총알 픽셀 충돌
			COLORREF color = GetPixel(DATA->getBgCol().img->getMemDC(), vEnemy[i].bullet[j].x, vEnemy[i].bullet[j].y);
			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);

			if (!(r == 255 && g == 0 && b == 255))
			{
				if (r == 0 && g == 0 && b == 0)continue;
				vEnemy[i].bullet[j].fire = false;
			}
		}
	}
}

void enemyClass::enemyMoveToPlayer(int i)
{
	if (vEnemy[i].distanceToPlayer > vEnemy[i].findRange + DATA->getBgCol().x && vEnemy[i].isAttackReady == false)
	{
		vEnemy[i].attackCnt = 0;
		if (vEnemy[i].category != E_TURRET)
		{
			vEnemy[i].state = E_RUN;
			if (vEnemy[i].isCrashedLeft == false && vEnemy[i].isRight == false)
			{
				vEnemy[i].x -= cosf(vEnemy[i].angleToPlayer) * (vEnemy[i].speed * 5);
			}
			if (vEnemy[i].isCrashedRight == false && vEnemy[i].isRight == true)
			{
				vEnemy[i].x -= cosf(vEnemy[i].angleToPlayer) * (vEnemy[i].speed * 5);
			}
		}
	}
	else
	{
		vEnemy[i].attackCnt++;
		vEnemy[i].state = E_IDLE;
		if (vEnemy[i].attackCnt > 5)
		{
			vEnemy[i].attackCnt = 0;
			vEnemy[i].isAttackReady = true;
		}
		
	}

	if (vEnemy[i].isAttackReady == true)
	{
		if (vEnemy[i].distanceToPlayer < vEnemy[i].attackRange + DATA->getBgCol().x )
		{
			switch (vEnemy[i].category)
			{
			case E_GRUNT: vEnemy[i].state = E_ATTACK;
				if (vEnemy[i].isAttackStart == false)
				{
					if (vEnemy[i].isRight)vEnemy[i].frameIdx = 0;
					else vEnemy[i].frameIdx = 6;
					vEnemy[i].isAttackStart = true;
				}
				break;
			case E_POMP: vEnemy[i].state = E_ATTACK;
				if (vEnemy[i].isAttackStart == false)
				{
					if (vEnemy[i].isRight)vEnemy[i].frameIdx = 0;
					else vEnemy[i].frameIdx = 5;
					vEnemy[i].isAttackStart = true;
				}
				break;
			case E_BOLD: vEnemy[i].state = E_AIM;
				break;
			case E_COP: vEnemy[i].state = E_AIM;
				break;
			case E_TURRET: vEnemy[i].state = E_AIM;
				break;
			}
		}
		else
		{
			vEnemy[i].state = E_IDLE;
			vEnemy[i].isAttack = false;
			vEnemy[i].isAttackReady = false;
			vEnemy[i].isAttackStart = false;
		}
	}


}

void enemyClass::enemyMove(int i)
{
	vEnemy[i].isAttack = false;
	if (vEnemy[i].state == E_HURT) return;
	if (vEnemy[i].state == E_IDLE) return;
	if (vEnemy[i].category == E_TURRET) return;
	if (vEnemy[i].state == E_GEN) return;
	
	vEnemy[i].state = E_WALK;
	if (vEnemy[i].isRight)
	{
		if (vEnemy[i].isCrashedRight == false)
		{
			vEnemy[i].x += vEnemy[i].speed;
		}
		
	}
	else
	{
		if (vEnemy[i].isCrashedLeft == false)
		{
			vEnemy[i].x -= vEnemy[i].speed;
		}
	}

}

void enemyClass::enemyPixelCollisionleft(int i)
{
	//픽셀충돌 왼쪽
	for (int j = vEnemy[i].y - vEnemy[i].height / 2; j < vEnemy[i].y; j++)
	{
		COLORREF color = GetPixel(DATA->getBgCol().img->getMemDC(), vEnemy[i].findL, j);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255 || r == 0 && g == 0 && b == 0))
		{
			vEnemy[i].isRight = true;
			vEnemy[i].isCrashedLeft = true;
			break;
		}
		else
		{
			vEnemy[i].isCrashedLeft = false;
		}
	}
}

void enemyClass::enemyPixelCollisionRight(int i)
{
	//픽셀충돌 왼쪽
	for (int j = vEnemy[i].y - vEnemy[i].height / 2; j < vEnemy[i].y; j++)
	{
		COLORREF color = GetPixel(DATA->getBgCol().img->getMemDC(), vEnemy[i].findR, j);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255 || r == 0 && g == 0 && b == 0))
		{
			vEnemy[i].isRight = false;
			vEnemy[i].isCrashedRight == true;
			break;
		}
		else
		{
			vEnemy[i].isCrashedRight == false;
		}
	}
}

void enemyClass::enemyPixelCollisionBottom(int i)
{
	//픽셀충돌 아래
	for (int j = vEnemy[i].x - vEnemy[i].width / 2; j < vEnemy[i].x + vEnemy[i].width / 2; j++)
	{
		COLORREF color = GetPixel(DATA->getBgCol().img->getMemDC(), j, vEnemy[i].findB);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			vEnemy[i].isCrashedBottom = true;
			break;
		}
		else
		{
			vEnemy[i].isCrashedBottom = false;
		}

	}
}

void enemyClass::enemyOnFloor(int i)
{
	/*이부분이 픽셀충돌의 핵심*/
	for (int j = vEnemy[i].findY - 35; j < vEnemy[i].findY + 35; j++)
	{
		COLORREF color = GetPixel(DATA->getBgCol().img->getMemDC(), vEnemy[i].x, j);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255)&& vEnemy[i].isCrashedLeft == false && vEnemy[i].isCrashedRight == false)
		{
			float tempY = vEnemy[i].y;
			vEnemy[i].y = j - vEnemy[i].height / 2;
			//ON_FLOOR = true;
			break;
		}
		else
		{
			//ON_FLOOR = false;
		}
	}
}




#include "stdafx.h"
#include "stage2.h"

HRESULT stage2::init()
{
	DATA->setBgColImg(IMAGEMANAGER->findImage("stage2_bg_collision"));
	DATA->setBgRenImg(IMAGEMANAGER->findImage("stage2_bg_render"));
	DATA->setBgRenImg(IMAGEMANAGER->findImage("stage2_bg_render_slow"));
	DATA->setHud(IMAGEMANAGER->findImage("hud_collapse"));
	DATA->stageInitialize(0, 275, 4288, 1000);
	DATA->setStartLoc(50, 720);

	fade.blackImg = IMAGEMANAGER->findImage("effect_black");
	fade.successImg = IMAGEMANAGER->findImage("effect_success");
	fade.failImg = IMAGEMANAGER->findImage("effect_fail");
	fade.cnt = fade.toNext = fade.alpha_success = fade.alpha_fail = fade.alpha_success = fade.isRender_back = fade.isRender_font = 0;
	fade.isFirstRender = true;
	fade.alpha_black = 255;

	StopCnt = 0;
	warningCnt = 0;

	//초기값 안주면 재시작 시 이상해진다.
	warningArea.x = -3000;
	warningArea.y = -3000;

	playerPt = new playerClass;
	playerPt->init();

	enemyPt = new enemyClass;
	enemyPt->init();

	effectPt = new effectClass;
	effectPt->init();

	objectPt = new objectClass;
	objectPt->init();

	itemPt = new itemClass;
	itemPt->init();

	playerPt->setCameraScale(1000, 700);
	//enemyPt->generateEnemy(549, 638, E_TURRET, E_IDLE, false); //10층

	objectPt->generateObject(360, 644, 600,O_DOOR);
	objectPt->generateObject(1062, 644, 1000, O_DOOR);
	objectPt->generateObject(2608, 388, 600, O_DOOR);
	objectPt->generateObject(3344, 388, 600, O_DOOR);
	objectPt->generateObject(3344, 644, 1200, O_DOOR);
	
	//1층
	itemPt->generateItem(309, 738, I_KNIFE);
	enemyPt->generateEnemy(1559, 738, E_BOLD, E_IDLE, true); 
	enemyPt->generateEnemy(1459, 738, E_TURRET, E_IDLE, false); 
	enemyPt->generateEnemy(1259, 738, E_COP, E_IDLE, false); 
	enemyPt->generateEnemy(1159,738, E_POMP, E_WALK, true); 
	enemyPt->generateEnemy(479, 738, E_GRUNT, E_IDLE, true); 
	
	//2층
	enemyPt->generateEnemy(1872, 443, 240, E_POMP, E_WALK, true); 
	enemyPt->generateEnemy(1972, 443, 240, E_COP, E_WALK, false);
	enemyPt->generateEnemy(2072, 443, 240, E_BOLD, E_WALK, true);
	enemyPt->generateEnemy(2172, 443, 240, E_COP, E_WALK,false);
	
	//2층 문 위
	enemyPt->generateEnemy(2800, 443, E_BOLD, E_IDLE,false);
	
	//2층 문 
	enemyPt->generateEnemy(3200, 681, E_COP, E_WALK, false);

	//2층 두번째 문 위
	enemyPt->generateEnemy(3681, 424, E_TURRET, E_IDLE, false);
	enemyPt->generateEnemy(3439, 424, E_POMP, E_IDLE,true);

	//2층 두번째 문 아래
	enemyPt->generateEnemy(3439, 681, E_COP, E_IDLE, false);
	enemyPt->generateEnemy(3639,681, E_COP, E_WALK, true);
	enemyPt->generateEnemy(3839, 681, E_COP, E_WALK, false);


	return S_OK;
}

void stage2::release()
{
	playerPt->release();
	SAFE_DELETE(playerPt);

	enemyPt->release();
	SAFE_DELETE(enemyPt);

	effectPt->release();
	SAFE_DELETE(effectPt);

	objectPt->release();
	SAFE_DELETE(objectPt);

	itemPt->release();
	SAFE_DELETE(itemPt);
}

void stage2::update()
{
	playerPt->update();
	enemyPt->update();
	effectPt->update();
	objectPt->update();
	itemPt->update();

	if (INPUT->GetKeyDown(VK_TAB))
	{
		if (SOUNDMANAGER->isPlaySound("slow_out") == false)SOUNDMANAGER->play("slow_out");
		this->release();
		this->init();
	}

	if (playerPt->getDead() == true && playerPt->getHurt() == false && playerPt->getPlayer().regen == true)
	{
		DATA->setDead(true);
		fade.isRender_font = true;
		if (INPUT->GetKeyDown(VK_RBUTTON))
		{
			if (SOUNDMANAGER->isPlaySound("slow_out") == false)SOUNDMANAGER->play("slow_out");
			this->release();
			this->init();
		}
	}

	if (DATA->getStop() == true)
	{
		StopCnt++;
		if (StopCnt > 12)
		{
			StopCnt = 0;
			DATA->setStop(false);
		}
	}

	//죽은 적 주변 플레이어 인식 범위 렉트
	warningArea.rc = RectMakeCenter(warningArea.x, warningArea.y, 400, 10);

	if (DATA->getSlow() == true || DATA->getStop() == true)
	{
		DATA->setBgRenImg(IMAGEMANAGER->findImage("stage2_bg_render_slow"));
	}
	else if ((playerPt->getPlayer().state == P_HURTFLY || playerPt->getPlayer().state == P_HURTGROUND))
	{
		DATA->setBgRenImg(IMAGEMANAGER->findImage("stage2_bg_render_slow"));
	}
	else
	{
		DATA->setBgRenImg(IMAGEMANAGER->findImage("stage2_bg_render"));
	}

	/*아이템 충돌*/
	for (int i = 0; i < itemPt->getVectorItem().size(); i++)
	{
		RECT tempRECT;
		if (IntersectRect(&tempRECT, &playerPt->getPlayerRect(), &itemPt->getItemRect(i)) && itemPt->getItem(i).isPick == false)
		{
			//충돌시 화살표 렌더
			itemPt->setArrowRender(i, true);
			//아이템 줍기
			if (INPUT->GetKeyDown(VK_RBUTTON))
			{
				itemPt->setItemPick(i, true);
				playerPt->setPickedItem(i);
				playerPt->setItemIsPick(true);
			}
		}
		else
		{
			itemPt->setArrowRender(i, false);
		}

	}

	if (playerPt->getPlayer().isPick == true)
	{
		//아이템 던지기
		if (INPUT->GetKeyDown(VK_RBUTTON))
		{
			itemPt->setItemThrow(playerPt->getPlayer().pickedItemIdx, playerPt->getPlayer().toMouseAngle, true);
			playerPt->setItemIsPick(false);
		}

		itemPt->setItemLocation(playerPt->getPlayer().pickedItemIdx, playerPt->getPlayer().centerX + DATA->getBgCol().x, playerPt->getPlayer().centerY + DATA->getBgCol().y);
	}
	//end of 아이템충돌

	/*오브젝트 충돌*/
	for (int i = 0; i < objectPt->getVectorObject().size(); i++)
	{
		RECT tempRECT99;
		
		if (IntersectRect(&tempRECT99, &playerPt->getPlayerRightRect(), &objectPt->getObjectRect(i)) && objectPt->getObject(i).isBreak== false)
		{
			playerPt->setDoorTouchedRight(true);
			if (INPUT->GetKeyDown('E') || playerPt->getPlayer().state == P_ATTACK)
			{
				objectPt->setDoorBreak(i);
				objectPt->setDoorWidth(i,120);
				playerPt->setPlayerState(P_DOORBREAK);
				playerPt->setDoorTouchedRight(false);
				playerPt->setFrameIdxZero();
			}
		}
	
		if (IntersectRect(&tempRECT99, &playerPt->getPlayerLeftRect(), &objectPt->getObjectRect(i)) && objectPt->getObject(i).isBreak == false)
		{
			playerPt->setDoorTouchedLeft(true);
			if (INPUT->GetKeyDown('E') || playerPt->getPlayer().state == P_ATTACK)
			{
				objectPt->setDoorBreak(i,false);
				objectPt->setDoorWidth(i, 120);
				playerPt->setPlayerState(P_DOORBREAK);
				playerPt->setDoorTouchedLeft(false);
				playerPt->setFrameIdxZero();
			}
		}
	

	}
	//end of 오브젝트 충돌

	/*에너미 충돌*/
	for (int i = 0; i < enemyPt->getVectorEnemy().size(); i++)
	{
		enemyPt->setDistance(i, getDistance(playerPt->getPlayer().centerX, playerPt->getPlayer().centerY, enemyPt->getEnemy(i).centerX, enemyPt->getEnemy(i).centerY) + DATA->getBgCol().x);
		enemyPt->setAngle(i, getAngle(playerPt->getPlayer().centerX, playerPt->getPlayer().centerY, enemyPt->getEnemy(i).centerX, enemyPt->getEnemy(i).centerY));
		playerPt->setPlayerToEnemyAngle(getAngle(playerPt->getPlayer().centerX, playerPt->getPlayer().centerY, enemyPt->getEnemy(i).centerX, enemyPt->getEnemy(i).centerY));

		float tempDistanceY = playerPt->getPlayer().centerY - enemyPt->getEnemy(i).centerY;
		enemyPt->setDistanceY(i, tempDistanceY);

		RECT tempRECT;
		RECT tempRECT2;
		RECT tempRECT3;
		RECT tempRECT4;
		RECT tempRECT5;
		RECT tempRECT6;

		/*아이템 충돌*/
		for (int j = 0; j < itemPt->getVectorItem().size(); j++)
		{
			RECT tempRECT1;
			if (IntersectRect(&tempRECT1, &enemyPt->getEnemyRect(i), &itemPt->getItemRect(j)) && itemPt->getItem(j).isThrow == true)
			{
				if (enemyPt->getEnemy(i).state != E_HURT)
				{
					effectPt->hitLazerRender(enemyPt->getEnemy(i).x, enemyPt->getEnemy(i).y, playerPt->getPlayer().toMouseAngle); //레이저 이펙트
					itemPt->setItemThrowFalse(j);
					enemyPt->setEnemyReflectDead(i);
					enemyPt->setIsPlayerRight(i, playerPt->getPlayer().isRight);
					enemyPt->setEnemyHurt(i);
					DATA->setStop(true);
					warningArea.x = enemyPt->getEnemyRect(i).left;
					warningArea.y = enemyPt->getEnemyRect(i).top;
					break;
				}
			}
		}
		//end of 아이템 충돌

		/*오브젝트 충돌*/
		for (int j = 0; j < objectPt->getVectorObject().size(); j++)
		{
			RECT tempRECT88;
			RECT tempRECT87;
			if (IntersectRect(&tempRECT88, &enemyPt->getEnemyRect(i), &objectPt->getObjectRect(j)) && objectPt->getObject(j).isBreak == true && playerPt->getPlayer().state == P_DOORBREAK)
			{
				enemyPt->setEnemyHurt(i,true);
			}
			
			if (IntersectRect(&tempRECT88, &enemyPt->getEnemyRect(i), &objectPt->getObjectRect(j)) && objectPt->getObject(j).isBreak == false)
			{
				if (enemyPt->getEnemy(i).isRight)
				{
					enemyPt->setEnemyDirection(i,false);
				}
				else
				{
					enemyPt->setEnemyDirection(i, true);
				}
			}

			//문 너머에 있는 플레이어는 감지하지 못하게 함.
			if (IntersectRect(&tempRECT87, &enemyPt->getEnemyRect(i), &objectPt->getObjectFindRect(j)))
			{
				if (objectPt->getObject(j).isPlayerReachedFindArea == false)
				{
					enemyPt->setDoorReached(i, true);
				}
				else
				{
					enemyPt->setDoorReached(i, false);
				}
			}
			

			RECT tempRECT34;
			if (IntersectRect(&tempRECT34, &playerPt->getPlayerRect(), &objectPt->getObjectFindRect(j)))
			{
				objectPt->setDoorReachedFindArea(j);
			}


			if (IntersectRect(&tempRECT87, &enemyPt->getEnemyRect(i), &objectPt->getObjectFindRect(j)) && objectPt->getObject(j).isBreak == true && playerPt->getPlayer().state!=P_DOORBREAK && enemyPt->getEnemy(i).state !=E_HURT)
			{
				enemyPt->setEnemyDirection(i, false);
			}
			
		}
		//end of 오브젝트 충돌

		////플레이어 공격 충돌 처리
		if (IntersectRect(&tempRECT, &playerPt->getAttRect(), &enemyPt->getEnemyRect(i)) && playerPt->getPlayer().isAttack == true && playerPt->getPlayer().state == P_ATTACK && DATA->getDead() == false)
		{
			if (enemyPt->getEnemy(i).state != E_HURT)
			{
				effectPt->hitLazerRender(enemyPt->getEnemy(i).x, enemyPt->getEnemy(i).y, playerPt->getPlayer().toMouseAngle); //레이저 이펙트
				enemyPt->setIsPlayerRight(i, playerPt->getPlayer().isRight);
				playerPt->randomSound("death_sword1", "death_sword2");
				enemyPt->setEnemyHurt(i);
				DATA->setStop(true);
				warningArea.x = enemyPt->getEnemyRect(i).left;
				warningArea.y = enemyPt->getEnemyRect(i).top;
				break;
			}
		}

		//에너미 공격 충돌 처리
		if (IntersectRect(&tempRECT, &playerPt->getPlayerRect(), &enemyPt->getEnemyAttackRect(i)) && enemyPt->getEnemy(i).isAttack == true && playerPt->getPlayer().state != P_ROLL)
		{
			playerPt->setHurt(true);
			enemyPt->setEnemyFounded(i, false);
		}

		if (enemyPt->getEnemy(i).state != E_HURT && DATA->getDead() == false && enemyPt->getEnemy(i).isDoorReached==false)
		{
			if (IntersectRect(&tempRECT2, &playerPt->getPlayerRect(), &enemyPt->getDetectRect(i)) && enemyPt->getEnemy(i).state != E_HURT)
			{
				enemyPt->setEnemyFounded(i, true);
			}
			//플레이어를 찾을 경우 방향 설정
			if (enemyPt->getEnemy(i).founded == true && enemyPt->getEnemy(i).category != E_TURRET)
			{
				if (enemyPt->getEnemy(i).centerX > playerPt->getPlayer().centerX)
				{
					enemyPt->setEnemyDirection(i, false);
				}
				else
				{
					enemyPt->setEnemyDirection(i, true);
				}
			}
		}

		RECT tempRect7;

		//죽은 적 주변 인식
		if (IntersectRect(&tempRect7, &warningArea.rc, &enemyPt->getEnemyRect(i)) && enemyPt->getEnemy(i).state != E_HURT && DATA->getDead() == false && enemyPt->getEnemy(i).isDoorReached == false)
		{
			enemyPt->setEnemyFounded(i, true);
			//플레이어를 찾을 경우 방향 설정
			if (enemyPt->getEnemy(i).founded == true && enemyPt->getEnemy(i).category != E_TURRET)
			{
				if (enemyPt->getEnemy(i).centerX > playerPt->getPlayer().centerX)
				{
					enemyPt->setEnemyDirection(i, false);
				}
				else
				{
					enemyPt->setEnemyDirection(i, true);
				}
			}
		}
		
		//총알 플레이어 맞을 떄
		for (int j = 0; j < 5; j++)
		{
			if (IntersectRect(&tempRECT3, &playerPt->getPlayerRect(), &enemyPt->getEnemyBulletRect(i, j)) && playerPt->getHurt() == false && playerPt->getPlayer().state != P_HURTGROUND && enemyPt->getEnemy(i).bullet[j].fire == true)
			{
				if (playerPt->getPlayer().state == P_ROLL)continue;
				if (enemyPt->getEnemy(i).isRight == true)
				{
					playerPt->setPlayerRight(false);
				}
				else
				{
					playerPt->setPlayerRight(true);
				}
				playerPt->setHurt(true);
				enemyPt->setEnemyBulletFire(i, j, false);
				break;
			}
			//총알 반사
			if (IntersectRect(&tempRECT4, &playerPt->getAttRect(), &enemyPt->getEnemyBulletRect(i, j)) && playerPt->getPlayer().isAttack == true && enemyPt->getEnemy(i).bullet[j].isReflected == false && playerPt->getPlayer().state == P_ATTACK)
			{
				SOUNDMANAGER->play("p_reflect");
				enemyPt->setEnemyBulletReflect(i, j, true, playerPt->getPlayer().toMouseAngle);
				enemyPt->setIsPlayerRight(i, playerPt->getPlayer().isRight);
				effectPt->reflectRender(enemyPt->getEnemy(i).bullet[j].x, enemyPt->getEnemy(i).bullet[j].y, playerPt->getPlayer().isRight);
				continue;
			}
		}
	}

	if (playerPt->getPlayer().clear == true)
	{
		if (fade.isRender_back == false)SOUNDMANAGER->play("clear");
		fade.isRender_back = true;
	}

}

void stage2::render()
{
	DATA->getBgCol().img->render(getMemDC(), -1 * DATA->getBgCol().x, -1 * DATA->getBgCol().y);
	DATA->getBgRen().img->render(getMemDC(), -1 * DATA->getBgCol().x, -1 * DATA->getBgCol().y);

	//Rectangle(getMemDC(), warningArea.rc);

	objectPt->render();
	enemyPt->render();
	playerPt->render();

	DATA->getHud()->render(getMemDC(), 0, 0);
	effectPt->render();
	itemPt->render();

	this->sceneChanger();
}

void stage2::toNextStage()
{
	SOUNDMANAGER->play("start");
	DATA->setClear(STAGE2_CLEARED);
	SCENEMANAGER->loadScene("스테이지3");
	this->saveClearedStage();
}

void stage2::saveClearedStage()
{
	HANDLE file;
	DWORD write;

	enum_Cleared tempStage[1];

	tempStage[0] = DATA->getCurrentClear();

	file = CreateFile("save.stage", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, tempStage, sizeof(enum_Cleared) * 1, &write, NULL);
	CloseHandle(file);
}

void stage2::sceneChanger()
{
	if (fade.isFirstRender)
	{
		if (fade.alpha_black >= 3)
		{
			DATA->setRenderUi(false);
			fade.alpha_black -= 3;
		}
		else
		{
			DATA->setRenderUi(true);
			fade.isFirstRender = false;
		}
		fade.blackImg->alphaRender(getMemDC(), 0, 0, fade.alpha_black);
	}
	if (fade.isRender_back)
	{
		if (fade.alpha_black <= 252)
		{
			fade.alpha_black += 3;
		}
		else
		{
			fade.isRender_font = true;
		}
		fade.blackImg->alphaRender(getMemDC(), 0, 0, fade.alpha_black);
	}

	if (fade.isRender_font)
	{
		if (fade.alpha_success <= 200)
		{
			fade.alpha_success += 3;
		}
		else
		{
			fade.cnt++;
		}
		if (DATA->getDead() == true)
		{
			fade.failImg->alphaRender(getMemDC(), 0, -100, fade.alpha_success);
		}
		else
		{
			DATA->setRenderUi(false);
			if (fade.cnt > 120)
			{
				this->toNextStage();
			}
			fade.successImg->alphaRender(getMemDC(), 0, 0, fade.alpha_success);
		}
	}
}







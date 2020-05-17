#include "stdafx.h"
#include "stage1.h"

HRESULT stage1::init()
{
	DATA->setBgColImg(IMAGEMANAGER->findImage("stage1_bg_collision"));
	DATA->setBgRenImg(IMAGEMANAGER->findImage("stage1_bg_render"));
	DATA->setHud(IMAGEMANAGER->findImage("hud_collapse"));
	DATA->stageInitialize(2176,3500);
	DATA->setStartLoc(350, 100);

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

	enemyPt->generateEnemy(1165, 608, E_BOLD,E_IDLE,false); //10층
	enemyPt->generateEnemy(950, 608, E_GRUNT,E_IDLE,true); //10층
	enemyPt->generateEnemy(880, 758, E_POMP,E_WALK,true); //9층
	enemyPt->generateEnemy(650, 758, E_COP,E_IDLE,true); //9층
	enemyPt->generateEnemy(1165, 900, E_GRUNT, E_IDLE, true); //8층
	enemyPt->generateEnemy(1000, 900, E_POMP, E_WALK, true); //8층
	enemyPt->generateEnemy(880, 1223, E_BOLD, E_IDLE, true); //7층
	enemyPt->generateEnemy(650, 1223, E_POMP, E_WALK, true); //7층
	enemyPt->generateEnemy(1154, 1456, E_COP, E_IDLE, false); //6층
	enemyPt->generateEnemy(1000, 1456, E_COP, E_WALK, true); //6층
	enemyPt->generateEnemy(670, 1676, E_GRUNT, E_IDLE, true); //5층
	enemyPt->generateEnemy(880, 1676, E_POMP, E_WALK, true); //5층
	enemyPt->generateEnemy(1154, 1896, E_BOLD, E_IDLE, true); //4층
	enemyPt->generateEnemy(1000, 1896, E_BOLD, E_WALK, true); //4층
	enemyPt->generateEnemy(670, 2106, E_GRUNT, E_IDLE, true); //3층
	enemyPt->generateEnemy(880, 2106, E_POMP, E_WALK, true); //3층

	objectPt->generateObject(1552, 2816,0, O_DOOR);

	itemPt->generateItem(541, 360, I_BUST);

	return S_OK;
}

void stage1::release()
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

void stage1::update()
{
	playerPt->update();
	enemyPt->update();
	effectPt->update();
	objectPt->update();
	itemPt->update();

	if (INPUT->GetKeyDown(VK_TAB))
	{
		if(SOUNDMANAGER->isPlaySound("slow_out")==false)SOUNDMANAGER->play("slow_out");
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

	if (DATA->getSlow() == true || DATA->getStop() == true)
	{
		DATA->setBgRenImg(IMAGEMANAGER->findImage("stage1_bg_render_slow"));
	}
	else if ((playerPt->getPlayer().state == P_HURTFLY || playerPt->getPlayer().state == P_HURTGROUND))
	{
		DATA->setBgRenImg(IMAGEMANAGER->findImage("stage1_bg_render_slow"));
	}
	else
	{
		DATA->setBgRenImg(IMAGEMANAGER->findImage("stage1_bg_render"));
	}

	//죽은 적 주변 플레이어 인식 범위 렉트
	warningArea.rc = RectMakeCenter(warningArea.x, warningArea.y, 600, 10);

	/*아이템 충돌*/
	for (int i = 0; i < itemPt->getVectorItem().size(); i++)
	{
		RECT tempRECT;
		if (IntersectRect(&tempRECT, &playerPt->getPlayerRect(), &itemPt->getItemRect(i)) && itemPt->getItem(i).isPick==false)
		{
			//충돌시 화살표 렌더
			itemPt->setArrowRender(i, true);
			//아이템 줍기
			if (INPUT->GetKeyDown(VK_RBUTTON))
			{
				itemPt->setItemPick(i,true);
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
			itemPt->setItemThrow(playerPt->getPlayer().pickedItemIdx, playerPt->getPlayer().toMouseAngle,true);
			playerPt->setItemIsPick(false);
		}

		itemPt->setItemLocation(playerPt->getPlayer().pickedItemIdx, playerPt->getPlayer().centerX + DATA->getBgCol().x, playerPt->getPlayer().centerY + DATA->getBgCol().y);
	}
	//end of 아이템충돌

	/*오브젝트 충돌*/
	for (int i = 0; i < objectPt->getVectorObject().size(); i++)
	{
		RECT tempRECT99;

		if (IntersectRect(&tempRECT99, &playerPt->getPlayerRightRect(), &objectPt->getObjectRect(i)) && objectPt->getObject(i).isBreak == false)
		{
			playerPt->setDoorTouchedRight(true);
			if (INPUT->GetKeyDown('E') || playerPt->getPlayer().state == P_ATTACK)
			{
				objectPt->setDoorBreak(i);
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
				objectPt->setDoorBreak(i, false);
				playerPt->setPlayerState(P_DOORBREAK);
				playerPt->setDoorTouchedLeft(false);
				playerPt->setFrameIdxZero();
			}
		}
	}
		
	/*에너미 충돌처리*/
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
			if (IntersectRect(&tempRECT1, &enemyPt->getEnemyRect(i), &itemPt->getItemRect(j)) && itemPt->getItem(j).isThrow==true)
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

		//플레이어 공격 충돌 처리
		if (IntersectRect(&tempRECT,&playerPt->getAttRect(), &enemyPt->getEnemyRect(i)) && playerPt->getPlayer().isAttack == true && playerPt->getPlayer().state==P_ATTACK && DATA->getDead()==false)
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

		//에너미 플레이어 인식 + 공격
		if (enemyPt->getEnemy(i).state != E_HURT && DATA->getDead()==false)
		{
			if (IntersectRect(&tempRECT2, &playerPt->getPlayerRect(), &enemyPt->getDetectRect(i)) && enemyPt->getEnemy(i).state != E_HURT)
			{
				enemyPt->setEnemyFounded(i, true);
			}
			if (enemyPt->getEnemy(i).founded == true)
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
			if (IntersectRect(&tempRECT3, &playerPt->getPlayerRect(), &enemyPt->getEnemyBulletRect(i,j))&&playerPt->getHurt()==false && playerPt->getPlayer().state!=P_HURTGROUND && enemyPt->getEnemy(i).bullet[j].fire == true)
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
				enemyPt->setEnemyBulletFire(i,j, false);
				break;
			}
			//총알 반사
			if (IntersectRect(&tempRECT4, &playerPt->getAttRect(), &enemyPt->getEnemyBulletRect(i, j)) && playerPt->getPlayer().isAttack == true && enemyPt->getEnemy(i).bullet[j].isReflected==false && playerPt->getPlayer().state == P_ATTACK)
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

void stage1::render()
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

	//피격판정 나타내기
	if (DATA->getRenderUi())
	{
		char str[256];
		SetBkMode(getMemDC(), TRANSPARENT);
		SetTextColor(getMemDC(), RGB(255, 0, 255));
		playerPt->getPlayer().hurtSwitch ? sprintf(str, "피격판정 : ON") : sprintf(str, "피격판정 : OFF");
		TextOut(getMemDC(), 312, 8, str, strlen(str));
		if(playerPt->getPlayer().switchFloating)TextOut(getMemDC(), 620, 350, str, strlen(str));
	}

	this->sceneChanger();
}

void stage1::toNextStage()
{
	SOUNDMANAGER->play("start");
	DATA->setClear(STAGE1_CLEARED);
	SCENEMANAGER->loadScene("스테이지2");
	this->saveClearedStage();
}

void stage1::saveClearedStage()
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

void stage1::sceneChanger()
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






#include "stdafx.h"
#include "stage3.h"

HRESULT stage3::init()
{
	DATA->setBgColImg(IMAGEMANAGER->findImage("stage3_bg_collision"));
	DATA->setBgRenImg(IMAGEMANAGER->findImage("stage3_bg_render"));
	DATA->setBgRenImg(IMAGEMANAGER->findImage("stage3_bg_render_slow"));
	DATA->setHud(IMAGEMANAGER->findImage("hud_collapse"));
	DATA->stageInitialize(0, 525, 4000, 1492);
	DATA->setStartLoc(50, 895);

	StopCnt = 0;
	warningCnt = 0;

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


	//enemyPt->generateEnemy(549, 638, E_TURRET, E_IDLE, false); //10층
	//시작위치
	itemPt->generateItem(331, 897, I_KNIFE);

	//중앙층
	objectPt->generateObject(400, 800, 1200, O_DOOR);
	enemyPt->generateEnemy(510, 887, E_COP, E_WALK, false);
	enemyPt->generateEnemy(1117, 887, E_COP, E_WALK, true);
	enemyPt->generateEnemy(2653, 887, E_GRUNT, E_WALK, true);
	enemyPt->generateEnemy(2962, 887, E_COP, E_IDLE, false);
	enemyPt->generateEnemy(3700, 887, E_BOLD, E_WALK, false);

	//2층
	objectPt->generateObject(168, 544, 542, O_DOOR, false);
	enemyPt->generateEnemy(460, 630, E_COP, E_WALK, false); // 바로 위
	objectPt->generateObject(720, 544, 990, O_DOOR);
	enemyPt->generateEnemy(800, 630, E_BOLD, E_WALK, false); // 바로 위
	enemyPt->generateEnemy(1100, 630, E_POMP, E_WALK, true); // 바로 위
	enemyPt->generateEnemy(1200, 630, E_BOLD, E_WALK, false); // 바로 위
	objectPt->generateObject(1712, 544, 414, O_DOOR);
	enemyPt->generateEnemy(1802, 630, E_COP, E_IDLE, false); // 바로 위
	enemyPt->generateEnemy(1900, 630, E_BOLD, E_WALK, true); // 바로 위
	objectPt->generateObject(2128, 544, 739, O_DOOR);
	enemyPt->generateEnemy(2328, 630, E_BOLD, E_WALK, false); // 바로 위
	objectPt->generateObject(2864, 544, 1130, O_DOOR);
	enemyPt->generateEnemy(3000, 630, E_POMP, E_WALK, true); // 바로 위
	enemyPt->generateEnemy(3428, 630, E_POMP, E_WALK, false); // 바로 위

	itemPt->generateItem(2946, 643, I_BOTTLE);

	//3층
	enemyPt->generateEnemy(3188, 362, E_COP, E_WALK, true); // 바로 위
	enemyPt->generateEnemy(3441, 362, E_COP, E_WALK, false); // 바로 위
	
	//지하
	enemyPt->generateEnemy(1417, 1172, E_COP, E_IDLE, false); // 바로 위
	objectPt->generateObject(1552, 1088, 1200, O_DOOR);
	objectPt->generateObject(2448, 1088, 1200, O_DOOR);
	enemyPt->generateEnemy(2560, 1172, E_COP, E_IDLE, true); // 바로 위
	enemyPt->generateEnemy(3310, 1172, E_COP, E_IDLE, false); // 바로 위

	//1층
	objectPt->generateObject(3088, 800, 1200, O_DOOR);


	return S_OK;
}

void stage3::release()
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

void stage3::update()
{
	playerPt->update();
	enemyPt->update();
	effectPt->update();
	objectPt->update();
	itemPt->update();

	if (INPUT->GetKeyDown(VK_TAB))
	{
		this->release();
		this->init();
	}


	if (playerPt->getDead() == true && playerPt->getHurt() == false && playerPt->getPlayer().regen == true)
	{
		DATA->setDead(true);
		if (INPUT->GetKeyDown(VK_RBUTTON))
		{
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
	warningArea.rc = RectMakeCenter(warningArea.x - DATA->getBgCol().x, warningArea.y - DATA->getBgCol().y, 300, 100);

	if (DATA->getSlow() == true || DATA->getStop() == true)
	{
		DATA->setBgRenImg(IMAGEMANAGER->findImage("stage3_bg_render_slow"));
	}
	else if ((playerPt->getPlayer().state == P_HURTFLY || playerPt->getPlayer().state == P_HURTGROUND))
	{
		DATA->setBgRenImg(IMAGEMANAGER->findImage("stage3_bg_render_slow"));
	}
	else
	{
		DATA->setBgRenImg(IMAGEMANAGER->findImage("stage3_bg_render"));
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
		if (playerPt->getPlayer().state == P_DOORBREAK)
		{
			if (objectPt->getObject(i).isBreak == true)
			{
				objectPt->setDoorWidth(i, 120);
			}
		}
		else
		{
			objectPt->setDoorWidth(i, 30);
		}


		RECT tempRECT99;

		if (IntersectRect(&tempRECT99, &playerPt->getPlayerRect(), &objectPt->getObjectRect(i)) && objectPt->getObject(i).isBreak == false)
		{
			if (playerPt->getPlayer().isRight)
			{
				playerPt->setDoorTouchedRight(true);
			}
			else
			{
				playerPt->setDoorTouchedLeft(false);
				playerPt->setDoorTouchedRight(false);
			}

			if (INPUT->GetKeyDown('E') || playerPt->getPlayer().state == P_ATTACK)
			{
				if (playerPt->getPlayer().isRight)
				{
					objectPt->setDoorBreak(i);
					playerPt->setPlayerState(P_DOORBREAK);
					playerPt->setDoorTouchedRight(false);
					playerPt->setFrameIdxZero();
				}
				else
				{
					objectPt->setDoorDirection(i, false);
					objectPt->setDoorFrameIdx(i, 19);
					objectPt->setDoorBreak(i);
					playerPt->setPlayerState(P_DOORBREAK);
					playerPt->setDoorTouchedLeft(false);
					playerPt->setFrameIdxZero();
				}

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
					warningArea.x = enemyPt->getEnemy(i).x;
					warningArea.y = enemyPt->getEnemy(i).y;
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
				enemyPt->setEnemyHurt(i, true);
			}

			if (IntersectRect(&tempRECT88, &enemyPt->getEnemyRect(i), &objectPt->getObjectRect(j)) && objectPt->getObject(j).isBreak == false)
			{
				if (enemyPt->getEnemy(i).isRight)
				{
					enemyPt->setEnemyDirection(i, false);
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


			if (IntersectRect(&tempRECT87, &enemyPt->getEnemyRect(i), &objectPt->getObjectFindRect(j)) && objectPt->getObject(j).isBreak == true && playerPt->getPlayer().state != P_DOORBREAK && enemyPt->getEnemy(i).state != E_HURT)
			{
				enemyPt->setEnemyDirection(i, false);
			}

		}
		//end of 오브젝트 충돌

		////플레이어 공격 충돌 처리
		if (IntersectRect(&tempRECT, &playerPt->getAttRect(), &enemyPt->getEnemyRect(i)) && playerPt->getPlayer().isAttack == true && playerPt->getPlayer().state == P_ATTACK)
		{
			if (enemyPt->getEnemy(i).state != E_HURT)
			{
				effectPt->hitLazerRender(enemyPt->getEnemy(i).x, enemyPt->getEnemy(i).y, playerPt->getPlayer().toMouseAngle); //레이저 이펙트
				enemyPt->setIsPlayerRight(i, playerPt->getPlayer().isRight);
				enemyPt->setEnemyHurt(i);
				DATA->setStop(true);
				warningArea.x = enemyPt->getEnemy(i).x;
				warningArea.y = enemyPt->getEnemy(i).y;
				break;
			}
		}

		//에너미 공격 충돌 처리
		if (IntersectRect(&tempRECT, &playerPt->getPlayerRect(), &enemyPt->getEnemyAttackRect(i)) && enemyPt->getEnemy(i).isAttack == true && playerPt->getPlayer().state != P_ROLL)
		{
			playerPt->setHurt(true);
		}

		if (enemyPt->getEnemy(i).state != E_HURT && DATA->getDead() == false && enemyPt->getEnemy(i).isDoorReached == false)
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
			if (IntersectRect(&tempRECT3, &playerPt->getPlayerRect(), &enemyPt->getEnemyBulletRect(i, j)) && playerPt->getHurt() == false && playerPt->getPlayer().state != P_HURTGROUND)
			{
				if (playerPt->getPlayer().state == P_ROLL)continue;
				//if (enemyPt->getEnemy(i).bullet[j].fire == false)continue;
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
				enemyPt->setEnemyBulletReflect(i, j, true, playerPt->getPlayer().toMouseAngle);
				enemyPt->setIsPlayerRight(i, playerPt->getPlayer().isRight);
				effectPt->reflectRender(enemyPt->getEnemy(i).bullet[j].x, enemyPt->getEnemy(i).bullet[j].y, playerPt->getPlayer().isRight);
				continue;
			}
		}
	}

	if (playerPt->getPlayer().clear == true)
	{
		toNextStage();
	}

}

void stage3::render()
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
}

void stage3::toNextStage()
{
	DATA->setClear(STAGE3_CLEARED);
	SCENEMANAGER->loadScene("스테이지4");
	this->saveClearedStage();
}

void stage3::saveClearedStage()
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
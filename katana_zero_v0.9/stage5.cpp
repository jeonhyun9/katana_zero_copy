#include "stdafx.h"
#include "stage5.h"

HRESULT stage5::init()
{
	DATA->setBgColImg(IMAGEMANAGER->findImage("stage5_bg_collision"));
	DATA->setBgRenImg(IMAGEMANAGER->findImage("stage5_bg_render"));
	DATA->setBgRenImg(IMAGEMANAGER->findImage("stage5_bg_render_slow"));
	DATA->setHud(IMAGEMANAGER->findImage("hud_collapse"));
	DATA->stageInitialize(0, 0, 1344, 784);
	DATA->setStartLoc(250, 50);

	StopCnt = 0;
	warningCnt = 0;

	enemyIdx = -1;
	stageCnt = 0;

	playerPt = new playerClass;
	playerPt->init();
	playerPt->setCameraScale(1100, 400);

	enemyPt = new enemyClass;
	enemyPt->init();

	bossPt = new bossClass;
	bossPt->init();

	effectPt = new effectClass;
	effectPt->init();

	objectPt = new objectClass;
	objectPt->init();

	itemPt = new itemClass;
	itemPt->init();


	//bossPt->generateBoss(470, 330, true, B_IDLE);
	//�뽬
	bossPt->generateBoss(270, 530, true, B_DASH_PRE);
	//bossPt->generateBoss(1120, 530, false, B_DASH_PRE);

	//����+�Ѿ˹߻�
	//bossPt->generateBoss(270, 530,true, B_PREJUMP);
	//bossPt->generateBoss(970, 530,false, B_PREJUMP);

	//��+��ź�߻�
	//bossPt->generateBoss(1120, 530, false, B_DRAWGUN);
	//bossPt->generateBoss(270, 530, true, B_DRAWGUN);

	//����
	//bossPt->generateBoss(500, 530, true, B_TELEPORT);

	return S_OK;
}

void stage5::release()
{
	playerPt->release();
	SAFE_DELETE(playerPt);

	enemyPt->release();
	SAFE_DELETE(enemyPt);

	bossPt->release();
	SAFE_DELETE(bossPt);

	effectPt->release();
	SAFE_DELETE(effectPt);

	objectPt->release();
	SAFE_DELETE(objectPt);

	itemPt->release();
	SAFE_DELETE(itemPt);

}

void stage5::update()
{
	playerPt->update();
	enemyPt->update();
	bossPt->update();
	effectPt->update();
	objectPt->update();
	itemPt->update();

	this->bossHit();
	this->stagePattern5();

	//�����
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

	//���� ȿ��
	if (DATA->getStop() == true)
	{
		StopCnt++;
		if (StopCnt > 12)
		{
			StopCnt = 0;
			DATA->setStop(false);
		}
	}

	//���� �� �ֺ� �÷��̾� �ν� ���� ��Ʈ
	warningArea.rc = RectMakeCenter(warningArea.x - DATA->getBgCol().x, warningArea.y - DATA->getBgCol().y, 300, 100);

	//���ο� ��� ó��
	if (DATA->getSlow() == true || DATA->getStop() == true)
	{
		DATA->setBgRenImg(IMAGEMANAGER->findImage("stage5_bg_render_slow"));
	}
	else if ((playerPt->getPlayer().state == P_HURTFLY || playerPt->getPlayer().state == P_HURTGROUND))
	{
		DATA->setBgRenImg(IMAGEMANAGER->findImage("stage5_bg_render_slow"));
	}
	else if (bossPt->getBoss().currentPattern >= 12)
	{
		playerPt->setJump();
		DATA->setBgRenImg(IMAGEMANAGER->findImage("stage5_bg_render_explosion"));
		DATA->setBgColImg(IMAGEMANAGER->findImage("stage5_bg_collision_explosion"));
	}
	else
	{
		DATA->setBgRenImg(IMAGEMANAGER->findImage("stage5_bg_render"));
	}

	/*������ �浹*/
	for (int i = 0; i < itemPt->getVectorItem().size(); i++)
	{
		RECT tempRECT;
		if (IntersectRect(&tempRECT, &playerPt->getPlayerRect(), &itemPt->getItemRect(i)) && itemPt->getItem(i).isPick == false)
		{
			//�浹�� ȭ��ǥ ����
			itemPt->setArrowRender(i, true);
			//������ �ݱ�
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
		//������ ������
		if (INPUT->GetKeyDown(VK_RBUTTON))
		{
			itemPt->setItemThrow(playerPt->getPlayer().pickedItemIdx, playerPt->getPlayer().toMouseAngle, true);
			playerPt->setItemIsPick(false);
		}

		itemPt->setItemLocation(playerPt->getPlayer().pickedItemIdx, playerPt->getPlayer().centerX + DATA->getBgCol().x, playerPt->getPlayer().centerY + DATA->getBgCol().y);
	}
	//end of �������浹

	/*������Ʈ �浹*/
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
	//end of ������Ʈ �浹

	/*���ʹ� �浹*/
	for (int i = 0; i < enemyPt->getVectorEnemy().size(); i++)
	{
		enemyPt->setDistance(i, getDistance(playerPt->getPlayer().centerX, playerPt->getPlayer().centerY, enemyPt->getEnemy(i).centerX, enemyPt->getEnemy(i).centerY) + DATA->getBgCol().x);
		enemyPt->setAngle(i, getAngle(playerPt->getPlayer().centerX, playerPt->getPlayer().centerY, enemyPt->getEnemy(i).centerX, enemyPt->getEnemy(i).centerY));
		playerPt->setPlayerToEnemyAngle(getAngle(playerPt->getPlayer().centerX, playerPt->getPlayer().centerY, enemyPt->getEnemy(i).centerX, enemyPt->getEnemy(i).centerY));

		float tempDistanceY = playerPt->getPlayer().centerY - enemyPt->getEnemy(i).centerY;
		enemyPt->setDistanceY(i, tempDistanceY);

		//���߽� ��ü����
		if (bossPt->getBoss().currentPattern >= 12)
		{
			enemyPt->setEnemyX(i, 1400);
		}

		RECT tempRECT;
		RECT tempRECT2;
		RECT tempRECT3;
		RECT tempRECT4;
		RECT tempRECT5;
		RECT tempRECT6;

		/*������ �浹*/
		for (int j = 0; j < itemPt->getVectorItem().size(); j++)
		{
			RECT tempRECT1;
			if (IntersectRect(&tempRECT1, &enemyPt->getEnemyRect(i), &itemPt->getItemRect(j)) && itemPt->getItem(j).isThrow == true)
			{
				if (enemyPt->getEnemy(i).state != E_HURT)
				{
					effectPt->hitLazerRender(enemyPt->getEnemy(i).x, enemyPt->getEnemy(i).y, playerPt->getPlayer().toMouseAngle); //������ ����Ʈ
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
		//end of ������ �浹

		/*������Ʈ �浹*/
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

			//�� �ʸӿ� �ִ� �÷��̾�� �������� ���ϰ� ��.
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
		//end of ������Ʈ �浹

		////�÷��̾� ���� �浹 ó��
		if (IntersectRect(&tempRECT, &playerPt->getAttRect(), &enemyPt->getEnemyRect(i)) && playerPt->getPlayer().isAttack == true && playerPt->getPlayer().state == P_ATTACK)
		{
			if (enemyPt->getEnemy(i).state != E_HURT)
			{
				effectPt->hitLazerRender(enemyPt->getEnemy(i).x, enemyPt->getEnemy(i).y, playerPt->getPlayer().toMouseAngle); //������ ����Ʈ
				enemyPt->setIsPlayerRight(i, playerPt->getPlayer().isRight);
				enemyPt->setEnemyHurt(i);
				DATA->setStop(true);
				warningArea.x = enemyPt->getEnemy(i).x;
				warningArea.y = enemyPt->getEnemy(i).y;
				break;
			}
		}

		//���ʹ� ���� �浹 ó��
		if (IntersectRect(&tempRECT, &playerPt->getPlayerRect(), &enemyPt->getEnemyAttackRect(i)) && enemyPt->getEnemy(i).isAttack == true && playerPt->getPlayer().state != P_ROLL)
		{
			playerPt->setHurt(true);
		}

		if (enemyPt->getEnemy(i).state != E_HURT && DATA->getDead() == false && enemyPt->getEnemy(i).isDoorReached == false && enemyPt->getEnemy(i).state != E_GEN)
		{
			if (IntersectRect(&tempRECT2, &playerPt->getPlayerRect(), &enemyPt->getDetectRect(i)) && enemyPt->getEnemy(i).state != E_HURT)
			{
				enemyPt->setEnemyFounded(i, true);
			}
			//�÷��̾ ã�� ��� ���� ����
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

		//���� �� �ֺ� �ν�
		if (IntersectRect(&tempRect7, &warningArea.rc, &enemyPt->getEnemyRect(i)) && enemyPt->getEnemy(i).state != E_HURT && DATA->getDead() == false && enemyPt->getEnemy(i).isDoorReached == false)
		{
			enemyPt->setEnemyFounded(i, true);
			//�÷��̾ ã�� ��� ���� ����
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


		for (int j = 0; j < 5; j++)
		{
			//�Ѿ� �÷��̾� ���� ��
			//if (IntersectRect(&tempRECT3, &playerPt->getPlayerRect(), &enemyPt->getEnemyBulletRect(i, j)) && playerPt->getHurt() == false && playerPt->getPlayer().state != P_HURTGROUND)
			//{
			//	if (playerPt->getPlayer().state == P_ROLL)continue;
			//	//if (enemyPt->getEnemy(i).bullet[j].fire == false)continue;
			//	if (enemyPt->getEnemy(i).isRight == true)
			//	{
			//		playerPt->setPlayerRight(false);
			//	}
			//	else
			//	{
			//		playerPt->setPlayerRight(true);
			//	}
			//	playerPt->setHurt(true);
			//	enemyPt->setEnemyBulletFire(i, j, false);
			//	break;
			//}
			//�Ѿ� �ݻ�
			if (IntersectRect(&tempRECT4, &playerPt->getAttRect(), &enemyPt->getEnemyBulletRect(i, j)) && playerPt->getPlayer().isAttack == true && enemyPt->getEnemy(i).bullet[j].isReflected == false && playerPt->getPlayer().state == P_ATTACK)
			{
				enemyPt->setEnemyBulletReflect(i, j, true, playerPt->getPlayer().toMouseAngle);
				enemyPt->setIsPlayerRight(i, playerPt->getPlayer().isRight);
				effectPt->reflectRender(enemyPt->getEnemy(i).bullet[j].x, enemyPt->getEnemy(i).bullet[j].y, playerPt->getPlayer().isRight);
				continue;
			}
		}
	}
	//end of ���ʹ� �浹

	//���� ����������
	if (playerPt->getPlayer().clear == true)
	{
		toNextStage();
	}

}

void stage5::render()
{
	DATA->getBgCol().img->render(getMemDC(), -1 * DATA->getBgCol().x, -1 * DATA->getBgCol().y);
	DATA->getBgRen().img->render(getMemDC(), -1 * DATA->getBgCol().x, -1 * DATA->getBgCol().y);

	//Rectangle(getMemDC(), warningArea.rc);

	objectPt->render();
	enemyPt->render();
	bossPt->render();
	playerPt->render();

	DATA->getHud()->render(getMemDC(), 0, 0);
	effectPt->render();
	itemPt->render();
}

void stage5::toNextStage()
{
	DATA->setClear(STAGE4_CLEARED);
	SCENEMANAGER->loadScene("��������5");
	this->saveClearedStage();
}

void stage5::saveClearedStage()
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

void stage5::bossHit()
{
	RECT tempRC;
	if (IntersectRect(&tempRC, &playerPt->getPlayerRect(), &bossPt->getLazerRect()) && bossPt->getBoss().lazerHitBox == true && DATA->getDead() == false && playerPt->getPlayer().state != P_ROLL )
	{
		playerPt->setHurt(true);
		bossPt->getBoss().isRight ? playerPt->setPlayerRight(false) : playerPt->setPlayerRight(true);
	}

	if (IntersectRect(&tempRC, &playerPt->getAttRect(), &bossPt->getBossRect()) && playerPt->getPlayer().isAttack == true && playerPt->getPlayer().state == P_ATTACK && bossPt->getBoss().state != B_HURTRECOVER && bossPt->getBoss().state != B_OUT && bossPt->getBoss().state != B_DIEFLY && bossPt->getBoss().state != B_DIEGROUND && bossPt->getBoss().state != B_NOHEAD)
	{
		effectPt->hitLazerRender(bossPt->getBoss().x, bossPt->getBoss().y, playerPt->getPlayer().toMouseAngle); //������ ����Ʈ
		playerPt->getPlayer().isRight ? bossPt->setBossHit(false) : bossPt->setBossHit(true);
		DATA->setStop(true);
	}

	if (bossPt->getLazerVector().empty() == false)
	{
		for (int i = 0; i < bossPt->getLazerVector().size(); i++)
		{
			RECT tempRECT;
			if (IntersectRect(&tempRECT, &bossPt->getLazerRect(i), &playerPt->getPlayerRect()) && DATA->getDead() == false && playerPt->getPlayer().state != P_ROLL)
			{
				playerPt->setHurt(true);
			}
		}
	}

	RECT tempR;
	if (IntersectRect(&tempR, &bossPt->getBossRect(), &playerPt->getPlayerRect()) && bossPt->getBoss().state == B_DASH)
	{
		playerPt->setHurt(true);
	}
	
	
}

void stage5::stagePattern5()
{
	//ù��° ����
	bossPt->setPlayerX(playerPt->getPlayer().x);
	
}

void stage5::findPlayer()
{
	enemyPt->setEnemyFounded(enemyIdx - 1, true);
	enemyPt->setEnemyFounded(enemyIdx, true);
	//�÷��̾ ã�� ��� ���� ����
	if (enemyPt->getEnemy(enemyIdx - 1).state == E_HURT
		&& enemyPt->getEnemy(enemyIdx).state == E_HURT)bossPt->addPattern();
}

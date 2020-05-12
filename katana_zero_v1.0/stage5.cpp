#include "stdafx.h"
#include "stage5.h"

HRESULT stage5::init()
{
	DATA->setBgColImg(IMAGEMANAGER->findImage("stage5_bg_collision"));
	DATA->setBgRenImg(IMAGEMANAGER->findImage("stage5_bg_render"));
	DATA->setBgRenImg(IMAGEMANAGER->findImage("stage5_bg_render_slow"));
	DATA->setHud(IMAGEMANAGER->findImage("hud_collapse"));
	
	fade.blackImg = IMAGEMANAGER->findImage("effect_black");
	fade.successImg = IMAGEMANAGER->findImage("effect_success");
	fade.failImg = IMAGEMANAGER->findImage("effect_fail");
	fade.cnt = fade.toNext = fade.alpha_success = fade.alpha_fail = fade.alpha_success = fade.isRender_back = fade.isRender_font = 0;
	
	fade.alpha_black = 255;

	if (DATA->getCurrentClear() == STAGE5_RESET)
	{
		fade.isFirstRender = true;
		DATA->stageInitialize(0, 30, 1344, 784);
		DATA->setStartLoc(250, 545);
	}
	else
	{
		fade.isFirstRender = false;
		DATA->stageInitialize(0, 0, 1344, 784);
		DATA->setStartLoc(250, 50);
	}


	StopCnt = 0;
	stageCnt = 0;

	playerPt = new playerClass;
	playerPt->init();
	playerPt->setCameraScale(900, 400);

	bossPt = new bossClass;
	bossPt->init();

	effectPt = new effectClass;
	effectPt->init();

	itemPt = new itemClass;
	itemPt->init();

	bossPt->generateBoss(640, 530, true, B_OUT);

	//대쉬
	//bossPt->generateBoss(270, 530, true, B_DASH_PRE);
	//bossPt->generateBoss(1120, 530, false, B_DASH_PRE);

	//점프+총알발사
	//bossPt->generateBoss(270, 530,true, B_PREJUMP);
	//bossPt->generateBoss(970, 530,false, B_PREJUMP);

	//총+유탄발사
	//bossPt->generateBoss(1120, 530, false, B_DRAWGUN);
	//bossPt->generateBoss(270, 530, true, B_DRAWGUN);

	//텔포
	//bossPt->generateBoss(500, 530, true, B_TELEPORT);

	return S_OK;
}

void stage5::release()
{
	playerPt->release();
	SAFE_DELETE(playerPt);

	bossPt->release();
	SAFE_DELETE(bossPt);

	effectPt->release();
	SAFE_DELETE(effectPt);

	itemPt->release();
	SAFE_DELETE(itemPt);
}

void stage5::update()
{
	playerPt->update();
	bossPt->update();
	effectPt->update();
	itemPt->update();

	this->bossHit();
	this->stagePattern5();

	//재시작
	if (INPUT->GetKeyDown(VK_TAB))
	{
		this->stageReset();
	}
	if (playerPt->getDead() == true && playerPt->getHurt() == false && playerPt->getPlayer().regen == true)
	{
		DATA->setDead(true);
		fade.isRender_font = true;
		if (INPUT->GetKeyDown(VK_RBUTTON))
		{
			this->release();
			this->init();
		}
	}

	//정지 효과
	if (DATA->getStop() == true)
	{
		StopCnt++;
		if (StopCnt > 12)
		{
			StopCnt = 0;
			DATA->setStop(false);
		}
	}

	//슬로우 배경 처리
	if (DATA->getSlow() == true || DATA->getStop() == true)
	{
		DATA->setBgRenImg(IMAGEMANAGER->findImage("stage5_bg_render_slow"));
	}
	else if ((playerPt->getPlayer().state == P_HURTFLY || playerPt->getPlayer().state == P_HURTGROUND))
	{
		DATA->setBgRenImg(IMAGEMANAGER->findImage("stage5_bg_render_slow"));
	}
	else if (bossPt->getBoss().state == B_DIEFLY || bossPt->getBoss().state == B_DEAD || bossPt->getBoss().state == B_DIEGROUND)
	{
		DATA->setBgRenImg(IMAGEMANAGER->findImage("stage5_bg_render_slow"));
	}
	else
	{
		DATA->setBgRenImg(IMAGEMANAGER->findImage("stage5_bg_render"));
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
}

void stage5::render()
{
	DATA->getBgCol().img->render(getMemDC(), -1 * DATA->getBgCol().x, -1 * DATA->getBgCol().y);
	DATA->getBgRen().img->render(getMemDC(), -1 * DATA->getBgCol().x, -1 * DATA->getBgCol().y);

	bossPt->render();
	playerPt->render();

	DATA->getHud()->render(getMemDC(), 0, 0);
	effectPt->render();
	itemPt->render();

	this->sceneChanger();
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
		effectPt->hitLazerRender(bossPt->getBoss().x, bossPt->getBoss().y, playerPt->getPlayer().toMouseAngle); //레이저 이펙트
		playerPt->getPlayer().isRight ? bossPt->setBossHit(false) : bossPt->setBossHit(true);
		DATA->setStop(true);
	}

	if (bossPt->getLazerVector().empty() == false)
	{
		for (int i = 0; i < bossPt->getLazerVector().size(); i++)
		{
			RECT tempRECT;
			if (IntersectRect(&tempRECT, &bossPt->getLazerRect(i), &playerPt->getPlayerRect()) && DATA->getDead() == false && playerPt->getPlayer().state != P_ROLL &&bossPt->getLazer(i).frameIdx==2 &&bossPt->getBoss().state != B_HURTRECOVER)
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
	//첫번째 패턴
	bossPt->setPlayerX(playerPt->getPlayer().x);
}

void stage5::sceneChanger()
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
	}
	
}

void stage5::stageReset()
{
	DATA->setClear(STAGE5_RESET);
	this->saveClearedStage();
	this->release();
	this->init();
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

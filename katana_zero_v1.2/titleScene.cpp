#include "stdafx.h"
#include "titleScene.h"

HRESULT titleScene::init()
{
	//렉트 위치 초기화
	SOUNDMANAGER->play("title");
	newBtn.rc = RectMake(WINSIZEX / 2 - 75, WINSIZEY - 150, 129, 45);
	newBtn.isClick = false;
	continueBtn.rc = RectMake(WINSIZEX / 2 -75, WINSIZEY - 100, 172, 45);
	continueBtn.isClick = false;
	ShowCursor(false);
	fade.blackImg = IMAGEMANAGER->findImage("effect_black");
	fade.cnt = fade.toNext = fade.alpha_success = fade.alpha_fail = fade.alpha_success = fade.isRender_back = fade.isRender_font = 0;
	fade.isFirstRender = true;
	fade.alpha_black = 255;

	DATA->setRenderUi(false);
	this->loadClearedStage();
	
	return S_OK;
}

void titleScene::release()
{
}

void titleScene::update()
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
			fade.isFirstRender = false;
		}
	}

	if (fade.isRender_back)
	{
		if (fade.alpha_black <= 252)
		{
			fade.alpha_black += 3;
		}
		else
		{
			switch (DATA->getCurrentClear())
			{
			case NONE_CLEARED:
				SOUNDMANAGER->play("stage");
				SCENEMANAGER->loadScene("스테이지1");
				break;
			case STAGE1_CLEARED:
				SOUNDMANAGER->play("stage");
				SCENEMANAGER->loadScene("스테이지2");
				break;
			case STAGE2_CLEARED:
				SOUNDMANAGER->play("stage");
				SCENEMANAGER->loadScene("스테이지3");
				break;
			case STAGE3_CLEARED:
				SOUNDMANAGER->play("stage4");
				SCENEMANAGER->loadScene("스테이지4");
				break;
			case STAGE4_CLEARED:
				SOUNDMANAGER->play("stage5");
				SCENEMANAGER->loadScene("스테이지5");
				break;
			case STAGE5_RESET:
				SOUNDMANAGER->play("stage5");
				SCENEMANAGER->loadScene("스테이지5");
				break;
			}
		}
	}


	if (PtInRect(&newBtn.rc, _ptMouse)&&fade.isFirstRender == false)
	{
		if(newBtn.isClick==false)SOUNDMANAGER->play("beep");
		newBtn.isClick = true;
		if (INPUT->GetKeyDown(VK_LBUTTON))
		{
			fade.isRender_back = true;
			SOUNDMANAGER->stop("title");
			SOUNDMANAGER->play("clear");
			DATA->setClear(NONE_CLEARED);
			//DATA->setClear(STAGE1_CLEARED);
			//DATA->setClear(STAGE2_CLEARED);
			//DATA->setClear(STAGE3_CLEARED);
			//DATA->setClear(STAGE4_CLEARED);
			
			//SOUNDMANAGER->play("stage");
			//DATA->setClear(STAGE2_CLEARED);
			//SCENEMANAGER->loadScene("스테이지3");

			//DATA->setClear(STAGE1_CLEARED);
		}
	}
	else
	{
		newBtn.isClick = false;
	}
	if (PtInRect(&continueBtn.rc, _ptMouse) && fade.isFirstRender == false)
	{
		if(continueBtn.isClick ==false)SOUNDMANAGER->play("beep");
		continueBtn.isClick = true;
		if (INPUT->GetKeyDown(VK_LBUTTON))
		{
			fade.isRender_back = true;
			SOUNDMANAGER->stop("title");
			SOUNDMANAGER->play("clear");
			switch (loadedStage[0])
			{
			case NONE_CLEARED:
				DATA->setClear(NONE_CLEARED);
				break;
			case STAGE1_CLEARED:
				DATA->setClear(STAGE1_CLEARED);
				break;
			case STAGE2_CLEARED:
				DATA->setClear(STAGE2_CLEARED);
				break;
			case STAGE3_CLEARED:
				DATA->setClear(STAGE3_CLEARED);
				break;
			case STAGE4_CLEARED:
				DATA->setClear(STAGE4_CLEARED);
				break;
			case STAGE5_RESET:
				DATA->setClear(STAGE4_CLEARED);
				break;
			}
		}
	}
	else
	{
		continueBtn.isClick = false;
	}
	

}

void titleScene::render()
{
	IMAGEMANAGER->findImage("title_bg")->render(getMemDC(), 0, 0);
	if (newBtn.isClick == false)
	{
		IMAGEMANAGER->findImage("btn_new")->render(getMemDC(), newBtn.rc.left, newBtn.rc.top);
	}
	else
	{
		IMAGEMANAGER->findImage("btn_new_clk")->render(getMemDC(), newBtn.rc.left, newBtn.rc.top);
	}
	
	if (continueBtn.isClick == false)
	{
		IMAGEMANAGER->findImage("btn_con")->render(getMemDC(), continueBtn.rc.left, continueBtn.rc.top);
	}
	else
	{
		IMAGEMANAGER->findImage("btn_con_clk")->render(getMemDC(), continueBtn.rc.left, continueBtn.rc.top);
	}
	
	IMAGEMANAGER->findImage("cursor")->render(getMemDC(), _ptMouse.x - 25, _ptMouse.y -25);

	


	fade.blackImg->alphaRender(getMemDC(), 0, 0, fade.alpha_black);

	
}

void titleScene::loadClearedStage()
{
	HANDLE file;
	DWORD write;

	file = CreateFile("save.stage", GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, loadedStage, sizeof(enum_Cleared) * 1, &write, NULL);
	CloseHandle(file);
}

#include "stdafx.h"
#include "titleScene.h"

HRESULT titleScene::init()
{
	//렉트 위치 초기화
	newBtn.rc = RectMake(WINSIZEX / 2 - 75, WINSIZEY - 150, 129, 45);
	newBtn.isClick = false;
	continueBtn.rc = RectMake(WINSIZEX / 2 -75, WINSIZEY - 100, 172, 45);
	continueBtn.isClick = false;
	ShowCursor(false);

	this->loadClearedStage();
	
	return S_OK;
}

void titleScene::release()
{
}

void titleScene::update()
{
	
	if (PtInRect(&newBtn.rc, _ptMouse))
	{
		newBtn.isClick = true;
		if (INPUT->GetKeyDown(VK_LBUTTON))
		{
			DATA->setClear(NONE_CLEARED);
			SCENEMANAGER->loadScene("스테이지1");
		}
	}
	else
	{
		newBtn.isClick = false;
	}
	if (PtInRect(&continueBtn.rc, _ptMouse))
	{
		continueBtn.isClick = true;
		if (INPUT->GetKeyDown(VK_LBUTTON))
		{
			switch (loadedStage[0])
			{
			case STAGE1_CLEARED:
				DATA->setClear(STAGE1_CLEARED);
				SCENEMANAGER->loadScene("스테이지2");
				break;
			case STAGE2_CLEARED:
				DATA->setClear(STAGE2_CLEARED);
				SCENEMANAGER->loadScene("스테이지3");
				break;
			case STAGE3_CLEARED:
				DATA->setClear(STAGE3_CLEARED);
				SCENEMANAGER->loadScene("스테이지4");
				break;
			case STAGE4_CLEARED:
				DATA->setClear(STAGE4_CLEARED);
				SCENEMANAGER->loadScene("스테이지5");
				break;
			case STAGE5_RESET:
				DATA->setClear(STAGE4_CLEARED);
				SCENEMANAGER->loadScene("스테이지5");
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

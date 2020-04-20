#include "stdafx.h"
#include "titleScene.h"

HRESULT titleScene::init()
{
	//렉트 위치 초기화
	newBtn.rc = RectMake(WINSIZEX / 2 - 50, WINSIZEY - 300, 100, 100);
	continueBtn.rc = RectMake(WINSIZEX / 2 - 50, WINSIZEY - 150, 100, 100);
	
	return S_OK;
}

void titleScene::release()
{
}

void titleScene::update()
{
	if (INPUT->GetKeyDown(VK_LBUTTON))
	{
		if (PtInRect(&newBtn.rc, _ptMouse))
		{
			DATAMANAGER->setClear(NONE_CLEARED);
			SCENEMANAGER->loadScene("스테이지1");
		}
		if (PtInRect(&continueBtn.rc, _ptMouse))
		{
			SCENEMANAGER->loadScene("게임화면");
		}
	}

}

void titleScene::render()
{
	IMAGEMANAGER->findImage("title_bg")->render(getMemDC(), 0, 0);
	Rectangle(getMemDC(), newBtn.rc);
	Rectangle(getMemDC(), continueBtn.rc);
	TextOut(getMemDC(), newBtn.rc.left, newBtn.rc.top, "NEW", strlen("NEW"));
	TextOut(getMemDC(), continueBtn.rc.left, continueBtn.rc.top, "CONTINUE", strlen("CONTINUE"));
	
}

#include "stdafx.h"
#include "mainGame.h"

//=============================================================
//	## 초기화 ## init()
//=============================================================
HRESULT mainGame::init()
{
	gameNode::init();
	//클래스 초기화를 이곳에서 한다
	
	/*앞으로 메인게임은 가각의 씬들만 관리를 한다*/
	//각각의 씬들은 메인게임 역할을 한다

	/*씬추가*/
	SCENEMANAGER->addScene("시작화면", new titleScene);
	SCENEMANAGER->addScene("게임화면", new inGameScene);
	SCENEMANAGER->addScene("로딩화면", new loadingScene);
	SCENEMANAGER->addScene("스테이지1", new stage1);
	SCENEMANAGER->addScene("스테이지2", new stage2);
	SCENEMANAGER->addScene("스테이지3", new stage3);
	SCENEMANAGER->addScene("스테이지4", new stage4);

	/*현재씬 설정*/
	SCENEMANAGER->loadScene("로딩화면");


	return S_OK;
}

//=============================================================
//	## 해제 ## release()
//=============================================================
void mainGame::release()
{
	gameNode::release();

}

//=============================================================
//	## 업데이트 ## update()
//=============================================================
void mainGame::update()
{
	gameNode::update();
	
	//씬매니져 업데이트
	SCENEMANAGER->update();
	//사운드매니져 업데이트(이놈안해주면 사운드가 정상작동안함)
	SOUNDMANAGER->update();
}

//=============================================================
//	## 렌더 ## render()
//=============================================================
void mainGame::render()
{
	//흰색 빈 비트맵
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
//=============================================================	

	//씬매니져 렌더
	SCENEMANAGER->render();
	//타임매니져 렌더
	TIMEMANAGER->render(getMemDC());

	//글자배경모드
	SetBkMode(getMemDC(), TRANSPARENT);
	//텍스트 색상 변경
	SetTextColor(getMemDC(), RGB(0, 0, 0));

//=============================================================
	//백버퍼의 내용을 화면DC에 그린다
	this->getBackBuffer()->render(getHDC());
}

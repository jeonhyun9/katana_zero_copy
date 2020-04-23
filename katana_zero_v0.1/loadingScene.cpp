#include "stdafx.h"
#include "loadingScene.h"

HRESULT loadingScene::init()
{
	//로딩클래스 초기화
	_loading = new loading;
	_loading->init();

	//로딩 이미지 및 사운드 초기화
	this->loadingImage();
	this->loadingSound();

	return S_OK;
}

void loadingScene::release()
{
	//로딩클래스 해제
	_loading->release();
	SAFE_DELETE(_loading);
}

void loadingScene::update()
{
	//로딩클래스 업데이트
	_loading->update();

	//로딩완료후 화면 변경
	if (_loading->loadingDone())
	{
		SCENEMANAGER->loadScene("시작화면");
	}
}

void loadingScene::render()
{
	//로딩클래스 렌더
	_loading->render();
}


//로딩이미지 함수(이곳에 이미지를 전부 넣어라)
void loadingScene::loadingImage()
{
	_loading->loadImage("title_bg","title_bg.bmp", 1280, 720);
	/*플레이어*/
	_loading->loadFrameImage("player_idle", "images/player/player_idle_11x2.bmp", 770, 140, 11, 2);
	_loading->loadFrameImage("player_run", "images/player/player_run_10x2.bmp", 880, 128, 10, 2);
	_loading->loadFrameImage("player_jump", "images/player/player_fall_4x2.bmp", 240, 168, 4, 2);
	_loading->loadFrameImage("player_roll", "images/player/player_roll_7x2.bmp", 658, 128, 7, 2);

	/*에너미*/
	_loading->loadFrameImage("enemy_bold_idle", "images/enemy/enemy_bold_Idle_8x2.bmp", 576, 140, 8, 2);
	_loading->loadFrameImage("enemy_bold_run", "images/enemy/enemy_bold_run_10x2.bmp", 900, 156, 10, 2);
	_loading->loadFrameImage("enemy_bold_walk", "images/enemy/enemy_bold_walk_8x2.bmp", 544, 152, 8, 2);
	_loading->loadFrameImage("enemy_bold_hurt", "images/enemy/enemy_bold_hurt_14x2.bmp", 1036, 128, 14, 2);

	/*스테이지1*/
	_loading->loadImage("stage1_bg_collision", "images/stage1_bg_collision.bmp", 2176, 3500);
	_loading->loadImage("stage1_bg_render", "images/stage1_bg_render.bmp", 2176, 3500);
	
	/*스테이지2*/
	_loading->loadImage("stage2_bg_collision", "images/stage2_bg_collision.bmp", 4288, 1000);


	//로딩이너무 빠르게 진행되서 천천히 돌아가도록 테스트용으로 만들기
	for (int i = 0; i < 10; i++)
	{
		_loading->loadImage("테스트", WINSIZEX, WINSIZEY);
	}

}

//로딩사운드 함수(이곳에 사운드를 전부 넣어라)
void loadingScene::loadingSound()
{
}

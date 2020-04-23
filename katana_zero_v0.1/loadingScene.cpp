#include "stdafx.h"
#include "loadingScene.h"

HRESULT loadingScene::init()
{
	//�ε�Ŭ���� �ʱ�ȭ
	_loading = new loading;
	_loading->init();

	//�ε� �̹��� �� ���� �ʱ�ȭ
	this->loadingImage();
	this->loadingSound();

	return S_OK;
}

void loadingScene::release()
{
	//�ε�Ŭ���� ����
	_loading->release();
	SAFE_DELETE(_loading);
}

void loadingScene::update()
{
	//�ε�Ŭ���� ������Ʈ
	_loading->update();

	//�ε��Ϸ��� ȭ�� ����
	if (_loading->loadingDone())
	{
		SCENEMANAGER->loadScene("����ȭ��");
	}
}

void loadingScene::render()
{
	//�ε�Ŭ���� ����
	_loading->render();
}


//�ε��̹��� �Լ�(�̰��� �̹����� ���� �־��)
void loadingScene::loadingImage()
{
	_loading->loadImage("title_bg","title_bg.bmp", 1280, 720);
	/*�÷��̾�*/
	_loading->loadFrameImage("player_idle", "images/player/player_idle_11x2.bmp", 770, 140, 11, 2);
	_loading->loadFrameImage("player_run", "images/player/player_run_10x2.bmp", 880, 128, 10, 2);
	_loading->loadFrameImage("player_jump", "images/player/player_fall_4x2.bmp", 240, 168, 4, 2);
	_loading->loadFrameImage("player_roll", "images/player/player_roll_7x2.bmp", 658, 128, 7, 2);

	/*���ʹ�*/
	_loading->loadFrameImage("enemy_bold_idle", "images/enemy/enemy_bold_Idle_8x2.bmp", 576, 140, 8, 2);
	_loading->loadFrameImage("enemy_bold_run", "images/enemy/enemy_bold_run_10x2.bmp", 900, 156, 10, 2);
	_loading->loadFrameImage("enemy_bold_walk", "images/enemy/enemy_bold_walk_8x2.bmp", 544, 152, 8, 2);
	_loading->loadFrameImage("enemy_bold_hurt", "images/enemy/enemy_bold_hurt_14x2.bmp", 1036, 128, 14, 2);

	/*��������1*/
	_loading->loadImage("stage1_bg_collision", "images/stage1_bg_collision.bmp", 2176, 3500);
	_loading->loadImage("stage1_bg_render", "images/stage1_bg_render.bmp", 2176, 3500);
	
	/*��������2*/
	_loading->loadImage("stage2_bg_collision", "images/stage2_bg_collision.bmp", 4288, 1000);


	//�ε��̳ʹ� ������ ����Ǽ� õõ�� ���ư����� �׽�Ʈ������ �����
	for (int i = 0; i < 10; i++)
	{
		_loading->loadImage("�׽�Ʈ", WINSIZEX, WINSIZEY);
	}

}

//�ε����� �Լ�(�̰��� ���带 ���� �־��)
void loadingScene::loadingSound()
{
}

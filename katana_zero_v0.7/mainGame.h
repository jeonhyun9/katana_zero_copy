#pragma once
#include "gameNode.h"
/*������ ���ΰ����� ������ ���鸸 ������ �Ѵ�*/
/*������ϸ� ���ΰ��ӿ� �߰��ϱ�*/
#include "titleScene.h"
#include "inGameScene.h"
#include "loadingScene.h"
#include "stage1.h"
#include "stage2.h"
#include "stage3.h"
#include "stage4.h"

class mainGame : public gameNode
{
public:
	HRESULT init();
	void release();
	void update();
	void render();

	mainGame() {}
	~mainGame() {}
};


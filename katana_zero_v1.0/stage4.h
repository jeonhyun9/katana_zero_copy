#pragma once
#include "gameNode.h"
#include "playerClass.h"
#include "enemyClass.h"
#include "effectClass.h"
#include "objectClass.h"
#include "itemClass.h"
#include "bossClass.h"

class stage4 : public gameNode
{
private:
	playerClass* playerPt;
	enemyClass* enemyPt;
	effectClass* effectPt;
	objectClass* objectPt;
	itemClass* itemPt;
	bossClass* bossPt;

	struct_FadeOut fade;

	struct_Rect warningArea;
	int warningCnt;

	int StopCnt;

	int stageCnt;
	int enemyIdx;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	void toNextStage();
	void saveClearedStage();
	void sceneChanger();

	//���� �浹
	void bossHit();

	//�������� 4 ����
	void stagePattern4();

	//�� �ν�
	void findPlayer();

	stage4() {};
	~stage4() {};

};


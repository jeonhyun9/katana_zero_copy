#pragma once
#include "gameNode.h"
#include "playerClass.h"
#include "enemyClass.h"
#include "effectClass.h"
#include "objectClass.h"
#include "itemClass.h"
#include "bossClass.h"


class stage5 : public gameNode
{
private:
	playerClass* playerPt;
	effectClass* effectPt;
	itemClass* itemPt;
	bossClass* bossPt;

	struct_FadeOut fade;

	int StopCnt;
	int stageCnt;
	bool isBreathOut;
	bool isEndingPlay;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//���� �浹
	void bossHit();

	//�������� 5 ����
	void stagePattern5();
	void sceneChanger();

	void stageReset();
	void saveClearedStage();

	//���� ����
	void ending();

	stage5() {};
	~stage5() {};

};


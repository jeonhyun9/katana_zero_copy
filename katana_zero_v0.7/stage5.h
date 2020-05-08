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
	enemyClass* enemyPt;
	effectClass* effectPt;
	objectClass* objectPt;
	itemClass* itemPt;
	bossClass* bossPt;

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

	//���� �浹
	void bossHit();

	//�������� 4 ����
	void stagePattern5();

	//�� �ν�
	void findPlayer();

	stage5() {};
	~stage5() {};

};


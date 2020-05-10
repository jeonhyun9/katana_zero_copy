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

	//보스 충돌
	void bossHit();

	//스테이지 4 패턴
	void stagePattern5();

	//적 인식
	void findPlayer();

	stage5() {};
	~stage5() {};

};


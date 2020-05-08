#pragma once
#include "gameNode.h"
#include "playerClass.h"
#include "enemyClass.h"
#include "effectClass.h"
#include "objectClass.h"
#include "itemClass.h"

class stage1 : public gameNode
{
private:
	playerClass* playerPt;
	enemyClass* enemyPt;
	effectClass* effectPt;
	objectClass* objectPt;
	itemClass* itemPt;

	struct_Rect warningArea;
	int warningCnt;

	int StopCnt;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	void toNextStage();
	void saveClearedStage();

	stage1() {};
	~stage1() {};

};


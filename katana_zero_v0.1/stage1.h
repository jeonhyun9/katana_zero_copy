#pragma once
#include "gameNode.h"
#include "playerClass.h"
#include "enemyClass.h"

class stage1 : public gameNode
{
private:
	playerClass* playerPt;
	enemyClass* enemyPt;
	
public:
	HRESULT init();
	void release();
	void update();
	void render();

	stage1() {};
	~stage1() {};

};


#pragma once
#include "gameNode.h"
#include "playerClass.h"
class stage2 :public gameNode
{
private:
	playerClass* playerPt;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	stage2() {}
	~stage2() {}
};


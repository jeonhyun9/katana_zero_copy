#pragma once
#include "gameNode.h"

class inGameScene : public gameNode
{
private:


public:
	HRESULT init();
	void release();
	void update();
	void render();

	inGameScene() {}
	~inGameScene() {}
};


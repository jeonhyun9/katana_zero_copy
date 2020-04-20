#pragma once
#include "gameNode.h"
class stage1 : public gameNode
{
private:

public:
	HRESULT init();
	void release();
	void update();
	void render();

	stage1() {};
	~stage1() {};

};


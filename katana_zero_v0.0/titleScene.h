#pragma once
#include "gameNode.h"
struct startButton
{
	RECT rc;
};


class titleScene : public gameNode
{
private:
	startButton newBtn;
	startButton continueBtn;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	titleScene() {}
	~titleScene() {}
};


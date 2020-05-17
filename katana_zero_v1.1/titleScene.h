#pragma once
#include "gameNode.h"
struct startButton
{
	RECT rc;
	bool isClick;
};


class titleScene : public gameNode
{
private:
	startButton newBtn;
	startButton continueBtn;

	struct_FadeOut fade;

	enum_Cleared loadedStage[1];

public:
	HRESULT init();
	void release();
	void update();
	void render();

	void loadClearedStage();

	titleScene() {}
	~titleScene() {}
};


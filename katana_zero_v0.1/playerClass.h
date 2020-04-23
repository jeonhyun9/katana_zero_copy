#pragma once
#include "gameNode.h"
class playerClass : public gameNode
{
private:
	struct_Player player;
	struct_Camera camera;

	image* player_run;
	image* player_idle;
	image* player_jump;
	image* player_roll;
	image* player_fall;
	image* player_grab;
	image* player_attack;
	image* player_attack_shadow;
	image* player_attack_shadow2;
	image* player_attack_shadow3;
	image* player_slash;

	float findleft;
	float findright;
	float findbottom;
	float findtop;
	float findcenterX;
	float findcenterY;

	float findY;

	bool JUMP;
	bool JUMP_BELOW;
	bool RUN;
	bool FALL;
	bool ROLL;
	bool REACHED_CLEAR;

	bool WALLCRASH_LEFT;
	bool WALLCRASH_RIGHT;
	bool WALLCRASH_BOTTOM;
	bool WALLCRASH_TOP;
	bool ON_FLOOR;

	int jumpCnt;
	int jump_belowCnt;
	int frameCnt;
	int frameIdx;
	int rollCnt;

	float gravity;
	
	char str2[100];
public:
	HRESULT init();
	void release();
	void update();
	void render();

	//�÷��̾� ��
	struct_Player getPlayer() { return player; }
	RECT getPlayerRect() { return player.rc; }

	//�ȼ� �浹
	void pixelCollisionLeft();
	void pixelCollisionRight();
	void pixelCollisionBottom();
	void pixelCollisionTop();
	void pixelCollisionFloor();
	void pixelCollisionClearLocation();

	//�÷��̾� �ִϸ��̼�
	void playerAnimation();

	//���̺�, �ε�
	void saveClearedStage();

	//�������� �̵�
	void toNextStage();

	
	playerClass() {};
	~playerClass() {};
};


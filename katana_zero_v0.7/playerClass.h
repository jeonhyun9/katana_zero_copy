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
	image* player_slash_basic;
	image* player_hurtfly;
	image* player_hurtground;
	image* player_flip;
	image* player_doorbreak;
	image* player_run_to_idle;
	image* player_idle_to_run;
	

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
	bool STAND_ON_BLACK;
	bool ATTACK;
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

	int frameTum;
	int oldFrameTum;

	int regenCnt;

	bool SLOW;
	float SLOW_Ratio;

	bool HURT;
	bool DEAD;

	int hurtCnt;
	int rollCnt;

	float gravity;
	float tempY;
	
	char str2[100];
public:
	HRESULT init();
	void release();
	void update();
	void render();

	//플레이어 겟
	struct_Player getPlayer() { return player; }
	RECT getPlayerRect() { return player.rc; }

	//플레이어 셋
	void setPlayerToEnemyAngle(float _set) { player.angleToEnemy = _set; }
	void setPlayerReLocation(float _set) { player.y += _set; }
	void setHurt(bool _set) { HURT = _set; gravity = 0; player.y -= 25; WALLCRASH_BOTTOM = false; }
	void setSlowOn() { SLOW = true; }
	void setPlayerRight(bool _set) { player.isRight = _set; }
	void setPlayerState(enum_PlayerState _set) { player.state = _set; }
	void setDoorTouchedLeft(bool _set) { player.doorTouchedLeft = _set; }
	void setDoorTouchedRight(bool _set) { player.doorTouchedRight = _set; }
	void setFrameIdxZero() { frameIdx = 0; }
	void setPickedItem(int _idx) { player.pickedItemIdx = _idx; }
	void setItemIsPick(bool _set) { player.isPick = _set; }
	void setCameraScale(float _width, float _height)
	{
		camera.width = _width;
		camera.height = _height;
	}

	void setJump() { JUMP = true; }
	void setBatteryRemain(bool _set) { player.isBatteryRemain = _set; }

	bool getHurt() { return HURT; }
	bool getDead() { return DEAD; }


	//공격
	bool getIsAttack() { return ATTACK; }
	RECT getAttRect() { return player.att.rc; }

	//픽셀 충돌
	void pixelCollisionLeft();
	void pixelCollisionRight();
	void pixelCollisionBottom();
	void pixelCollisionTop();
	void pixelCollisionFloor();
	void pixelCollisionClearLocation();
	void rollOnFloor();
	
	void cameraReset();


	//플레이어 애니메이션
	void playerAnimation();


	//세이브, 로드
	void saveClearedStage();

	//스테이지 이동
	void toNextStage();

	
	playerClass() {};
	~playerClass() {};
};


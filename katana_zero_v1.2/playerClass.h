#pragma once
#include "gameNode.h"
class playerClass : public gameNode
{
private:
	struct_Player player;
	struct_Camera camera;

	struct_Effect landDust;
	struct_Effect jumpDust;
	struct_Effect wallJumpDust;

	struct_Effect sAfterImage;
	vector<struct_Effect> vAfterImage;

	struct_Effect sDust;
	vector<struct_Effect> vDust;

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
	image* player_play_song;
	
	bool rcRender;

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

	bool hurtSwith;

	

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
	RECT getPlayerRightRect() { return player.rc_Right; }
	RECT getPlayerLeftRect() { return player.rc_Left; }

	//플레이어 셋
	void setPlayerToEnemyAngle(float _set) { player.angleToEnemy = _set; }
	void setPlayerReLocation(float _set) { player.y += _set; }
	void setPlayerReLocationX(float _set) { player.x += _set; }

	//먼지 생성
	void generateLandDust(float x, float y);
	void generateJumpDust(float x, float y);
	void generateWallJumpDust(float x, float y,bool _set);
	void generateDust(float x, float y,int _frameIdx);
	void dustAnimation();

	//잔상 생성
	void generateAfterImage(float x, float y, int frameIdx);
	void afterImageUpdate();

	//플레이어 죽는 함수
	void setHurt(bool _set) {
		if (player.hurtSwitch)
		{
			if (SOUNDMANAGER->isPlaySound("slow_in"))SOUNDMANAGER->stop("slow_in");
			if (SOUNDMANAGER->isPlaySound("slow_out"))SOUNDMANAGER->stop("slow_out");
			SOUNDMANAGER->play("p_dead");
			HURT = _set; gravity = 0;
			player.y -= 25;
			WALLCRASH_BOTTOM = false;
		}
	}
	void setSlowOn(bool _set) { SLOW = _set; }
	void setPlayerRight(bool _set) { player.isRight = _set; }

	//플레이어 상태 바꾸는 함수
	void setPlayerState(enum_PlayerState _set) 
	{ 
		if (_set == P_PLAYSONG)
		{
			player.isRight ? frameIdx = 0 : frameIdx = 31;
		}
		if (_set == P_DOORBREAK)
		{
			player.isRight ? frameIdx = 0 : frameIdx = 10;
		}
		player.state = _set; 
	}

	//문과의 충돌 판정
	void setDoorTouchedLeft(bool _set) { player.doorTouchedLeft = _set; }
	void setDoorTouchedRight(bool _set) { player.doorTouchedRight = _set; }

	void setFrameIdxZero() { frameIdx = 0; }

	//아이템 줍기
	void setPickedItem(int _idx) { player.pickedItemIdx = _idx; }
	void setItemIsPick(bool _set) { player.isPick = _set; }

	//카메라 크기 변경
	void setCameraScale(float _width, float _height)
	{
		camera.width = _width;
		camera.height = _height;
	}

	void setClear() { player.clear = true; }

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
	
	//카메라 보정 함수
	void cameraReset();

	//플레이어 애니메이션
	void playerAnimation();

	//세이브, 로드
	void saveClearedStage();

	//스테이지 이동
	void toNextStage();

	//랜덤으로 사운드 출력
	void randomSound(string _1, string _2, string _3, string _4, float _volume)
	{
		int num = rand() % 4 + 1;
		switch (num)
		{
		case 1:SOUNDMANAGER->play(_1, _volume);
			break;
		case 2:SOUNDMANAGER->play(_2, _volume);
			break;
		case 3:SOUNDMANAGER->play(_3, _volume);
			break;
		case 4:SOUNDMANAGER->play(_4, _volume);
			break;
		}
	}
	void randomSound(string _1, string _2, string _3, float _volume)
	{
		int num = rand() % 3 + 1;
		switch (num)
		{
		case 1:SOUNDMANAGER->play(_1, _volume);
			break;
		case 2:SOUNDMANAGER->play(_2, _volume);
			break;
		case 3:SOUNDMANAGER->play(_3, _volume);
			break;
		}
	}
	void randomSound(string _1, string _2, float _volume)
	{
		int num = rand() % 2 + 1;
		switch (num)
		{
		case 1:SOUNDMANAGER->play(_1, _volume);
			break;
		case 2:SOUNDMANAGER->play(_2, _volume);
			break;
		}
	}
	void randomSound(string _1, string _2, string _3, string _4)
	{
		int num = rand() % 4 + 1;
		switch (num)
		{
		case 1:SOUNDMANAGER->play(_1);
			break;
		case 2:SOUNDMANAGER->play(_2);
			break;
		case 3:SOUNDMANAGER->play(_3);
			break;
		case 4:SOUNDMANAGER->play(_4);
			break;
		}
	}
	void randomSound(string _1, string _2, string _3)
	{
		int num = rand() % 3 + 1;
		switch (num)
		{
		case 1:SOUNDMANAGER->play(_1);
			break;
		case 2:SOUNDMANAGER->play(_2);
			break;
		case 3:SOUNDMANAGER->play(_3);
			break;
		}
	}
	void randomSound(string _1, string _2)
	{
		int num = rand() % 2 + 1;
		switch (num)
		{
		case 1:SOUNDMANAGER->play(_1);
			break;
		case 2:SOUNDMANAGER->play(_2);
			break;
		}
	}

	
	playerClass() {};
	~playerClass() {};
};


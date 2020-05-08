#pragma once
#include "singletonBase.h"
enum enum_Cleared
{
	NONE_CLEARED,
	STAGE1_CLEARED,
	STAGE2_CLEARED,
	STAGE3_CLEARED,
	STAGE4_CLEARED,
};

enum enum_ObjectCategory
{
	O_DOOR,
};

enum enum_BossState
{
	B_IDLE,
	B_AIM,
	B_HURTRECOVER,
	B_DRAWGUN,
	B_PREJUMP,
	B_JUMP,
	B_WALLGRAB,
	B_WALLJUMP,
	B_LAND,
};

enum enum_PlayerState
{
	P_IDLE,
	P_RUN,
	P_JUMP,
	P_FALL,
	P_ROLL,
	P_ATTACK,
	P_HURTFLY,
	P_HURTGROUND,
	P_RECOVER,
	P_GRAB,
	P_FLIP,
	P_DOORBREAK,
	P_RUNTOIDLE,
	P_IDLETORUN,
};

enum enum_EnemyState
{
	E_IDLE,
	E_WALK,
	E_RUN,
	E_AIM,
	E_HURT,
	E_ATTACK,
	E_GEN,
};

enum enum_EnemyCategory
{
	E_BOLD,
	E_COP,
	E_POMP,
	E_GRUNT,
	E_TURRET,
};

enum enum_ItemCategory
{
	I_BOTTLE,
	I_BUST,
	I_KNIFE,
};

struct struct_Rect
{
	image* img;
	RECT rc;

	float x;
	float y;

	int width;
	int height;

	bool isAvailable;
	int width_old;
};

struct struct_Backgrounds
{
	image* img;
	float x;
	float y;
	int width;
	int height;

};

struct struct_Bullet
{
	image* img;
	RECT rc;
	float x;
	float y;
	float angle;
	float speed;

	int fireCnt;

	int width;
	int height;

	bool fire;
	bool isReflected;
	
};

struct struct_Object
{
	image* img;
	RECT rc;
	RECT findRange;

	enum_ObjectCategory category;
	float x;
	float y;
	
	int width;
	int height;

	int findWidth;
	int findHeight;

	int frameCnt;
	int frameTum;
	int oldFrameTum;
	int frameY;
	int frameIdx;

	bool isRight;
	bool isBreak;
	bool isPlayerReachedFindArea;
	bool isRender;
};

struct struct_Item
{
	image* img;
	image* uiImg;
	image* floatingImg;
	
	struct_Rect arrow;

	RECT rc;
	RECT arrowRC;

	enum_ItemCategory category;
	bool isPick;
	bool isThrow;
	bool isFirstPick;
	bool isFloating;

	float x;
	float y;
	float alpha;

	int floatingCnt;
	int floatingTime;

	float speed;
	float shootAngle;
	float rotateAngle;

	int width;
	int height;
};

struct struct_Camera
{
	RECT rc;
	int width;
	int height;
};

struct struct_effect
{
	float x;
	float y;
	bool isRender;
	int frameIdx;
	int frameTum;
	int oldFrameTum;
	int frameCnt;
	image* img;
};

struct struct_Player
{
	RECT rc;
	struct_Rect att;
	
	bool clear;
	bool isRight;

	bool wallGrabLeft;
	bool wallGrabRight;
	bool WALLGRAB;
	bool FLIP;
	bool touchedTop_Black;

	bool doorTouchedLeft;
	bool doorTouchedRight;

	int pickedItemIdx;
	bool isPick;

	enum_PlayerState state;

	bool isAttack;
	bool isReflected;
	bool regen;
	bool isBatteryRemain;

	float x;
	float y;
	float centerX;
	float centerY;
	float effectX;
	float effectY;

	float renderX;

	int height;
	int width;
	float angle;
	float toMouseAngle;
	float attAngle;
	float angleToEnemy;

	float speed;
	float speed_old;
	float attSpeed;
	float attSpeed_old;
	float jumpSpeed;
	float jumpSpeed_old;
	float fallSpeed;
	float fallSpeed_old;
	float rollSpeed;
	float rollSpeed_old;
	float jumpBelowSpeed;
	float jumpBelowSpeed_old;
	float flipSpeed;
	float flipSpeed_old;

	int frameTum;

};

struct struct_Enemy
{
	RECT rc;
	struct_Rect att;
	struct_Rect detect;
	struct_Bullet bullet[5];
	struct_effect reflect;

	bool isRight;
	bool isCrashedLeft;
	bool isCrashedRight;
	bool isCrashedTop;
	bool isCrashedBottom;
	bool isDoorDead;
	bool isDoorDeadMove;
	bool isDoorReached;

	float doorReachedX;
	

	bool isPlayerRight;
	bool isSplatted;
	bool isFire;
	bool isAttackStart;
	bool isAttack;
	bool isAttackReady;

	bool isReflectDead;

	int alpha;

	enum_EnemyState state;
	enum_EnemyCategory category;

	image* idleImg;
	image* runImg;
	image* walkImg;
	image* hurtImg;
	image* deadImgL;
	image* deadImgR;
	image* bloodImg;
	image* bloodRemainImgL;
	image* bloodRemainImgR;
	image* slashHitImg;
	image* attackImg;
	image* aimImg_L;
	image* aimImg_R;
	image* gunImg_L;
	image* gunImg_R;
	image* rightarm_L;
	image* rightarm_R;
	image* leftarm_L;
	image* leftarm_R;
	image* slashImg;
	image* gunsparkImg;

	int bloodAlpha;

	float bloodRemainX;
	float bloodRemainY;

	float x;
	float y;
	float renderX;

	float centerX;
	float centerY;

	float distanceToPlayer;
	float angleToPlayer;

	float founded;
	float attackRange;
	
	float gunsparkY;
	float gunsparkX;
	float distanceToPlayer_Y;

	float findL;
	float findR;
	float findT;
	float findB;
	float findY;

	int width;
	int height;

	int frameCnt;
	int frameIdx;

	int fireCnt;

	int frameY;
	int frameX;

	int attackCnt;
	int attackTum;

	int frameCntBlood;
	int frameIdxBlood;

	int frameTum;
	int oldFrameTum;

	int bloodFrameTum;
	int oldBloodFrameTum;

	int slashFrameTum;
	int oldSlashFrameTum;

	int frameCntHitSlash;
	int frameIdxHitSlash;

	int findRange;

	float speed;
	float speed_old;

};

struct struct_Boss
{
	RECT rc;
	RECT rcLazer;
	image* idleImg;
	image* aimImg;
	image* hurtRecoverImg;
	image* aimLazerImg;
	image* lazerImg;
	image* drawGunImg;
	image* jumpImg;
	image* wallJumpImg;
	image* wallGrabImg;
	image* preJumpImg;
	image* landImg;


	enum_BossState state;

	bool isRight;
	bool isCrashedLeft;
	bool isCrashedRight;
	bool isCrashedBottom;

	bool isGenrateCoolTime;
	bool isFire;

	bool isWallJump;
	int wallJumpCnt;
	
	int currentPattern;

	float gravity;
	float speed;
	float jumpSpeed;
	float wallJumpSpeed;
	float moveAngle;
	float shootAngle;
	float gunAngle;
	float x;
	float y;
	
	float aimAngle;

	float findL;
	float findR;
	float findT;
	float findB;
	float findY;
	float centerX;
	float centerY;

	int width;
	int height;

	int frameCnt;
	int frameIdx;
	int lazerY;
	int frameY;
	int frameTum;
	int oldFrameTum;

	bool lazerReady;
	bool lazerHitBox;
	int aimCnt;
};


class allData : public singletonBase <allData>
{
private:
	enum_Cleared clearedStage;

	POINT startLoc;

	struct_Backgrounds bgCollision;
	struct_Backgrounds bgRender;

	image* hud;

	bool SLOW_inData;
	float SLOW_RATIO_inData;

	bool STOP;

	bool PLAYERDEAD;
	int a;



public:
	HRESULT init();
	void release();

	enum_Cleared getCurrentClear() { return clearedStage; }
	void setClear(enum_Cleared _set) { clearedStage = _set; }

	image* getHud() { return hud; }
	void setHud(image* _img) { hud = _img; }

	bool getStop() { return STOP; }
	void setStop(bool _set) { STOP = _set; }

	POINT getStartLoc() { return startLoc; }
	void setStartLoc(float _x, float _y) { startLoc.x = _x; startLoc.y = _y; }

	struct_Backgrounds getBgCol() { return bgCollision; }
	void setBgColImg(image* _img) { bgCollision.img = _img; }
	void setBgColX(float _x) { bgCollision.x = _x; }
	void addBgX(float _x) { bgCollision.x += _x; }
	void minusBgX(float _x) { bgCollision.x -= _x; }
	void addBgY(float _y) { bgCollision.y += _y; }
	void minusBgY(float _y) { bgCollision.y -= _y; }

	struct_Backgrounds getBgRen() { return bgRender; }
	void setBgRenImg(image* _img) { bgRender.img = _img; }
	void setBgRenX(float _x) { bgRender.x = _x; }
	void setBgRenY(float _y) { bgRender.y = _y; }

	bool getDead() { return PLAYERDEAD; }
	void setDead(bool _set) { PLAYERDEAD = _set; }

	//½½·Î¿ì
	void setSlow(bool _set) { SLOW_inData = _set; }
	bool getSlow() { return SLOW_inData; }

	void setSlowRatio(float _set) { SLOW_RATIO_inData = _set; }
	float getSlowRatio() { return SLOW_RATIO_inData; }

	void stageInitialize(int _width, int _height) 
	{ 
		bgCollision.x = bgCollision.y = bgRender.x = bgRender.y = 0; 
		bgCollision.width = _width; 
		bgCollision.height = _height;
		STOP = false;
		PLAYERDEAD = false;
	}

	void stageInitialize(float _x, float _y, int _width, int _height)
	{
		bgCollision.x = bgRender.x = _x;
		bgCollision.y = bgRender.y = _y;
		bgCollision.width = _width;
		bgCollision.height = _height;
		STOP = false;
		PLAYERDEAD = false;
	}

	int getA() { return a; }
	void setA(int _set) { a = _set; }

	allData() {}
	~allData() {}
};


#pragma once
#include "singletonBase.h"
enum enum_Cleared
{
	NONE_CLEARED,
	STAGE1_CLEARED,
	STAGE2_CLEARED,
	STAGE3_CLEARED,
	STAGE4_CLEARED,
	STAGE5_RESET,
};

enum enum_ObjectCategory
{
	O_DOOR,
	O_DRUM,
};

enum enum_BossState
{
	B_IDLE,
	B_AIM,
	B_HURTRECOVER,
	B_DRAWRIFLE,
	B_PREJUMP,
	B_JUMP,
	B_WALLGRAB,
	B_WALLJUMP,
	B_LAND,
	B_DASH_END,
	B_DASH_PRE,
	B_SWEEP,
	B_DRAWGUN,
	B_SHOOT,
	B_TELEPORT,
	B_TELEPORT_GROUND,
	B_DASH,
	B_IN,
	B_OUT,
	B_DIEGROUND,
	B_HEADGROUND,
	B_HEADFLY,
	B_DIEFLY,
	B_NOHEAD,
	B_DEAD,
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
	P_PLAYSONG,
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
	I_MOLOTOV,
};

enum enum_HeadState
{
	H_FLY,
	H_GROUND,
};

struct struct_Image
{
	image* img;
	float x;
	float y;
	int cnt;
	bool isMove;
};

struct effect
{
	image* img;
	image* mineImg;

	RECT rc;
	float speed;
	bool isRender;
	bool isRight;

	int frameCnt;
	int frameIdx;
	int frameY;
	int frameTum;
	int oldFrameTum;

	int width;
	int height;

	float angle;

	float x;
	float y;
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

struct struct_head
{
	float x;
	float y;
	float moveAngle;
	float gravity;

	bool isCrashedLeft;
	bool isCrahsedRight;
	bool isCrahsedBottom;
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

struct struct_Effect
{
	RECT rc;
	float x;
	float y;
	float angle;


	bool isRight;
	bool isRotate;
	bool isRender;
	bool isGround;

	int frameX;
	int frameY;
	int frameIdx;
	int frameTum;
	int oldFrameTum;
	int frameCnt;
	int alpha;

	image* img;
};

struct struct_FadeOut
{
	image* blackImg;
	image* successImg;
	image* failImg;
	image* madeByImg;

	int alpha_black;
	int alpha_success;
	int alpha_fail;
	int alpha_madeby;

	int cnt;

	bool isRender_back;
	bool isRender_font;
	bool isRender_madeby;
	bool toNext;
	bool isFirstRender;

};

struct struct_Player
{
	RECT rc;
	RECT rc_Right;
	RECT rc_Left;
	struct_Rect att;
	

	int switchCnt;
	bool switchFloating;
	bool clear;
	bool isRight;
	bool isReflect;
	bool hurtSwitch;

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
	struct_Effect reflect;

	bool realDead;
	bool isRight;
	bool isCrashedLeft;
	bool isCrashedRight;
	bool isCrashedTop;
	bool isCrashedBottom;
	bool isDoorDead;
	bool isDoorDeadMove;
	bool isDoorReached;
	bool isExplosionDead;
	bool alreadyDead;

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
	int foundedCnt;

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

	float deadAngle;
	float gravity;
	bool isHurtFly;

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
	image* drawRifleImg;
	image* jumpImg;
	image* wallJumpImg;
	image* wallGrabImg;
	image* preJumpImg;
	image* landImg;
	image* dashEndImg;
	image* preDashImg;
	image* sweepImg;
	image* drawGunImg;
	image* shootImg;
	image* teleportImg;
	image* dashImg;
	image* teleportGroundImg;
	image* inImg;
	image* outImg;
	image* dieGroundImg;
	image* headGroundImg;
	image* headFlyImg;
	image* dieFlyImg;
	image* noHeadImg;
	image* deadImg;

	enum_BossState state;
	enum_HeadState headState;

	bool isRight;
	bool isCrashedLeft;
	bool isCrashedRight;
	bool isCrashedBottom;
	bool isLazerGenerated;
	
	float distance;

	bool isGenrateCoolTime;
	bool isFire;

	bool isWallJump;
	int stateCnt;
	
	int currentPattern;

	float gravity;
	float speed;
	float jumpSpeed;
	float wallJumpSpeed;
	float fallSpeed;
	float moveAngle;
	float shootAngle;
	float gunAngle;
	float x;
	float y;
	
	float noHead_x;
	float noHead_y;

	float aimAngle;

	float findL;
	float findR;
	float findT;
	float findB;
	float findY;
	float centerX;
	float centerY;

	float dashSpeed;

	int width;
	int height;

	int frameCnt;
	int headFrameCnt;
	int frameIdx;
	int headFrameIdx;
	int headFrameY;
	int lazerY;
	int frameY;
	int frameTum;
	int oldFrameTum;

	bool lazerReady;
	bool lazerHitBox;
	int aimCnt;
};

struct struct_Switch
{
	bool on;
	bool isRight;
	float angle;
	float x;
	float y;
};

class allData : public singletonBase <allData>
{
private:
	enum_Cleared clearedStage;

	POINT startLoc;
	RECT playerRect;
	RECT attRect;
	RECT bossRect;

	struct_Backgrounds bgCollision;
	struct_Backgrounds bgRender;

	image* hud;

	struct_Player playerData;
	struct_Boss bossData;

	bool SLOW_inData;
	float SLOW_RATIO_inData;

	bool renderUi;

	bool bossHit;
	bool canClear;
	
	bool onPlay;

	struct_Switch reflect;
	struct_Switch hitLazer;

	bool isBatteryRemain;
	bool hurt;
	bool roll;

	bool STOP;

	bool PLAYERDEAD;
	int a;

public:
	HRESULT init();
	void release();
	
	void setCanClear(bool _set)
	{
		canClear = _set;
	}

	void setBatteryRemain(bool _set)
	{
		isBatteryRemain = _set;
	}

	bool getBatteryRemain()
	{
		return isBatteryRemain;
	}

	bool getCanClear() { return canClear; }

	void setRenderUi(bool _set) { renderUi = _set; }
	bool getRenderUi() { return renderUi; }

	void setBgmOn() { onPlay = true; }
	bool getBgmOn() { return onPlay; }

	enum_Cleared getCurrentClear() { return clearedStage; }
	void setClear(enum_Cleared _set) { clearedStage = _set; }

	struct_Player getPlayerData() { return playerData; }
	void setPlayerData(struct_Player _set) { playerData = _set; }

	struct_Boss getBossData() { return bossData; }
	void setBossData(struct_Boss _set) { bossData = _set; }

	struct_Switch getReflect() { return reflect; }
	void setReflect(float _x, float _y) 
	{
		reflect.on = true;
		reflect.x = _x;
		reflect.y = _y;
		reflect.isRight = playerData.isRight ? true : false;
	}
	void reflectOff()
	{
		reflect.on = false;
	}

	struct_Switch getHitLazer() { return hitLazer; }
	void setHitLazer(float _x, float _y, float _angle)
	{
		hitLazer.on = true;
		hitLazer.x = _x;
		hitLazer.y = _y;
		hitLazer.angle = _angle;
	}
	void hitLazerOff()
	{
		hitLazer.on = false;
	}

	bool getBossHit() { return bossHit; }
	void setBossHit(bool _set) { bossHit = _set; }

	RECT getPlayerAttRect() { return attRect; }
	RECT getPlayerRECT() { return playerRect; }

	RECT getBossRect() { return bossRect; }
	void setBossRect(RECT rc) { bossRect = rc; }

	void setPlayerRect(RECT rc) { playerRect = rc; }
	void setPlayerAttRect(RECT rc) { attRect = rc; }

	void setPlayerHurt(bool _set)
	{
		hurt = _set;
	}

	bool getPlayerHurt()
	{
		return hurt;
	}

	void setRoll(bool _set)
	{
		roll = _set;
	}

	bool getRoll()
	{
		return roll;
	}

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


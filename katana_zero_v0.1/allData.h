#pragma once
#include "singletonBase.h"
enum enum_Cleared
{
	NONE_CLEARED,
	STAGE1_CLEARED,
	STAGE2_CLEARED,
};

enum enum_PlayerState
{
	P_IDLE,
	P_RUN,
	P_JUMP,
	P_FALL,
	P_ROLL,
};

enum enum_EnemyState
{
	E_IDLE,
	E_WALK,
	E_RUN,
	E_AIM,
	E_HURT,
};

enum enum_EnemyCategory
{
	E_BOLD,
	E_GANG,
};

struct struct_Backgrounds
{
	image* img;
	float x;
	float y;
	int width;
	int height;
};

struct struct_Camera
{
	RECT rc;
	int width;
	int height;
};

struct struct_Player
{
	RECT rc;

	bool isRight;

	enum_PlayerState state;

	float x;
	float y;

	float renderX;

	int height;
	int width;
	float angle;
	float speed;
	float jumpSpeed;
	float fallSpeed;
	float rollSpeed;

};

struct struct_Enemy
{
	RECT rc;

	bool isRight;
	bool isRender;
	bool isGenerate;

	enum_EnemyState state;
	enum_EnemyCategory category;

	image* idleImg;
	image* runImg;
	image* walkImg;
	image* hurtImg;

	float x;
	float y;
	float renderX;

	int width;
	int height;

	int frameCnt;
	int frameIdx;

	float speed;

};


class allData : public singletonBase <allData>
{
private:
	enum_Cleared clearedStage;
	
	POINT startLoc;
	
	struct_Backgrounds bgCollision;
	struct_Backgrounds bgRender;

	int a;

public:
	HRESULT init();
	void release();

	enum_Cleared getCurrentClear() { return clearedStage; }
	void setClear(enum_Cleared _set) { clearedStage = _set; }

	POINT getStartLoc() { return startLoc; }
	void setStartLoc(float _x, float _y) { startLoc.x = _x; startLoc.y = _y; }

	struct_Backgrounds getBgCol() { return bgCollision; }
	void setBgColImg(image* _img) { bgCollision.img = _img; }

	void addBgX(float _x) { bgCollision.x += _x; }
	void minusBgX(float _x) { bgCollision.x -= _x; }
	void addBgY(float _y) { bgCollision.y += _y; }
	void minusBgY(float _y) { bgCollision.y -= _y; }

	struct_Backgrounds getBgRen() { return bgRender; }
	void setBgRenImg(image* _img) { bgRender.img = _img; }
	void setBgRenX(float _x) { bgRender.x = _x; }
	void setBgRenY(float _y) { bgRender.y = _y; }


	void stageInitialize(int _width, int _height) 
	{ 
		bgCollision.x = bgCollision.y = bgRender.x = bgRender.y = 0; 
		bgCollision.width = _width; 
		bgCollision.height = _height; 
	}

	int getA() { return a; }
	void setA(int _set) { a = _set; }

	allData() {}
	~allData() {}
};


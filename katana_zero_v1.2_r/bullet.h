#pragma once
#include "gameNode.h"
#include "effectClass.h"

//총알 구조체
struct tagBullet
{
	image* bulletImage;
	RECT rc;
	float x, y;
	float fireX, fireY;
	float speed;
	float angle;
	float rotateAngle;
	float gravity;
	float radius;
	int frameIdx;
	int frameCnt;
	int moveCnt;
	int stickyTum;
	int randAngle;
	int stickyCnt;
	bool fire;
	bool isReflect;
	bool isSticky;
	int count;
};

//=============================================================
//	## missile ## (missile[0] -> 배열처럼 미리 장전해두고 총알발사)
//=============================================================
class bossBullet : public gameNode
{
private:
	//총알 구조체를 담을 벡터, 반복자
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

private:
	float _range;		//총알 사거리
	int _bulletMax;		//총알 최대갯수

public:
	HRESULT init(int bulletMax, float range);
	void release();
	void update();
	void render();

	//총알발사
	void fire(float x, float y, float _angle);
	//총알무브
	void move();

	bossBullet() {}
	~bossBullet() {}
};

class bossMine : public gameNode
{
private:
	//총알 구조체를 담을 벡터, 반복자
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;
	effectClass* effectPtBullet;

private:
	float _range;		//총알 사거리
	int _bulletMax;		//총알 최대갯수

public:
	HRESULT init(int bulletMax, float range);
	void release();
	void update();
	void render();

	//총알발사
	void fire(float x, float y, float _angle);
	//총알무브
	void move();
	//폭발렉트 가져오기
	RECT getMineRect(int i) { return effectPtBullet->getExplosionRect(i); }

	bossMine() {}
	~bossMine() {}
};





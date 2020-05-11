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
	bool isSticky;
	int count;
};

//=============================================================
//	## bullet ## (공용총알 - 너희들이 만들어야 함)
//=============================================================
class bullet : public gameNode
{
private:
	//총알 구조체를 담을 벡터, 반복자
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

private:
	const char* _imageName;	//총알 이미지 이름
	float _range;			//총알 사거리
	int _bulletMax;			//총알 최대갯수
	bool _isFrameImg;		//프레임 이미지냐?

public:
	HRESULT init(const char* imageName, int bulletMax, float range, bool isFrameImg = false);
	void release();
	void update();
	void render();

	//총알발사
	void fire(float x, float y, float angle, float speed);
	//총알무브
	void move();

	//총알벡터 가져오기
	vector<tagBullet> getBullet() { return _vBullet; }
	//폭탄 삭제
	void removeBullet(int index);

	bullet() {}
	~bullet() {}
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

//=============================================================
//	## bomb ## (폭탄처럼 한발씩 발사하고 생성하고 자동삭제)
//=============================================================
class bomb : public gameNode
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
	void fire(float x, float y);
	//총알무브
	void move();

	//총알벡터 가져오기
	vector<tagBullet> getBullet() { return _vBullet; }
	//폭탄 삭제
	void removeBomb(int index);

	bomb() {}
	~bomb() {}
};




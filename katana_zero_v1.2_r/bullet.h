#pragma once
#include "gameNode.h"
#include "effectClass.h"

//�Ѿ� ����ü
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
//	## missile ## (missile[0] -> �迭ó�� �̸� �����صΰ� �Ѿ˹߻�)
//=============================================================
class bossBullet : public gameNode
{
private:
	//�Ѿ� ����ü�� ���� ����, �ݺ���
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

private:
	float _range;		//�Ѿ� ��Ÿ�
	int _bulletMax;		//�Ѿ� �ִ밹��

public:
	HRESULT init(int bulletMax, float range);
	void release();
	void update();
	void render();

	//�Ѿ˹߻�
	void fire(float x, float y, float _angle);
	//�Ѿ˹���
	void move();

	bossBullet() {}
	~bossBullet() {}
};

class bossMine : public gameNode
{
private:
	//�Ѿ� ����ü�� ���� ����, �ݺ���
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;
	effectClass* effectPtBullet;

private:
	float _range;		//�Ѿ� ��Ÿ�
	int _bulletMax;		//�Ѿ� �ִ밹��

public:
	HRESULT init(int bulletMax, float range);
	void release();
	void update();
	void render();

	//�Ѿ˹߻�
	void fire(float x, float y, float _angle);
	//�Ѿ˹���
	void move();
	//���߷�Ʈ ��������
	RECT getMineRect(int i) { return effectPtBullet->getExplosionRect(i); }

	bossMine() {}
	~bossMine() {}
};





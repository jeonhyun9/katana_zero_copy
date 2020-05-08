#pragma once
#include "gameNode.h"
#include "bullet.h"
class bossClass : public gameNode
{
private:
	struct_Boss boss;
	missile* bulletPt;

	float SLOW_inBoss;
	float SLOW_ratio_inBoss;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//���� ����
	void generateBoss(float _x, float _y, bool _set, enum_BossState _state)
	{
		boss.isGenrateCoolTime = false;
		boss.x = _x;
		boss.y = _y;
		boss.isRight = _set;
		boss.state = _state;

		switch (_state)
		{
		case B_IDLE:
			boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 12;
			break;
		case B_AIM:
			boss.frameIdx = 0;
			break;
		case B_DRAWGUN:
			boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 8;
			break;
		}

	}

	//���� ����
	void setBossHit(bool _set)
	{
		boss.state = B_HURTRECOVER;
		boss.currentPattern++;
		boss.gravity = 0;
		boss.isWallJump = false;
		boss.wallJumpCnt = 0;
		boss.isRight = _set;
		boss.isRight ? boss.frameIdx = 0 : boss.frameIdx = 10;
	}
	
	//���� �ѱ��
	void addPattern()
	{
		boss.currentPattern++;
	}

	void bossOnFloor()
	{
		/*�̺κ��� �ȼ��浹�� �ٽ�*/
		for (int j = boss.findY - 35; j < boss.findY + 35; j++)
		{
			COLORREF color = GetPixel(DATA->getBgCol().img->getMemDC(), boss.x, j);
			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);

			if (!(r == 255 && g == 0 && b == 255))
			{
				float tempY = boss.y;
				boss.y = j - boss.height / 2;
				//ON_FLOOR = true;
				break;
			}
			else
			{
				//ON_FLOOR = false;
			}
		}
	}

	//���� �ִϸ��̼�
	void bossAnimation();

	//���� �ִϸ��̼� ����
	void animationSetFrame(int _frameTum, int _End);

	//���� ����ü ��
	struct_Boss getBoss() { return boss; }
	RECT getBossRect() { return boss.rc; }

	//���� ������ ��Ʈ
	RECT getLazerRect() { return boss.rcLazer; }

	//���� ������ �ִϸ��̼�
	void animationSetLazer();
	//���� ���� ���� ����
	void setBossAimAngle(float _angle)
	{
		boss.aimAngle = _angle;
	}

	void bossPixelCollision();

	void setBossState(enum_BossState _state);

	bossClass() {}
	~bossClass() {}
};


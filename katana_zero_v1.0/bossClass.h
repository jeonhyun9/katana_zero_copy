#pragma once
#include "gameNode.h"
#include "bullet.h"

class bossClass : public gameNode
{
private:
	struct_Boss boss;
	bossBullet* bulletPt;
	bossMine* minePt;

	struct_effect sLazer;
	vector<struct_effect> vLazer;

	int preLocation;
	enum_BossState prePattern;
	int pattern[6];

	bool animationOff;

	float playerX;

	bool SLOW_inBoss;
	float SLOW_ratio_inBoss;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	bossBullet* getBulletPt() { return bulletPt; }

	//������ ���
	struct_effect getLazer(int i) { return vLazer[i]; }
	
	void setPlayerX(float _set)
	{
		playerX = _set;
	}

	//�Ӹ� ������
	void headAnimation();

	//���� ����
	void generateBoss(float _x, float _y, bool _set, enum_BossState _state)
	{
		boss.isGenrateCoolTime = false;
		boss.x = _x;
		boss.y = _y;
		boss.isRight = _set;
		this->setBossState(_state);
	}

	//������ ����
	void generateLazer(float _x, float _y, int _frameidx);
	void generateLazer(float _x, float _y, bool _set, int _frameidx);
	void generateRotateLazer(float _x, float _y,bool _set, int _frameidx);

	vector<struct_effect> getLazerVector()
	{
		return vLazer;
	}

	RECT getLazerRect(int i)
	{
		return vLazer[i].rc;
	}

	//���� ����
	void setBossHit(bool _set);
	
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

	//���� ��ġ�� ��� �Լ�
	void randomLocation();

	//���� ������ �������� ������ �ʰ� �ϴ� �Լ�
	void randomPatten(int _bossX);

	bossClass() {}
	~bossClass() {}
};


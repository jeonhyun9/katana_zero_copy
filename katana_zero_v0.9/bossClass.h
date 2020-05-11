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

	int previousPattern;

	float playerX;

	bool SLOW_inBoss;
	float SLOW_ratio_inBoss;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	void setPlayerX(float _set)
	{
		playerX = _set;
	}

	//보스 생성
	void generateBoss(float _x, float _y, bool _set, enum_BossState _state)
	{
		boss.isGenrateCoolTime = false;
		boss.x = _x;
		boss.y = _y;
		boss.isRight = _set;
		this->setBossState(_state);
	}

	//레이저 생성
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

	//보스 때림
	void setBossHit(bool _set);
	
	//패턴 넘기기
	void addPattern()
	{
		boss.currentPattern++;
	}

	void bossOnFloor()
	{
		/*이부분이 픽셀충돌의 핵심*/
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

	//보스 애니메이션
	void bossAnimation();

	//보스 애니메이션 세팅
	void animationSetFrame(int _frameTum, int _End);

	//보스 구조체 겟
	struct_Boss getBoss() { return boss; }
	RECT getBossRect() { return boss.rc; }

	//보스 레이저 렉트
	RECT getLazerRect() { return boss.rcLazer; }

	//보스 레이저 애니메이션
	void animationSetLazer();
	//보스 조준 각도 설정
	void setBossAimAngle(float _angle)
	{
		boss.aimAngle = _angle;
	}

	void bossPixelCollision();

	void setBossState(enum_BossState _state);

	void randomLocation();

	void randomPatten(int _bossX);

	bossClass() {}
	~bossClass() {}
};


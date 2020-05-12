#pragma once
#include "gameNode.h"

class enemyClass :public gameNode
{
private:
	struct_Enemy sEnemy;
	vector<struct_Enemy> vEnemy;

	int frameY;
	int BULLETSIZE;

	bool SLOW_inEnemy;
	float SLOW_Ratio_inEnemy;

	bool firstSetup;
	
public:
	HRESULT init();
	void release();
	void update();
	void render();

	//���ʹ� ���Լ�
	vector<struct_Enemy> getVectorEnemy() { return vEnemy; }
	void setVectorClear() { vEnemy.clear(); }
	struct_Enemy getEnemy(int i) {return vEnemy[i];}
	RECT getEnemyRect(int i) {return vEnemy[i].rc;}
	RECT getEnemyAttackRect(int i) {return vEnemy[i].rc;}
	RECT getDetectRect(int i) { return vEnemy[i].detect.rc; }
	RECT getEnemyBulletRect(int i, int j) { return vEnemy[i].bullet[j].rc; }

	void setEnemyBulletFire(int i, int j, bool _set) { vEnemy[i].bullet[j].fire = _set; }
	void setEnemyBulletReflect(int i, int j, bool _set, float _angle) { vEnemy[i].bullet[j].isReflected = _set; vEnemy[i].bullet[j].angle = _angle; }
	
	void setDetectRectX(int i,float _set) { vEnemy[i].doorReachedX = _set; }
	void setDoorReached(int i, bool _set) { vEnemy[i].isDoorReached = _set; }
	void setEnemyX(int i, int _set) { vEnemy[i].x += _set; }

	void setEnemyReflectDead(int i)
	{
		vEnemy[i].isReflectDead = true;
	}



	//���ʹ� ���⼳��
	void setEnemyDirection(int i, bool _set) { vEnemy[i].isRight = _set; }
	void setEnemyState(int i,enum_EnemyState _set) { vEnemy[i].state = _set; }

	//isPlayerRight ���Լ�
	void setIsPlayerRight(int i, bool set) { vEnemy[i].isPlayerRight = set; }

	//���ʹ� ���̴� �Լ�
	void setEnemyHurt(int i) 
	{ 
		if (vEnemy[i].state != E_HURT)
		{
			if (vEnemy[i].isPlayerRight)
			{
				vEnemy[i].frameIdxBlood = 0;
				vEnemy[i].frameIdxHitSlash = 0;
			}
			else
			{
				vEnemy[i].frameIdxBlood = 11;
				vEnemy[i].frameIdxHitSlash = 6;
			}

			switch (vEnemy[i].category)
			{
			case E_BOLD:
				if (vEnemy[i].isRight)
				{
					vEnemy[i].frameIdx = 0;
				}
				else
				{
					vEnemy[i].frameIdx = 14;
				}
				break;
			case E_COP:
				if (vEnemy[i].isRight)
				{
					vEnemy[i].frameIdx = 0;
				}
				else
				{
					vEnemy[i].frameIdx = 14;
				}
				break;
			case E_TURRET:
				if (vEnemy[i].isRight)
				{
					vEnemy[i].frameIdx = 0;
				}
				else
				{
					vEnemy[i].frameIdx = 14;
				}
				break;

			case E_POMP:
				if (vEnemy[i].isRight)
				{
					vEnemy[i].frameIdx = 0;
				}
				else
				{
					vEnemy[i].frameIdx = 15;
				}
			case E_GRUNT:
				if (vEnemy[i].isRight)
				{
					vEnemy[i].frameIdx = 0;
				}
				else
				{
					vEnemy[i].frameIdx = 15;
				}
				break;
			}
		}
		vEnemy[i].state = E_HURT;
	}
	void setEnemyHurt(int i, bool _set)
	{
		if (vEnemy[i].state != E_HURT)
		{
			if (vEnemy[i].isPlayerRight)
			{
				vEnemy[i].frameIdxBlood = 0;
				vEnemy[i].frameIdxHitSlash = 0;
			}
			else
			{
				vEnemy[i].frameIdxBlood = 11;
				vEnemy[i].frameIdxHitSlash = 6;
			}

			switch (vEnemy[i].category)
			{
			case E_BOLD:
				if (vEnemy[i].isRight)
				{
					vEnemy[i].frameIdx = 0;
				}
				else
				{
					vEnemy[i].frameIdx = 14;
				}
				break;
			case E_COP:
				if (vEnemy[i].isRight)
				{
					vEnemy[i].frameIdx = 0;
				}
				else
				{
					vEnemy[i].frameIdx = 14;
				}
				break;
			case E_POMP:
				if (vEnemy[i].isRight)
				{
					vEnemy[i].frameIdx = 0;
				}
				else
				{
					vEnemy[i].frameIdx = 15;
				}
			case E_GRUNT:
				if (vEnemy[i].isRight)
				{
					vEnemy[i].frameIdx = 0;
				}
				else
				{
					vEnemy[i].frameIdx = 15;
				}
				break;
			}
		}
		vEnemy[i].isDoorDead = true;
		vEnemy[i].isDoorDeadMove = true;
		vEnemy[i].state = E_HURT;
	}

	//���¾Ƽ� ������ ƨ���..
	void enemyDoorDead(int i);

	void enemyDoorDeadMoveOff(int i)
	{
		vEnemy[i].isDoorDeadMove = false;
	}

	//�νĿ� �Լ�
	void setEnemyFounded(int i, bool _set)
	{
		vEnemy[i].founded = _set;
	}
	void setDistance(int i, float _set) { vEnemy[i].distanceToPlayer = _set; }
	void setAngle(int i, float _set) { vEnemy[i].angleToPlayer = _set; }

	//���ʹ� ����
	void generateEnemy(float _x, float _y, enum_EnemyCategory _category, enum_EnemyState _state , bool _set);
	void generateEnemy(float _x, float _y, int _findRange , enum_EnemyCategory _category, enum_EnemyState _state, bool _set);
	void generateEnemy(float _x, float _y, int _findRange , bool _found ,  enum_EnemyCategory _category, enum_EnemyState _state, bool _set);

	//���ʹ� �ִϸ��̼�
	void enemyAnimation(int i);
	void bloodAnimation(int i);

	//���ʹ� Y��ǥ ���� ���
	void setDistanceY(int i, int _set) { vEnemy[i].distanceToPlayer_Y = _set; }

	//���ʹ� ����
	void enemyPattern(int i);
	void enemySetBullet(int i);
	void enemyShoot(int i);
	void enemyMoveBullet(int i);


	//���ʹ� �÷��̾� �ν�
	void enemyMoveToPlayer(int i);

	//���ʹ� ����
	void enemyMove(int i);

	void enemyPixelCollisionleft(int i);
	void enemyPixelCollisionRight(int i);
	void enemyPixelCollisionBottom(int i);
	void enemyOnFloor(int i);

	enemyClass() {}
	~enemyClass() {}
};


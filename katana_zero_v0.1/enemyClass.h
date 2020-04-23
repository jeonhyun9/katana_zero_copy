#pragma once
#include "gameNode.h"
class enemyClass :public gameNode
{
private:
	struct_Enemy sEnemy;
	vector<struct_Enemy> vEnemy;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//���ʹ� ���Լ�
	vector<struct_Enemy> getVectorEnemy() { return vEnemy; }
	struct_Enemy getEnemy(int i) {return vEnemy[i];}
	RECT getEnemyRect(int i) {return vEnemy[i].rc;}

	//���ʹ� ���Լ�
	void setEnemyState(int i, enum_EnemyState _set) { vEnemy[i].state = _set; }

	//���ʹ� ����
	void generateEnemy(float _x, float _y, enum_EnemyCategory _category);

	//���ʹ� �ִϸ��̼�
	void enemyAnimation(int i);

	enemyClass() {}
	~enemyClass() {}
};


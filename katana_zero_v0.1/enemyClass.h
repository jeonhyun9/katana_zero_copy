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

	//에너미 겟함수
	vector<struct_Enemy> getVectorEnemy() { return vEnemy; }
	struct_Enemy getEnemy(int i) {return vEnemy[i];}
	RECT getEnemyRect(int i) {return vEnemy[i].rc;}

	//에너미 셋함수
	void setEnemyState(int i, enum_EnemyState _set) { vEnemy[i].state = _set; }

	//에너미 생성
	void generateEnemy(float _x, float _y, enum_EnemyCategory _category);

	//에너미 애니메이션
	void enemyAnimation(int i);

	enemyClass() {}
	~enemyClass() {}
};

